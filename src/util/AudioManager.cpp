#include <sage/util/AudioManager.h>
#include <sage/util/macros.h>

#include <climits>

using namespace sage;

AudioManager::AudioManager(AudioCache* audioCache){
	this->audioCache = audioCache;
}

void AudioManager::play(const std::string& soundFile){
	int ch = 0;
	while(Mix_Playing(ch) || this->reservedChannels.find(ch) != this->reservedChannels.end()){
		ch++;
	}
	
	this->playOnChannel(soundFile, ch);
}

void AudioManager::playOnChannel(const std::string& soundFile, int channel){
	if(Mix_AllocateChannels(-1) <= channel){
		Mix_AllocateChannels(channel+1);
	}
	Mix_PlayChannel(channel, audioCache->get(soundFile), 0);
}

void AudioManager::reserveChannel(int channel){
	this->reservedChannels.insert(channel);
}

void AudioManager::unreserveChannel(int channel){
	this->reservedChannels.erase(channel);
}


AudioManager::~AudioManager(){
}
