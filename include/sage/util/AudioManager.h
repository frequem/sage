#ifndef _SAGE_AUDIOMANAGER_H
#define _SAGE_AUDIOMANAGER_H

#include <sage/cache/AudioCache.h>

#include <string>
#include <set>
#include <SDL2/SDL_mixer.h> 
/*#include <map>
#include <mutex>
#include <thread>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h> 
#include <SDL2/SDL_ttf.h> 
#include <GL/glu.h>
#include <glm/glm.hpp>*/

namespace sage{
	class AudioManager{
	public:
		AudioManager(AudioCache*);
		void play(const std::string&);
		void playOnChannel(const std::string&, int);
		void reserveChannel(int);
		void unreserveChannel(int);
		
		~AudioManager();
	private:
		AudioCache* audioCache;
		std::set<int> reservedChannels;
		/*void load_func(const std::string&, int);
		
		std::map<const std::string, std::map<int, std::thread>> threads;
		
		std::mutex fontMutex;
		std::map<const std::string, std::map<int, TTF_Font*>> fonts;*/
	};
}

#endif // _SAGE_AUDIOMANAGER_H
