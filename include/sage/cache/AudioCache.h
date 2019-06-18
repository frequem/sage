#ifndef _SAGE_AUDIOCACHE_H
#define _SAGE_AUDIOCACHE_H

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace sage{
	class Application;
	class AudioCache{
	public:
		AudioCache(Application&);
		void load(const std::string&);
		void unload(const std::string&);
		Mix_Chunk* get(const std::string&);
		
		~AudioCache();
	private:
		void load_func(const std::string&);
		
		Application* application;
		
		std::mutex mtx;
		std::condition_variable cv;
		std::map<const std::string, Mix_Chunk*> chunks;
	};
}

#endif // _SAGE_AUDIOACHE_H
