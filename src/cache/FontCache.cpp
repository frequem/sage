#include <sage/cache/FontCache.h>
#include <sage/util/macros.h>

using namespace sage;

FontCache::FontCache(FileCache* fileCache, ThreadManager* tm){
	this->fileCache = fileCache;
	this->threadManager = tm;
}

void FontCache::load_func(const std::string& fn, int ptsize){
	SDL_RWops* rwops = SDL_RWFromMem((void*)this->fileCache->get(fn), this->fileCache->size(fn));
	{
		std::lock_guard<std::mutex> guard(this->mtx);
		
		if(this->fonts.find(fn) == this->fonts.end() || this->fonts[fn].find(ptsize) == this->fonts[fn].end()){//unload has been called, don't load 
			LOG("sage::FontCache: '%s' has been unloaded before loading could start.", fn.c_str());
			return;
		}else if(this->fonts.find(fn) != this->fonts.end() && this->fonts[fn][ptsize] != nullptr){
			LOG("sage::FontCache: '%s' has been unloaded, then loaded again before first loading could start.", fn.c_str());
			return;
		}
		
		this->fonts[fn][ptsize] = TTF_OpenFontRW(rwops, 0, ptsize);
	}
	this->cv.notify_all();
}

void FontCache::load(const std::string& fn, int ptsize){
	std::lock_guard<std::mutex> guard(this->mtx);
	if(this->fonts.find(fn) != this->fonts.end()){
		if(this->fonts[fn].find(ptsize) != this->fonts[fn].end()){
			return;
		}
	}else{
		this->fonts[fn] = std::map<int, TTF_Font*>();
	}
	
	this->fonts[fn][ptsize] = nullptr;
	this->threadManager->run(&FontCache::load_func, this, fn, ptsize);
}

void FontCache::unload(const std::string& fn, int ptsize){
	std::lock_guard<std::mutex> guard(this->mtx);
	if(this->fonts.find(fn) == this->fonts.end()){ //no such font
		return;
	}else if(this->fonts[fn].find(ptsize) == this->fonts[fn].end()){ //no such pointsize
		return;
	}
	
	//delete a single font
	
	if(this->fonts[fn][ptsize] != nullptr){
		TTF_CloseFont(this->fonts[fn][ptsize]);
	}
	
	this->fonts[fn].erase(ptsize);
	
	if(this->fonts[fn].size() == 0){ //erase outer map when its empty
		this->fonts.erase(fn);
		this->fileCache->unload(fn);
	}
}

void FontCache::unload(const std::string& fn){
	while(this->fonts.find(fn) != this->fonts.end() && !this->fonts[fn].empty()){
		this->unload(fn, this->fonts[fn].begin()->first);
	}
}

TTF_Font* FontCache::get(const std::string& fn, int ptsize){
	this->load(fn, ptsize);
	
	std::unique_lock<std::mutex> lock(this->mtx);
	while(this->fonts.find(fn) != this->fonts.end() && this->fonts[fn].find(ptsize) != this->fonts[fn].end() && this->fonts[fn][ptsize] == nullptr)
		this->cv.wait(lock);
	
	return this->fonts[fn][ptsize];
}

FontCache::~FontCache(){
	while(!this->fonts.empty()){
		this->unload(this->fonts.begin()->first);
	}
}
