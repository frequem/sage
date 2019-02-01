#ifndef _SAGE_APPLICATION_H
#define _SAGE_APPLICATION_H

#include "sage/FileCache.h"
#include "sage/ImageCache.h"
#include "sage/ShaderCache.h"
#include "sage/FontCache.h"
#include "sage/AudioCache.h"
#include "sage/AudioManager.h"

#include <stack>
#include <SDL2/SDL.h>
#include "ogl.h"
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
		glm::vec2 getWindowSize();
		float getWindowWidth();
		float getWindowHeight();
		
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
