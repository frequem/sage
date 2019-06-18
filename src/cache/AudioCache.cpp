#include <sage/cache/AudioCache.h>
#include <sage/view/Application.h>
#include <sage/cache/FileCache.h>
#include <sage/util/ThreadManager.h>
#include <sage/util/macros.h>

using namespace sage;

AudioCache::AudioCache(Application& application) : application(&application){}

void AudioCache::load_func(const std::string& fn){
	SDL_RWops* rwops = SDL_RWFromMem((void*)this->application->getFileCache().get(fn), this->application->getFileCache().size(fn));
	{
		std::lock_guard<std::mutex> guard(this->mtx);
		
		if(this->chunks.find(fn) == this->chunks.end()){//unload has been called, don't load 
			LOG("sage::AudioCache: '%s' has been unloaded before loading could start.", fn.c_str());
			return;
		}else if(this->chunks[fn] != nullptr){
			LOG("sage::AudioCache: '%s' has been unloaded, then loaded again before first loading could start.", fn.c_str());
			return;
		}
		
		this->chunks[fn] = Mix_LoadWAV_RW(rwops, 0);
	}
	this->cv.notify_all();
}

void AudioCache::load(const std::string& fn){
	std::lock_guard<std::mutex> guard(this->mtx);
	if(this->chunks.find(fn) != this->chunks.end()){
		return;
	}
	
	this->chunks[fn] = nullptr;
	this->application->getThreadManager().run(&AudioCache::load_func, this, fn);
}

void AudioCache::unload(const std::string& fn){
	std::lock_guard<std::mutex> guard(this->mtx);
	
	if(this->chunks.find(fn) == this->chunks.end()){
		return;
	}
	
	if(this->chunks[fn] != nullptr){
		Mix_FreeChunk(this->chunks[fn]);
	}
	
	this->chunks.erase(fn);
	
	this->application->getFileCache().unload(fn);
}

Mix_Chunk* AudioCache::get(const std::string& fn){
	this->load(fn);
	
	std::unique_lock<std::mutex> lock(this->mtx);
	while(this->chunks.find(fn) != this->chunks.end() && this->chunks[fn] == nullptr)
		this->cv.wait(lock);
	
	return this->chunks[fn];
}

AudioCache::~AudioCache(){
	while(!this->chunks.empty()){
		this->unload(this->chunks.begin()->first);
	}
}
