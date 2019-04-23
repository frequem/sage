#ifndef _SAGE_EVENTDISPATCHER_TPP
#define _SAGE_EVENTDISPATCHER_TPP

namespace sage{	
	template<typename... Args>
	void EventDispatcher::registerEvent(Event e, std::function<void(Args...)>&& f){
		this->handlers[e].push_back(f);
	}
	
	template<typename... Args>
	void EventDispatcher::registerNodeEvent(NodeEvent e, Node* n, std::function<void(Args...)>&& f){
		this->node_handlers[e].push_back(std::make_pair(n, f));
	}
	
	
	template<typename... Args>
	void EventDispatcher::dispatchEvent(Event e, Args... a){
		for(auto h : this->handlers[e])
			std::any_cast<std::function<void(Args...)>>(h)(a...);
	}
}

#endif // _SAGE_EVENTDISPATCHER_TPP

