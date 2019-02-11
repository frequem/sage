#include <sage/cache/FileCache.h>
#include <sage/util/macros.h>
#include <fstream>
#include <iostream>

using namespace sage;

FileCache::FileCache(ThreadManager* tm){
	this->threadManager = tm;
}

void FileCache::load_func(const std::string& fn){
	std::ifstream file(fn, std::ios::binary | std::ios::ate); 
	std::streamsize size = file.tellg();
	
	ASSERT(file.good(), "File '%s' does not exist.", fn.c_str());
	
	file.seekg(0, std::ios::beg);
	
	{
		std::lock_guard<std::mutex> guard(this->mtx);
	
		if(this->data.find(fn) == this->data.end()){//unload has been called, don't load 
			LOG("sage::FileCache: '%s' has been unloaded before loading could start.", fn.c_str());
			return;
		}else if(this->data[fn].has_value()){
			LOG("sage::FileCache: '%s' has been unloaded, then loaded again before first loading could start.", fn.c_str());
			return;
		}
		
		this->data[fn] = std::vector<char>(size);
		file.read(this->data[fn]->data(), size);
	}
	
	this->cv.notify_all(); //notify get/size
	
	ASSERT(file.good(), "Error loading file '%s'", fn.c_str());
	
	file.close();
	
	LOG("sage::FileCache: Loaded '%s'", fn.c_str());
}

void FileCache::load(const std::string& fn){
	std::lock_guard<std::mutex> guard(this->mtx);
	
	if(this->data.find(fn) != this->data.end()){
		return;
	}
	
	this->data[fn] = std::nullopt;
	this->threadManager->run(&FileCache::load_func, this, fn);
}

void FileCache::unload(const std::string& fn){
	std::lock_guard<std::mutex> guard(this->mtx);
	
	if(this->data.find(fn) == this->data.end()){
		return;
	}
		
	this->data.erase(fn); //remove file data or nullopt
	
	LOG("sage::FileCache: Unloaded '%s'", fn.c_str());
}

const char* FileCache::get(const std::string& fn){
	this->load(fn);
	
	std::unique_lock<std::mutex> lock(this->mtx);
	while(this->data.find(fn) != this->data.end() && !this->data[fn].has_value())
		this->cv.wait(lock);
	
	return this->data[fn]->data();
}

int FileCache::size(const std::string& fn){
	this->load(fn);
	
	std::unique_lock<std::mutex> lock(this->mtx);
	while(this->data.find(fn) != this->data.end() && !this->data[fn].has_value())
		this->cv.wait(lock);
	
	if(!this->data[fn].has_value()) //should not be possible because load was called earlier
		return 0;
	
	return this->data[fn]->size() * sizeof(char);
}

FileCache::~FileCache(){
}
