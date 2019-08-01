#ifndef _SAGE_EVENTDISPATCHER_TPP
#define _SAGE_EVENTDISPATCHER_TPP

#include <sage/util/macros.h>
namespace sage{		
	template<typename Function>
	int EventDispatcher::addEventHandler(Event e, Scene& s, Function&& f){
		this->handlers[e][nextHandlerId] = std::make_pair(&s, std::forward<Function>(f));
		return nextHandlerId++;
	}
	
	template<typename Function>
	int EventDispatcher::addEventHandler(Event e, Function&& f){
		this->handlers[e][nextHandlerId] = std::make_pair(nullptr, std::forward<Function>(f));
		return nextHandlerId++;
	}
	
	template<typename Function>
	int EventDispatcher::addEventHandler(NodeEvent e, Node& n, Function&& f){
		this->node_handlers[e][nextHandlerId] = std::make_pair(&n, std::forward<Function>(f));
		return nextHandlerId++;
	}
	
	template<typename Key, typename Value>
	std::vector<Key> get_keys(std::map<Key, Value> const& map){
		std::vector<Key> keys;
		for(auto const& e : map){
			keys.push_back(e.first);
		}
		return keys;
	}
	
	template<typename CheckFunction>
	void EventDispatcher::dispatchEvent(NodeEvent e, CheckFunction&& checkfunc, void* args){
		Scene* activeScene = this->getActiveScene();
		std::vector<int> ids = get_keys(this->node_handlers[e]);
		
		std::map<int, std::pair<Node*, std::function<void(void*)>>>::iterator it;
		for(int id : ids){
			it = this->node_handlers[e].find(id);
			if(it == this->node_handlers[e].end())
				continue;
			
			Node* n = it->second.first; // = pair.node
			if(n == nullptr){
				this->node_handlers[e].erase(it);
				continue;
			}else if(this->isSceneMember(activeScene, n) && checkfunc(*n, args)){
				it->second.second(args);
			}
		}
	}
}

#endif // _SAGE_EVENTDISPATCHER_TPP

