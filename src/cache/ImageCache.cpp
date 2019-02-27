#include <sage/cache/ImageCache.h>
#include <sage/util/macros.h>

using namespace sage;

ImageCache::ImageCache(FileCache* fileCache, ThreadManager* tm){
	this->fileCache = fileCache;
	this->threadManager = tm;
}

void ImageCache::load_func(const std::string& fn){
	int w, h; 
	unsigned char* data = SOIL_load_image_from_memory(
		(unsigned char*)this->fileCache->get(fn), 
		this->fileCache->size(fn),
		&w,
		&h,
		0,
		SOIL_LOAD_RGBA
	);
	
	ASSERT(data != nullptr, "SOIL loading error: '%s'\n", SOIL_last_result());
	
	{
		std::lock_guard<std::mutex> guard(this->mtx);
		
		if(this->textures.find(fn) == this->textures.end()){//unload has been called, don't load 
			LOG("sage::ImageCache: '%s' has been unloaded before loading could start.", fn.c_str());
			SOIL_free_image_data(data);
			return;
		}else if(this->textures[fn].has_value()){
			LOG("sage::ImageCache: '%s' has been unloaded, then loaded again before first loading could start.", fn.c_str());
			SOIL_free_image_data(data);
			return;
		}
			
		this->textures[fn] = {glm::vec2(w, h), data};
	}
	this->cv.notify_all();
}

void ImageCache::load(const std::string& fn){
	std::lock_guard<std::mutex> guard(this->mtx);
	
	if(this->textures.find(fn) != this->textures.end()){
		return;
	}
	
	this->textures[fn] = std::nullopt;
	this->threadManager->run(&ImageCache::load_func, this, fn);
}

void ImageCache::unload(const std::string& fn){
	std::lock_guard<std::mutex> guard(this->mtx);
	
	if(this->textures.find(fn) == this->textures.end()){
		return;
	}
	
	if(this->textures[fn].has_value()){
		if(std::holds_alternative<unsigned char*>(this->textures[fn]->second))
			SOIL_free_image_data(std::get<unsigned char*>(this->textures[fn]->second));
		else
			glDeleteTextures(1, &std::get<GLuint>(this->textures[fn]->second));
	}
	
	this->textures.erase(fn);
	
	this->fileCache->unload(fn);
}

GLuint ImageCache::getTexture(const std::string& fn){
	this->load(fn);
	
	std::unique_lock<std::mutex> lock(this->mtx);
	while(this->textures.find(fn) != this->textures.end() && !this->textures[fn].has_value())
		this->cv.wait(lock);
	
	if(!this->textures[fn].has_value()) //should not be possible because load was called earlier
		return 0;
		
	if(std::holds_alternative<unsigned char*>(this->textures[fn]->second)){
		unsigned char* image_data = std::get<unsigned char*>(this->textures[fn]->second);
		GLuint tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			this->textures[fn]->first.x,
			this->textures[fn]->first.y,
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			image_data
		);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		SOIL_free_image_data(image_data);
		
		this->textures[fn]->second = tid;
	}
		
	return std::get<GLuint>(this->textures[fn]->second);
}

glm::vec2 ImageCache::getSize(const std::string& fn){
	this->load(fn);
	
	std::unique_lock<std::mutex> lock(this->mtx);
	while(this->textures.find(fn) != this->textures.end() && !this->textures[fn].has_value())
		this->cv.wait(lock);
		
	return this->textures[fn]->first;
}

float ImageCache::getWidth(const std::string& fn){
	return this->getSize(fn).x;
}

float ImageCache::getHeight(const std::string& fn){
	return this->getSize(fn).y;
}

ImageCache::~ImageCache(){
	while(!this->textures.empty()){
		this->unload(this->textures.begin()->first);
	}
}
