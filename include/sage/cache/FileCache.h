#ifndef _SAGE_FILECACHE_H
#define _SAGE_FILECACHE_H

#include <sage/util/ThreadManager.h>

#include <string>
#include <map>
#include <optional>
#include <vector>
#include <mutex>
#include <condition_variable>

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
		
		ThreadManager* threadManager;
		
		std::mutex mtx;
		std::condition_variable cv;
		std::map<const std::string, std::optional<std::vector<char>>> data;
		
	};
}

#endif // _SAGE_FILECACHE_H
