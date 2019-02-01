#include "AudioCache.h"
#include "macros.h"

using namespace sage;

AudioCache::AudioCache(FileCache* fileCache){
	this->fileCache = fileCache;
}

void AudioCache::load_func(const std::string& fn){
	this->chunkMutex.lock();
	SDL_RWops* rwops = SDL_RWFromMem((void*)this->fileCache->get(fn), this->fileCache->size(fn));
	this->chunks[fn] = Mix_LoadWAV_RW(rwops, 0);
	this->chunkMutex.unlock();
}

void AudioCache::load(const std::string& fn){
	if(this->threads.find(fn) != this->threads.end()){
		return;
	}else{
		this->threads[fn] = std::thread();
	}
	
	this->threads[fn] = std::thread(&AudioCache::load_func, this, fn);
}


Mix_Chunk* AudioCache::get(const std::string& fn){
	this->load(fn);
	
	if(this->threads[fn].joinable()){
		this->threads[fn].join();
	}
	
	this->chunkMutex.lock();
	Mix_Chunk* chunk = this->chunks[fn];
	this->chunkMutex.unlock();
	
	return chunk;
}

AudioCache::~AudioCache(){
	for(std::map<const std::string, std::thread>::iterator i = this->threads.begin(); i!=this->threads.end(); i++){
		std::thread& t = i->second;//std::get<std::thread>(*i);
		if(t.joinable()){
			t.join();
		}
	}
	
	this->chunkMutex.lock();
	for(std::map<const std::string, Mix_Chunk*>::iterator i = this->chunks.begin(); i!=this->chunks.end(); i++){
		Mix_Chunk* chunk = i->second;//std::get<Mix_Chunk*>(*i);
		Mix_FreeChunk(chunk);
	}
	this->chunkMutex.unlock();
}
