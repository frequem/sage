#include <sage/util/ThreadManager.h>
#include <sage/util/macros.h>

using namespace sage;

ThreadManager::ThreadManager() : ThreadManager(MAX_THREAD_COUNT){
}

ThreadManager::ThreadManager(int maxThreads){
	this->maxThreads = maxThreads;
}

ThreadManager::~ThreadManager(){
	LOG("sage::ThreadManager Destructor");
	while(!this->threads.empty()){
		if(this->threads.front().joinable())
			this->threads.front().join();
		this->threads.pop();
	}
}
