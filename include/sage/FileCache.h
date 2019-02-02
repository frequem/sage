#ifndef _SAGE_FILECACHE_H
#define _SAGE_FILECACHE_H

#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

namespace sage{
	class FileCache{
	public:
		FileCache();
		void load(const std::string&);
		const char* get(const std::string&);
		int size(const std::string&);
		~FileCache();
	protected:
		std::mutex cacheMutex;
		std::map<const std::string, std::vector<char>> cache;
		std::map<const std::string, std::thread> threads;
	private:
		void waitfor(const std::string&);
		void load_func(const std::string&);
	};
}

#endif // _SAGE_FILECACHE_H
