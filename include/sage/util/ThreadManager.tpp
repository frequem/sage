#ifndef _SAGE_THREADMANAGER_TPP
#define _SAGE_THREADMANAGER_TPP

#include <sage/util/config.h>

namespace sage{	
	template <typename Callable, typename... Args>
	void ThreadManager::run(Callable&& f, Args&&... args){
		if(this->threads.size() >= this->maxThreads){
			if(this->threads.front().joinable())
				this->threads.front().join();
			this->threads.pop();
		}
		
		this->threads.push(std::thread(f, args...));
	}
}

#endif // _SAGE_THREADMANAGER_TPP

