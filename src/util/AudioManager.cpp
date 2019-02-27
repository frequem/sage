#include "sage/AudioManager.h"
#include "sage/macros.h"

#include <climits>

using namespace sage;

AudioManager::AudioManager(AudioCache* audioCache){
	this->audioCache = audioCache;
}

void AudioManager::play(const std::string& soundFile){
	int ch = 0;
	while(true){
		if(!Mix_Playing(ch) && this->reservedChannels.find(ch) == this->reservedChannels.end())
			break;
		ch++;
	}
	
	this->playOnChannel(soundFile, ch);
}

void AudioManager::playOnChannel(const std::string& soundFile, int channel){
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
