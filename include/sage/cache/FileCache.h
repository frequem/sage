#ifndef _SAGE_FILECACHE_H
#define _SAGE_FILECACHE_H

#include <sage/util/ThreadManager.h>

#include <string>
#include <map>
#include <optional>
#include <vector>
#include <mutex>
#include <condition_variable>

#ifdef __ANDROID__
	#include <SDL2/SDL.h> //SDL_AndroidGetJNIEnv()
	#include <jni.h>
	#include <android/asset_manager.h>
	#include <android/asset_manager_jni.h>
#endif

namespace sage{
	class FileCache{
	public:
		FileCache(ThreadManager*);
		
		void load(const std::string&);
		void unload(const std::string&);
		const char* get(const std::string&);
		int size(const std::string&);
		~FileCache();
	private:
		void load_func(const std::string&);
		
		#ifdef __ANDROID__
			JNIEnv* android_jnienv;
			jobject glob_android_amgr;
			AAssetManager* android_amgr;
		#endif
		ThreadManager* threadManager;
		
		std::mutex mtx;
		std::condition_variable cv;
		std::map<const std::string, std::optional<std::vector<char>>> data;
		
	};
}

#endif // _SAGE_FILECACHE_H
