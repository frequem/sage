#ifndef _SAGE_EVENTDISPATCHER_TPP
#define _SAGE_EVENTDISPATCHER_TPP

#include <sage/util/macros.h>
namespace sage{	
	template<typename... Args>
	int EventDispatcher::addEventHandler(Event e, std::function<void(Args...)>&& f){
		this->handlers[e][nextHandlerId] = f;
		return nextHandlerId++;
	}
	
	template<typename... Args>
	int EventDispatcher::addEventHandler(NodeEvent e, Node& n, std::function<void(Args...)>&& f){
		this->node_handlers[e][nextHandlerId] = std::make_pair(&n, f);
		return nextHandlerId++;
	}

	template<typename... Args>
	void EventDispatcher::dispatchEvent(Event e, Args&&... a){
		for(auto const& [key, h] : this->handlers[e]){
			std::any_cast<std::function<void(Args...)>>(h)(a...);
		}
	}
	
	
	template<typename... Args>
	void EventDispatcher::dispatchEvent(NodeEvent e, std::function<bool(Node&, Args...)>&& checkfunc, Args&&... a){
		for(auto i = this->node_handlers[e].begin(); i != this->node_handlers[e].end();){
			Node* n = i->second.first; // = pair.node
			if(n == nullptr){
				i = this->node_handlers[e].erase(i);
				continue;
			}else if(checkfunc(*n, a...)){
				std::any_cast<std::function<void(Args...)>>(i->second.second)(a...);
			}
			i++;
		}
	}
}

#endif // _SAGE_EVENTDISPATCHER_TPP

