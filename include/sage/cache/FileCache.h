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
	/**
	 * @brief Class that caches files into memory, so files don't have to be loaded from disk more often than necessary.
	 */
	class FileCache{
	public:
		/**
		 * @brief FileCache constructor.
		 * @param tm the ThreadManager
		 */
		FileCache(ThreadManager* tm);
		
		/**
		 * @brief Starts a thread that loads a file from disk into memory.
		 * @param fn the path to (or filename of) the file
		 * @see unload(const std::string& fn)
		 * @see get(const std::string& fn)
		 */
		void load(const std::string& fn);
		
		/**
		 * @brief Purges a file from memory.
		 * @param fn the path to (or filename of) the file
		 * @see load(const std::string& fn)
		 */
		void unload(const std::string& fn);
		
		/**
		 * @brief Returns a pointer to the contents of a file.
		 * @param fn the path to (or filename of) the file
		 * @return a pointer to the contents of the given file.
		 * @see load(const std::string& fn)
		 * @see size(const std::string& fn)
		 */
		const char* get(const std::string& fn);
		
		/**
		 * @brief Gets the size of the contents of a file.
		 * @param fn the path to (or filename of) the file
		 * @return the size to the contents of the given file.
		 * @see get(const std::string& fn)
		 */
		int size(const std::string&);
		
		/**
		 * @brief FileCache destructor.
		 */
		~FileCache();
	private:
	
		/**
		 * @brief Loads a file from disk into memory.
		 * Called by a file loading thread.
		 * @param fn the path to (or filename of) the file
		 */
		void load_func(const std::string& fn);
		
		#ifdef __ANDROID__
			JNIEnv* android_jnienv; /**< The android JNI Environment */
			jobject glob_android_amgr; /**< The android java AssetManager */
			AAssetManager* android_amgr;/**< The android native AssetManager */
		#endif
		ThreadManager* threadManager; /**< The threadManager */
	
		std::mutex mtx; /**< Mutex to protect the data map */
		std::condition_variable cv; /**< Condition variable to protect the data map */
		std::map<const std::string, std::optional<std::vector<char>>> data; /**< The map where the file contents are stored, if a map entry exists then a file load has been issued, if the entry exists and the optional has a value, the file has successfully been loaded */
		
	};
}

#endif // _SAGE_FILECACHE_H
