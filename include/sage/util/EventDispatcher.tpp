#ifndef _SAGE_EVENTDISPATCHER_TPP
#define _SAGE_EVENTDISPATCHER_TPP

#include <sage/util/macros.h>
namespace sage{		
	template<typename... Args>
	int EventDispatcher::addEventHandler(Event e, Scene& s, std::function<void(Args...)>&& f){
		this->handlers[e][nextHandlerId] = std::make_pair(&s, f);
		return nextHandlerId++;
	}
	
	template<typename... Args>
	int EventDispatcher::addEventHandler(Event e, std::function<void(Args...)>&& f){
		this->handlers[e][nextHandlerId] = std::make_pair(nullptr, f);
		return nextHandlerId++;
	}
	
	template<typename... Args>
	int EventDispatcher::addEventHandler(NodeEvent e, Node& n, std::function<void(Args...)>&& f){
		this->node_handlers[e][nextHandlerId] = std::make_pair(&n, f);
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

	template<typename... Args>
	void EventDispatcher::dispatchEvent(Event e, Args&&... a){
		Scene* activeScene = this->getActiveScene();
		std::vector<int> ids = get_keys(this->handlers[e]);
		
		std::map<int, std::pair<Scene*, std::any>>::iterator it;
		for(int id : ids){
			it = this->handlers[e].find(id);
			if(it == this->handlers[e].end())
				continue;
			
			Scene* s = it->second.first; // pair.scene
			if(s==activeScene || s==nullptr){
				std::any_cast<std::function<void(Args...)>>(it->second.second)(a...);
			}
		}
	}
	
	template<typename... Args>
	void EventDispatcher::dispatchEvent(NodeEvent e, std::function<bool(Node&, Args...)>&& checkfunc, Args&&... a){
		Scene* activeScene = this->getActiveScene();
		std::vector<int> ids = get_keys(this->node_handlers[e]);
		
		std::map<int, std::pair<Node*, std::any>>::iterator it;
		for(int id : ids){
			it = this->node_handlers[e].find(id);
			if(it == this->node_handlers[e].end())
				continue;
			
			Node* n = it->second.first; // = pair.node
			if(n == nullptr){
				this->node_handlers[e].erase(it);
				continue;
			}else if(this->isSceneMember(activeScene, n) && checkfunc(*n, a...)){
				std::any_cast<std::function<void(Args...)>>(it->second.second)(a...);
			}
		}
	}
}

#endif // _SAGE_EVENTDISPATCHER_TPP

