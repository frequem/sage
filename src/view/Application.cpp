#include <sage/view/Application.h>
#include <sage/util/config.h>
#include <sage/util/macros.h>
#include <sage/view/Scene.h>

using namespace sage;

Application::Application() : Application("sage Application"){}

Application::Application(const std::string& title) : Application(title, 960, 540){}

Application::Application(const std::string& title, int width, int height){
	ASSERT(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == 0, "Failed to initialize SDL: %s", SDL_GetError());
	
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
	
	ASSERT(SDL_GL_SetSwapInterval(1) == 0, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	
	glEnable(GL_STENCIL_TEST);
	
	//sdl ttf
	ASSERT(TTF_Init() == 0, "Failed to initialize SDL_ttf: %s", TTF_GetError());
	//sdl mixer
	//int flags = MIX_INIT_MP3;
	//ASSERT((Mix_Init(flags)&flags) == flags, "Could not initialize mp3 loader: %s", Mix_GetError());
	ASSERT(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == 0, "Failed to initialize SDL2_mixer: %s", Mix_GetError());
	
	this->threadManager = new ThreadManager(MAX_THREAD_COUNT);
	this->fileCache = new FileCache(this->threadManager);
	this->imageCache = new ImageCache(this->fileCache, this->threadManager);
	this->fontCache = new FontCache(this->fileCache, this->threadManager);
	this->shaderCache = new ShaderCache(this->fileCache);
	this->audioCache = new AudioCache(this->fileCache, this->threadManager);
	this->audioManager = new AudioManager(this->audioCache);
	
	this->lastUpdate = SDL_GetTicks();
}

void Application::pushScene(Scene* scene){
	scene->setApplication(this);
	this->scenes.push(scene);
	scene->init();
}

void Application::replaceScene(Scene* scene){
	this->popScene();
	this->pushScene(scene);
}

Scene* Application::getScene(){
	return this->scenes.top();
}

void Application::popScene(){
	auto s = this->getScene();
	this->scenes.pop();
	delete s;
}

void Application::handleEvents(){
	while(SDL_PollEvent(&sdlEvent) != 0){
		switch(sdlEvent.type){
			case SDL_WINDOWEVENT:
				switch(sdlEvent.window.event){
					case SDL_WINDOWEVENT_FOCUS_LOST:
					case SDL_WINDOWEVENT_MINIMIZED:
						this->isPaused = true;
						this->getAudioManager()->pauseAll();
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_RESTORED:
						this->isPaused = false;
						this->getAudioManager()->resumeAll();
						this->lastUpdate = SDL_GetTicks();
						break;
				}
				break;
			case SDL_QUIT:
				this->isRunning = false;
				break;
		}
	}
}

void Application::run(){
	ASSERT(this->scenes.size() > 0, "Application::run - No Scene");
	
	this->isRunning = true;
	
	while(this->isRunning){
		if(!this->isPaused){
			uint32_t time = SDL_GetTicks();
			uint32_t diff = time - this->lastUpdate;
			float diff_f = diff / 1000.0f;
			
			this->getScene()->update(diff_f);
			this->getScene()->render();
			SDL_GL_SwapWindow(this->sdlWindow);
			
			this->lastUpdate = time;
		}
		
		SDL_Delay(1000/FPS);
		this->handleEvents();
	}
}

glm::vec2 Application::getWindowSize(){
	int w, h;
	SDL_GetWindowSize(this->sdlWindow, &w, &h);
	return glm::vec2(w, h);
}

float Application::getWindowWidth(){
	return this->getWindowSize().x;
}

float Application::getWindowHeight(){
	return this->getWindowSize().y;
}

void Application::setWindowSize(glm::vec2 size){
	SDL_SetWindowSize(this->sdlWindow, size.x, size.y);
}

FileCache* Application::getFileCache(){ return this->fileCache; }

ImageCache* Application::getImageCache(){ return this->imageCache; }

FontCache* Application::getFontCache(){ return this->fontCache; }

ShaderCache* Application::getShaderCache(){ return this->shaderCache; }

AudioCache* Application::getAudioCache(){
	return this->audioCache;
}

AudioManager* Application::getAudioManager(){
	return this->audioManager;
}

Application::~Application(){
	LOG("sage::Application Destructor");
	while(!this->scenes.empty()){
		this->popScene();
	}
	
	delete this->imageCache;
	delete this->fontCache;
	delete this->shaderCache;
	delete this->audioCache;
	delete this->audioManager;
	delete this->fileCache;
	delete this->threadManager;
	
	Mix_CloseAudio();
	
	TTF_Quit();
	
	SDL_GL_DeleteContext(this->glContext);
	SDL_DestroyWindow(this->sdlWindow);
	SDL_Quit();
	
}

