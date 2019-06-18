#include <sage/util/EventDispatcher.h>
#include <sage/view/Application.h>
#include <sage/view/Node.h>
#include <sage/util/config.h>
#include <sage/util/macros.h>
#include <glm/glm.hpp>

using namespace sage;

EventDispatcher::EventDispatcher(Application& application) : application(&application){}

void EventDispatcher::removeEventHandler(int id){
	for(auto const& [event, h_map] : this->handlers){
		auto i = this->handlers[event].find(id);
		if (i != this->handlers[event].end()) {
			this->handlers[event].erase(i);
			return;
		}
	}
	for(auto const& [ne, h_map] : this->node_handlers){
		auto i = this->node_handlers[ne].find(id);
		if (i != this->node_handlers[ne].end()) {
			this->node_handlers[ne].erase(i);
			return;
		}
	}
}

void EventDispatcher::handleEvents(){
	while(SDL_PollEvent(&sdlEvent) != 0){
		switch(sdlEvent.type){
			case SDL_WINDOWEVENT:
				switch(sdlEvent.window.event){
					case SDL_WINDOWEVENT_FOCUS_LOST:
					case SDL_WINDOWEVENT_MINIMIZED:
						this->dispatchEvent(Event::WINDOW_LEAVE);
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_RESTORED:
						this->dispatchEvent(Event::WINDOW_ENTER);
						break;
				}
				break;
			case SDL_KEYDOWN:
				this->dispatchEvent(Event::KEY_DOWN, &sdlEvent.key.keysym);
				break;
			case SDL_KEYUP:
				this->dispatchEvent(Event::KEY_UP, &sdlEvent.key.keysym);
				break;
			case SDL_MOUSEMOTION:
				this->dispatchEvent(Event::MOUSE_MOVE, static_cast<int>(sdlEvent.motion.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.motion.y),
					static_cast<int>(sdlEvent.motion.xrel), static_cast<int>(-sdlEvent.motion.yrel));
					
				if(this->mouse_down){
					this->dispatchEvent(Event::MOUSE_DRAG, static_cast<int>(sdlEvent.motion.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.motion.y),
					static_cast<int>(sdlEvent.motion.xrel), static_cast<int>(-sdlEvent.motion.yrel), static_cast<int>(this->mouse_down_button));
				}	
				
				this->dispatchEvent(NodeEvent::MOUSE_MOVE, static_cast<std::function<bool(Node&,int,int,int,int)>>([](Node& n, int x, int y, int xrel, int yrel){
						return n.containsAbs(glm::vec2(x,y));
					}), static_cast<int>(sdlEvent.motion.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.motion.y),
					static_cast<int>(sdlEvent.motion.xrel), static_cast<int>(-sdlEvent.motion.yrel));
				
				this->dispatchEvent(NodeEvent::MOUSE_ENTER, static_cast<std::function<bool(Node&,int,int,int,int)>>([](Node& n, int x, int y, int xrel, int yrel){
						return !n.containsAbs(glm::vec2(x-xrel, y-yrel)) && n.containsAbs(glm::vec2(x,y));
					}), static_cast<int>(sdlEvent.motion.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.motion.y),
					static_cast<int>(sdlEvent.motion.xrel), static_cast<int>(-sdlEvent.motion.yrel));
				
				this->dispatchEvent(NodeEvent::MOUSE_LEAVE, static_cast<std::function<bool(Node&,int,int,int,int)>>([](Node& n, int x, int y, int xrel, int yrel){
						return n.containsAbs(glm::vec2(x-xrel, y-yrel)) && !n.containsAbs(glm::vec2(x,y));
					}), static_cast<int>(sdlEvent.motion.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.motion.y),
					static_cast<int>(sdlEvent.motion.xrel), static_cast<int>(-sdlEvent.motion.yrel));
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(!isInsideClickRadius(sdlEvent.button.x, sdlEvent.button.y) || this->mouse_click_time + MOUSE_CLICK_MAX_TIME <= SDL_GetTicks()){
					this->mouse_click_count = 0;
				}
				this->mouse_click_x = sdlEvent.button.x;
				this->mouse_click_y = sdlEvent.button.y;
				this->mouse_click_time = SDL_GetTicks();
				this->mouse_down = true;
				this->mouse_down_button = sdlEvent.button.button;
				
				this->dispatchEvent(Event::MOUSE_DOWN, static_cast<int>(sdlEvent.button.x), static_cast<int>(sdlEvent.button.y),
					static_cast<int>(sdlEvent.button.button));
					
				this->dispatchEvent(NodeEvent::MOUSE_DOWN, static_cast<std::function<bool(Node&,int,int,int)>>([](Node& n, int x, int y, int button){
						return n.containsAbs(glm::vec2(x,y));
					}),
					static_cast<int>(sdlEvent.button.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y),
					static_cast<int>(sdlEvent.button.button));
				break;
			case SDL_MOUSEBUTTONUP:
				this->mouse_down = false;
				this->dispatchEvent(Event::MOUSE_UP, static_cast<int>(sdlEvent.button.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y),
					static_cast<int>(sdlEvent.button.button));
					
				this->dispatchEvent(NodeEvent::MOUSE_UP, static_cast<std::function<bool(Node&,int,int,int)>>([](Node& n, int x, int y, int button){
						return n.containsAbs(glm::vec2(x,y));
					}), static_cast<int>(sdlEvent.button.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y),
					static_cast<int>(sdlEvent.button.button));
				
				if(isInsideClickRadius(sdlEvent.button.x, sdlEvent.button.y) && this->mouse_click_time + MOUSE_CLICK_MAX_TIME > SDL_GetTicks()){
					this->mouse_click_count++;
					this->dispatchEvent(Event::MOUSE_CLICK, static_cast<int>(sdlEvent.button.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y),
						static_cast<int>(sdlEvent.button.button), static_cast<int>(this->mouse_click_count));
						
					this->dispatchEvent(NodeEvent::MOUSE_CLICK, static_cast<std::function<bool(Node&,int,int,int,int)>>([](Node& n, int x, int y, int button, int clicks){
							return n.containsAbs(glm::vec2(x,y));
						}), static_cast<int>(sdlEvent.button.x), static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y),
						static_cast<int>(sdlEvent.button.button),  static_cast<int>(this->mouse_click_count));
				}else{
					this->mouse_click_count = 0;
				}
				break;
			case SDL_MOUSEWHEEL:
				{
				int dir = sdlEvent.wheel.direction==SDL_MOUSEWHEEL_NORMAL?1:-1;
				this->dispatchEvent(Event::MOUSE_SCROLL, static_cast<int>(sdlEvent.wheel.x*dir), static_cast<int>(sdlEvent.wheel.y*dir));
				}	
				break;
			case SDL_QUIT:
				this->dispatchEvent(Event::QUIT);
				break;
		}
	}
}

bool EventDispatcher::isInsideClickRadius(int x, int y){
	glm::vec2 dpi = this->application->getDPI();
	return abs(x - this->mouse_click_x) <= MOUSE_CLICK_MAX_SHIFT*dpi.x &&
				abs(y - this->mouse_click_y) <= MOUSE_CLICK_MAX_SHIFT*dpi.y;
}

EventDispatcher::~EventDispatcher(){}
