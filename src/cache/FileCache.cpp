#include <sage/cache/FileCache.h>
#include <sage/util/macros.h>
#include <fstream>
#include <iostream>

using namespace sage;

FileCache::FileCache(ThreadManager* tm){
	this->threadManager = tm;
	
	#ifdef __ANDROID__
		this->android_jnienv = (JNIEnv*)SDL_AndroidGetJNIEnv();
		jobject activity = (jobject)SDL_AndroidGetActivity();
		
		jclass activity_class = this->android_jnienv->GetObjectClass(activity);
		jmethodID activity_class_getAssets = this->android_jnienv->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
		
		jobject asset_manager = this->android_jnienv->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
		this->glob_android_amgr = this->android_jnienv->NewGlobalRef(asset_manager);
		
		this->android_jnienv->DeleteLocalRef(activity);
		this->android_jnienv->DeleteLocalRef(activity_class);
		
		this->android_amgr = AAssetManager_fromJava(this->android_jnienv, this->glob_android_amgr);
	#endif
}

void FileCache::load_func(const std::string& fn){
	#ifdef __ANDROID__
		AAsset* asset = AAssetManager_open(this->android_amgr, fn.c_str(), AASSET_MODE_STREAMING);
		ASSERT(asset != nullptr, "File '%s' does not exist.", fn.c_str());
		int size = AAsset_getLength(asset);
		ASSERT(size > 0, "File '%s' is of size 0.", fn.c_str());
		
	#else
		std::ifstream file(fn, std::ios::binary | std::ios::ate); 
		std::streamsize size = file.tellg();
		ASSERT(file.good(), "File '%s' does not exist.", fn.c_str());
		file.seekg(0, std::ios::beg);
	#endif
	
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
		#ifdef __ANDROID__
			AAsset_read(asset, this->data[fn]->data(), size);
		#else
			file.read(this->data[fn]->data(), size);
		#endif
	}
	
	this->cv.notify_all(); //notify get/size
	
	#ifdef __ANDROID__
		AAsset_close(asset);
	#else
		ASSERT(file.good(), "Error loading file '%s'", fn.c_str());
		file.close();
	#endif
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
	#ifdef __ANDROID__
		this->android_jnienv->DeleteGlobalRef(this->glob_android_amgr);
	#endif
}
