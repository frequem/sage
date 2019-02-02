#include <sage/FontCache.h>
#include <sage/macros.h>

using namespace sage;

FontCache::FontCache(FileCache* fileCache){
	this->fileCache = fileCache;
}

void FontCache::load_func(const std::string& fn, int ptsize){
	this->fontMutex.lock();
	SDL_RWops* rwops = SDL_RWFromMem((void*)this->fileCache->get(fn), this->fileCache->size(fn));
	this->fonts[fn][ptsize] = TTF_OpenFontRW(rwops, 0, ptsize);
	this->fontMutex.unlock();
}

void FontCache::load(const std::string& fn, int ptsize){
	if(this->threads.find(fn) != this->threads.end()){
		if(this->threads[fn].find(ptsize) != this->threads[fn].end()){
			return;
		}
	}else{
		this->threads[fn] = std::map<int, std::thread>();
	}
	
	this->threads[fn][ptsize] = std::thread(&FontCache::load_func, this, fn, ptsize);
}


TTF_Font* FontCache::get(const std::string& fn, int ptsize){
	this->load(fn, ptsize);
	
	if(this->threads[fn][ptsize].joinable()){
		this->threads[fn][ptsize].join();
	}
	
	this->fontMutex.lock();
	TTF_Font* font = this->fonts[fn][ptsize];
	this->fontMutex.unlock();
	
	return font;
}

FontCache::~FontCache(){
	for(std::map<const std::string, std::map<int, std::thread>>::iterator i=this->threads.begin(); i!=this->threads.end(); i++){
		std::map<int, std::thread>& m = i->second;
		for(std::map<int, std::thread>::iterator j=m.begin(); j!=m.end(); j++){
			std::thread& t = j->second;
			if(t.joinable()){
				t.join();
			}
		}
	}
	
	this->fontMutex.lock();
	for(std::map<const std::string, std::map<int, TTF_Font*>>::iterator i=this->fonts.begin(); i!=this->fonts.end(); i++){
		std::map<int, TTF_Font*>& m = i->second;
		for(std::map<int, TTF_Font*>::iterator j=m.begin(); j!=m.end(); j++){
			TTF_Font* font = j->second;
			TTF_CloseFont(font);
		}
	}
	this->fontMutex.unlock();
}
