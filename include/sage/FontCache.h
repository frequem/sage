#ifndef _SAGE_FONTCACHE_H
#define _SAGE_FONTCACHE_H

#include <sage/FileCache.h>

#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <SDL2/SDL.h>
#include "ogl.h"
#include <SDL_ttf/SDL_ttf.h> 
#include <glm/glm.hpp>

namespace sage{
	class FontCache{
	public:
		FontCache(FileCache*);
		void load(const std::string&, int);
		TTF_Font* get(const std::string&, int);
		
		~FontCache();
	private:
		void load_func(const std::string&, int);
		
		FileCache* fileCache;
		std::map<const std::string, std::map<int, std::thread>> threads;
		
		std::mutex fontMutex;
		std::map<const std::string, std::map<int, TTF_Font*>> fonts;
	};
}

#endif // _SAGE_FONTCACHE_H
