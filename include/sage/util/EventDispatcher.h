#ifndef _SAGE_EVENTDISPATCHER_H
#define _SAGE_EVENTDISPATCHER_H

#include <SDL2/SDL.h>

#include <map>
#include <any>
#include <utility>
#include <functional>

namespace sage{
	class Application;
	class Node;
	//TODO: KEY_PRESS (delay)
	enum class Event{ WINDOW_LEAVE, WINDOW_ENTER, KEY_DOWN, KEY_UP, MOUSE_MOVE, MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, MOUSE_SCROLL, QUIT };
	enum class NodeEvent{ MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, MOUSE_MOVE, MOUSE_ENTER, MOUSE_LEAVE };
	
	class EventDispatcher{
	public:
		EventDispatcher(Application*);
		
		
		template<typename... Args>
		int addEventHandler(Event, std::function<void(Args...)>&&);
		
		template<typename... Args>
		int addEventHandler(NodeEvent, Node*, std::function<void(Args...)>&&);
		
		void removeEventHandler(int);
						
		void handleEvents();
		
		template<typename... Args>
		void dispatchEvent(Event, Args&&...);
		
		template<typename... Args>
		void dispatchEvent(NodeEvent, std::function<bool(Node*, Args...)>&&, Args&&...);
		
		~EventDispatcher();
	private:
		Application* application;
		
		int nextHandlerId = 0;
		std::map<Event, std::map<int, std::any>> handlers;
		std::map<NodeEvent, std::map<int, std::pair<Node*, std::any>>> node_handlers;
		
		SDL_Event sdlEvent;
		
		int mouse_click_x = 0, mouse_click_y = 0;
		int mouse_click_count = 0;
		uint32_t mouse_click_time = 0;
		
		bool isInsideClickRadius(int, int);
	};
}

#include <sage/util/EventDispatcher.tpp>

#endif //_SAGE_EVENTDISPATCHER_H
