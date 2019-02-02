#include <sage/ImageCache.h>
#include <sage/macros.h>

using namespace sage;

ImageCache::ImageCache(FileCache* fileCache){
	this->fileCache = fileCache;
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
	
	this->imageMutex.lock();
	int index = this->image_data.size();
	this->image_data.push_back(data);
	this->imageMutex.unlock();
	
	this->textureMutex.lock();
	Texture& t = this->textures[fn] = Texture();
	t.size.x = w;
	t.size.y = h;
	t.index = index;
	this->textureMutex.unlock();
}

void ImageCache::load(const std::string& fn){
	if(this->threads.find(fn) != this->threads.end()){
		return;
	}
	
	this->threads[fn] = std::thread(&ImageCache::load_func, this, fn);
}

void ImageCache::waitfor(const std::string& fn){
	this->load(fn);
	
	if(this->threads[fn].joinable()){ //image still loading?
		this->threads[fn].join();
	}
}

void ImageCache::createTexture(const std::string& fn){
	this->waitfor(fn);
	
	this->textureMutex.lock();
	Texture& t = this->textures[fn];
	
	if(t.index >= 0){
		this->imageMutex.lock();
		unsigned char* image = this->image_data[t.index];
		
		glGenTextures(1, &t.tid);
		glBindTexture(GL_TEXTURE_2D, t.tid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.size.x, t.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		SOIL_free_image_data(image);
		
		this->imageMutex.unlock();
		
		t.index = -1;
	}
	this->textureMutex.unlock();
}

GLuint ImageCache::getTexture(const std::string& fn){
	this->createTexture(fn);
	
	this->textureMutex.lock();
	GLuint tid = this->textures[fn].tid;
	this->textureMutex.unlock();
	
	return tid;
}

glm::vec2 ImageCache::getSize(const std::string& fn){
	this->waitfor(fn);
	
	this->textureMutex.lock();
	glm::vec2 size = this->textures[fn].size;
	this->textureMutex.unlock();
	
	return size;
}

float ImageCache::getWidth(const std::string& fn){
	return this->getSize(fn).x;
}

float ImageCache::getHeight(const std::string& fn){
	return this->getSize(fn).y;
}

ImageCache::~ImageCache(){
	for(std::map<const std::string, std::thread>::iterator i=this->threads.begin(); i!=this->threads.end(); i++){
		std::thread& t = i->second;//std::get<std::thread>(*i);
		if(t.joinable()){
			t.join();
		}
	}
	
	this->textureMutex.lock();
	for(std::map<const std::string, Texture>::iterator i=this->textures.begin(); i!=this->textures.end(); i++){
		Texture& t = i->second;//std::get<Texture>(*i);
		if(t.index >= 0){
			this->imageMutex.lock();
			SOIL_free_image_data(this->image_data[t.index]);
			this->imageMutex.unlock();
		}else{
			glDeleteTextures(1, &t.tid);
		}
	}
	this->textureMutex.unlock();
}
