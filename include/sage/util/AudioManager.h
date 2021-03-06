#ifndef _SAGE_AUDIOMANAGER_H
#define _SAGE_AUDIOMANAGER_H

#include <string>
#include <set>
#include <SDL2/SDL_mixer.h> 

namespace sage{
	class Application;
	class AudioManager{
	public:
		AudioManager(Application&);
		void play(const std::string&, int);
		void playOnChannel(const std::string&, int, int);
		void play(const std::string&);
		void playOnChannel(const std::string&, int);
		
		void reserveChannel(int);
		void unreserveChannel(int);
		
		void pauseAll();
		void pause(int);
		void resumeAll();
		void resume(int);
		
		~AudioManager();
	private:
		Application* application;
		std::set<int> reservedChannels;
	};
}

#endif // _SAGE_AUDIOMANAGER_H
