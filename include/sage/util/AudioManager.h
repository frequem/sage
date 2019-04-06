#ifndef _SAGE_AUDIOMANAGER_H
#define _SAGE_AUDIOMANAGER_H

#include <sage/cache/AudioCache.h>

#include <string>
#include <set>
#include <SDL2/SDL_mixer.h> 

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
	};
}

#endif // _SAGE_AUDIOMANAGER_H
