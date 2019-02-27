#ifndef _SAGE_AUDIOCACHE_H
#define _SAGE_AUDIOCACHE_H

#include <sage/cache/FileCache.h>

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <SDL2/SDL_mixer.h> 
#include <GL/glu.h>
#include <glm/glm.hpp>

namespace sage{
	class AudioCache{
	public:
		AudioCache(FileCache*, ThreadManager*);
		void load(const std::string&);
		void unload(const std::string&);
		Mix_Chunk* get(const std::string&);
		
		~AudioCache();
	private:
		void load_func(const std::string&);
		
		ThreadManager* threadManager;
		FileCache* fileCache;
		
		std::mutex mtx;
		std::condition_variable cv;
		std::map<const std::string, Mix_Chunk*> chunks;
	};
}

#endif // _SAGE_AUDIOACHE_H
