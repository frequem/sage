#ifndef _SAGE_APPLICATION_H
#define _SAGE_APPLICATION_H

#include <sage/cache/FileCache.h>
#include <sage/cache/ImageCache.h>
#include <sage/cache/ShaderCache.h>
#include <sage/cache/FontCache.h>
#include <sage/cache/AudioCache.h>
#include <sage/view/AudioManager.h>

#include <stack>
#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <SDL2/SDL_mixer.h> 
#include <glm/glm.hpp>
#include <string>

namespace sage{
	class Scene;
	
	class Application{
	public:
		Application();
		Application(const std::string&);
		Application(const std::string&, int, int);
		
		void pushScene(Scene*);
		void replaceScene(Scene*);
		Scene* getScene();
		void popScene();
		
		void setWindowSize(glm::vec2);
				
		glm::vec2 getWindowSize();
		float getWindowWidth();
		float getWindowHeight();
		
		FileCache* getFileCache();
		ImageCache* getImageCache();
		FontCache* getFontCache();
		ShaderCache* getShaderCache();
		AudioCache* getAudioCache();
		
		AudioManager* getAudioManager();
		
		void run();
		~Application();
	protected:
		std::stack<Scene*> scenes;
		uint32_t lastUpdate = 0;
		
		ThreadManager* threadManager;
		FileCache* fileCache;
		ImageCache* imageCache;
		ShaderCache* shaderCache;
		FontCache* fontCache;
		AudioCache* audioCache;
		
		AudioManager* audioManager;
		
		bool isRunning = false;
		SDL_Event sdlEvent;
		void handleEvents();
		
		SDL_Window* sdlWindow;
		SDL_GLContext glContext;
	};
}
#endif // _SAGE_APPLICATION_H
