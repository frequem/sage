#include <sage/FileCache.h>
#include <sage/macros.h>
#include <fstream>
#include <iostream>

using namespace sage;

FileCache::FileCache(){
}

void FileCache::load_func(const std::string& fn){
	LOG("sage::FileCache: Loading '%s'", fn.c_str());
	std::ifstream file(fn, std::ios::binary | std::ios::ate); 
	std::streamsize size = file.tellg();
	
	ASSERT(file.good(), "File '%s' does not exist.", fn.c_str());
	
	this->cacheMutex.lock();
	this->cache[fn] = std::vector<char>(size);
	file.seekg(0, std::ios::beg);
	file.read(this->cache[fn].data(), size);
	this->cacheMutex.unlock();
	
	ASSERT(file.good(), "Error loading file '%s'", fn.c_str());
	file.close();
}

void FileCache::load(const std::string& fn){
	if(this->threads.find(fn) != this->threads.end()){
		return;
	}
	
	this->threads[fn] = std::thread(&FileCache::load_func, this, fn);	
}

void FileCache::waitfor(const std::string& fn){
	this->load(fn);
	
	if(this->threads[fn].joinable()){ //file still loading?
		this->threads[fn].join();
	}
}

const char* FileCache::get(const std::string& fn){
	this->waitfor(fn);
	
	this->cacheMutex.lock();
	char* data = this->cache[fn].data();
	this->cacheMutex.unlock();
	
	return data;
}

int FileCache::size(const std::string& fn){
	this->waitfor(fn);
	
	this->cacheMutex.lock();
	int s = this->cache[fn].size() * sizeof(char);
	this->cacheMutex.unlock();
	
	return s;
}

FileCache::~FileCache(){
	for(std::map<const std::string, std::thread>::iterator i=this->threads.begin(); i!=this->threads.end(); i++){
		std::thread& t = i->second;
		if(t.joinable()){
			t.join();
		}
	}
}
