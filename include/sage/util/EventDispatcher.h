#ifndef _SAGE_EVENTDISPATCHER_H
#define _SAGE_EVENTDISPATCHER_H

#include <SDL2/SDL.h>

#include <map>
#include <utility>
#include <functional>

#include <sage/util/Events.h>

namespace sage{
	class Application;
	class Scene;
	class Node;
	
	class EventDispatcher{
	public:
		EventDispatcher(Application&);
		
		template<typename Function>
		int addEventHandler(Event, Scene&, Function&&);
		
		template<typename Function>
		int addEventHandler(Event, Function&&);
		
		template<typename Function>
		int addEventHandler(NodeEvent, Node&, Function&&);
		
		void removeEventHandler(int);
						
		void handleEvents();
		
		void dispatchEvent(Event, void* args);
		
		template<typename CheckFunction>
		void dispatchEvent(NodeEvent, CheckFunction&&, void* args);
		
		~EventDispatcher();
	private:
		Application* application;
		
		int nextHandlerId = 0;
		std::map<Event, std::map<int, std::pair<Scene*, std::function<void(void*)>>>> handlers;
		std::map<NodeEvent, std::map<int, std::pair<Node*, std::function<void(void*)>>>> node_handlers;
		
		SDL_Event sdlEvent;
		
		int mouse_click_x = 0, mouse_click_y = 0;
		int mouse_click_count = 0;
		uint32_t mouse_click_time = 0;
		bool mouse_down = false;
		int mouse_down_button;
		
		bool isInsideClickRadius(int, int);
		bool isSceneMember(Scene*, Node*);
		Scene* getActiveScene();
	};
}

#include <sage/util/EventDispatcher.tpp>

#endif //_SAGE_EVENTDISPATCHER_H
