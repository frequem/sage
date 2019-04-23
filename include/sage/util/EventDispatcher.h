#ifndef _SAGE_EVENTDISPATCHER_H
#define _SAGE_EVENTDISPATCHER_H

#include <SDL2/SDL.h>

#include <map>
#include <vector>
#include <any>
#include <utility>
#include <functional>

namespace sage{
	class Node;
	//TODO: KEY_PRESS (delay)
	enum class Event{ WINDOW_LEAVE, WINDOW_ENTER, KEY_DOWN, KEY_UP, MOUSE_MOVE, MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, MOUSE_SCROLL, QUIT };
	enum class NodeEvent{ MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, MOUSE_ENTER, MOUSE_LEAVE };
	
	class EventDispatcher{
	public:
		EventDispatcher();
		
		template<typename... Args>
		void registerEvent(Event, std::function<void(Args...)>&&);
		
		template<typename... Args>
		void registerNodeEvent(NodeEvent, Node*, std::function<void(Args...)>&&);
		
		void handleEvents(float);
		
		template<typename... Args>
		void dispatchEvent(Event, Args...);
		
		~EventDispatcher();
	private:
		std::map<Event, std::vector<std::any>> handlers;
		std::map<NodeEvent, std::vector<std::pair<Node*, std::any>>> node_handlers;
		
		SDL_Event sdlEvent;
		
		int mouse_click_x = 0, mouse_click_y = 0;
	};
}

#include <sage/util/EventDispatcher.tpp>

#endif //_SAGE_EVENTDISPATCHER_H
