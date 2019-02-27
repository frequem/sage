#ifndef _SAGE_AUDIOCACHE_H
#define _SAGE_AUDIOCACHE_H

#include "sage/FileCache.h"

#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL_ttf/SDL_ttf.h> 
#include <SDL2/SDL_mixer.h> 
#include <GL/glu.h>
#include <glm/glm.hpp>

namespace sage{
	class AudioCache{
	public:
		AudioCache(FileCache*);
		void load(const std::string&);
		Mix_Chunk* get(const std::string&);
		
		~AudioCache();
	private:
		void load_func(const std::string&);
		
		FileCache* fileCache;
		std::map<const std::string, std::thread> threads;
		
		std::mutex chunkMutex;
		std::map<const std::string, Mix_Chunk*> chunks;
	};
}

#endif // _SAGE_AUDIOACHE_H
