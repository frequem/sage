#include <sage/view/Application.h>
#include <sage/util/AudioManager.h>
#include <sage/util/macros.h>

#include <climits>

using namespace sage;

AudioManager::AudioManager(Application& application) : application(&application){}

void AudioManager::play(const std::string& soundFile, int loops){
	int ch = 0;
	while(Mix_Playing(ch) || this->reservedChannels.find(ch) != this->reservedChannels.end()){
		ch++;
	}
	
	this->playOnChannel(soundFile, ch, loops);
}

void AudioManager::play(const std::string& soundFile){ this->play(soundFile, 0); }

void AudioManager::playOnChannel(const std::string& soundFile, int channel, int loops){
	if(Mix_AllocateChannels(-1) <= channel){
		Mix_AllocateChannels(channel+1);
	}
	Mix_PlayChannel(channel, this->application->getAudioCache().get(soundFile), loops);
}

void AudioManager::playOnChannel(const std::string& soundFile, int channel){ this->playOnChannel(soundFile, channel, 0); }

void AudioManager::reserveChannel(int channel){ this->reservedChannels.insert(channel); }
void AudioManager::unreserveChannel(int channel){ this->reservedChannels.erase(channel); }

void AudioManager::pause(int channel){ Mix_Pause(channel); }
void AudioManager::pauseAll(){ this->pause(-1); }
void AudioManager::resume(int channel){ Mix_Resume(channel); }
void AudioManager::resumeAll(){ this->resume(-1); }

AudioManager::~AudioManager(){
}
