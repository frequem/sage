#include <sage/view/Application.h>
#include <sage/util/config.h>
#include <sage/util/macros.h>
#include <sage/view/Scene.h>
#include <sage/renderer/BasicRenderer.h>
#include <sage/renderer/DepthPeelRenderer.h>

using namespace sage;

Application::Application() : Application("sage Application"){}

Application::Application(const std::string& title) : Application(title, 960, 540){}

Application::Application(const std::string& title, int width, int height){
	ASSERT(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Failed to initialize SDL: %s", SDL_GetError());
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	#ifdef __ANDROID__
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		this->sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		this->sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	#endif
	
	ASSERT(this->sdlWindow != NULL, "Window could not be created! SDL Error: %s", SDL_GetError() );
	
	this->glContext = SDL_GL_CreateContext(this->sdlWindow);
	ASSERT(this->glContext != NULL, "OpenGL context could not be created! SDL Error: %s", SDL_GetError() );
	
	#ifndef __ANDROID__
		GLenum glewError = glewInit();
		ASSERT(glewError == GLEW_OK, "Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	#endif //__ANDROID__
	
	//call may not be supported on all systems -> no assert
	if(SDL_GL_SetSwapInterval(-1)==-1){
		if(SDL_GL_SetSwapInterval(1)==-1){
			SDL_GL_SetSwapInterval(0);
		}
	}
	
	ASSERT(TTF_Init() == 0, "Failed to initialize SDL_ttf: %s", TTF_GetError());
	ASSERT(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == 0, "Failed to initialize SDL2_mixer: %s", Mix_GetError());
	
	this->threadManager = std::make_unique<ThreadManager>(MAX_THREAD_COUNT);
	this->fileCache = std::make_unique<FileCache>(*this);
	this->imageCache = std::make_unique<ImageCache>(*this);
	this->fontCache = std::make_unique<FontCache>(*this);
	this->shaderCache = std::make_unique<ShaderCache>(*this);
	this->audioCache = std::make_unique<AudioCache>(*this);
	this->audioManager = std::make_unique<AudioManager>(*this);
	this->eventDispatcher = std::make_unique<EventDispatcher>(*this);
	this->renderer = std::make_unique<BasicRenderer>(*this);
	
	this->getEventDispatcher().addEventHandler(Event::WINDOW_LEAVE, std::function<void()>([this](){
		this->isPaused = true;
		this->getAudioManager().pauseAll();
	}));
	
	this->getEventDispatcher().addEventHandler(Event::WINDOW_ENTER, std::function<void()>([this](){
		this->isPaused = false;
		this->getAudioManager().resumeAll();
		this->lastUpdate = SDL_GetTicks();
	}));
	
	this->getEventDispatcher().addEventHandler(Event::QUIT, std::function<void()>([this](){
		this->isRunning = false;
	}));
	
	this->lastUpdate = SDL_GetTicks();
}

void Application::pushScene(std::shared_ptr<Scene> scene){
	scene->setApplication(*this);
	this->scenes.push(scene);
	scene->init();
}

void Application::replaceScene(std::shared_ptr<Scene> scene){
	this->popScene();
	this->pushScene(scene);
}

std::shared_ptr<Scene> Application::getScene(){ return this->scenes.top(); }

void Application::popScene(){ this->scenes.pop(); }

void Application::run(){
	ASSERT(this->scenes.size() > 0, "Application::run - No Scene");
	
	this->isRunning = true;
	
	uint32_t time;
	int32_t delay;
	while(this->isRunning){
		time = SDL_GetTicks();
		delay = 1000/FPS;
		if(!this->isPaused){			
			this->getScene()->update((time-this->lastUpdate)/1000.0f);//time since last update in s
			//this->getScene()->render();
			this->getRenderer().render();
			SDL_GL_SwapWindow(this->sdlWindow);
			
			this->lastUpdate = time;
			delay -= SDL_GetTicks()-time;//subtract render time
		}
		
		if(delay > 0){ SDL_Delay(delay); }
		
		this->eventDispatcher->handleEvents();
	}
}

glm::vec2 Application::getWindowSize(){
	int w, h;
	SDL_GetWindowSize(this->sdlWindow, &w, &h);
	return glm::vec2(w, h);
}

void Application::setWindowSize(glm::vec2 size){
	SDL_SetWindowSize(this->sdlWindow, size.x, size.y);
}

float Application::getWindowWidth(){ return this->getWindowSize().x; }
float Application::getWindowHeight(){ return this->getWindowSize().y; }

glm::vec2 Application::getDPI(){
	float vdpi, hdpi;
	#ifdef __ANDROID__
		hdpi = 400;
		vdpi = 400;
	#else
		SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(this->sdlWindow), nullptr, &hdpi, &vdpi);
	#endif
	return glm::vec2(hdpi, vdpi);
}

FileCache& Application::getFileCache(){ return *(this->fileCache); }
ImageCache& Application::getImageCache(){ return *(this->imageCache); }
FontCache& Application::getFontCache(){ return *(this->fontCache); }
ShaderCache& Application::getShaderCache(){ return *(this->shaderCache); }
AudioCache& Application::getAudioCache(){ return *(this->audioCache); }
AudioManager& Application::getAudioManager(){ return *(this->audioManager); }
ThreadManager& Application::getThreadManager(){ return *(this->threadManager); }
EventDispatcher& Application::getEventDispatcher(){ return *(this->eventDispatcher); }

Renderer& Application::getRenderer(){ return *(this->renderer); }
void Application::setRenderer(std::unique_ptr<Renderer> renderer){ this->renderer = std::move(renderer); }

Application::~Application(){
	LOG("sage::Application Destructor");
	
	this->audioCache.reset();
	Mix_CloseAudio();
	this->fontCache.reset();
	TTF_Quit();
	
	SDL_GL_DeleteContext(this->glContext);
	SDL_DestroyWindow(this->sdlWindow);
	SDL_Quit();
	
}

