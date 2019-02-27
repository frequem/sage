#ifndef _SAGE_THREADMANAGER_H
#define _SAGE_THREADMANAGER_H

#include <mutex>
#include <queue>
#include <thread>

namespace sage{	
	class ThreadManager{
	public:
		ThreadManager();
		ThreadManager(int);
		
		template <typename Callable, typename... Args>
		void run(Callable&& f, Args&&... args);
		
		~ThreadManager();
	private:
		int maxThreads = 10;
		std::queue<std::thread> threads;
	};
}

#include <sage/util/ThreadManager.tpp>

#endif // _SAGE_THREADMANAGER_H
