#ifndef _SAGE_FONTCACHE_H
#define _SAGE_FONTCACHE_H

#include <sage/cache/FileCache.h>

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>

#include <SDL2/SDL.h>
#include <sage/util/ogl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <glm/glm.hpp>

namespace sage{
	class FontCache{
	public:
		FontCache(FileCache*, ThreadManager*);
		void load(const std::string&, int);
		void unload(const std::string&, int);
		void unload(const std::string&);
		TTF_Font* get(const std::string&, int);
		
		~FontCache();
	private:
		void load_func(const std::string&, int);
		
		ThreadManager* threadManager;
		FileCache* fileCache;
		
		std::mutex mtx;
		std::condition_variable cv;
		std::map<const std::string, std::map<int, TTF_Font*>> fonts;
	};
}

#endif // _SAGE_FONTCACHE_H
