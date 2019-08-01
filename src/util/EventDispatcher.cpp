#include <sage/util/EventDispatcher.h>
#include <sage/view/Application.h>
#include <sage/view/Scene.h>
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


void EventDispatcher::dispatchEvent(Event e, void* args){
	Scene* activeScene = this->getActiveScene();
	std::vector<int> ids = get_keys(this->handlers[e]);
	
	std::map<int, std::pair<Scene*, std::function<void(void*)>>>::iterator it;
	for(int id : ids){
		it = this->handlers[e].find(id);
		if(it == this->handlers[e].end())
			continue;
		
		Scene* s = it->second.first; // pair.scene
		if(s==activeScene || s==nullptr){
			it->second.second(args);
		}
	}
}

void EventDispatcher::handleEvents(){
	EventArgs_WINDOW_LEAVE empty = {}; // used for WINDOW_LEAVE, WINDOW_ENTER, QUIT
	while(SDL_PollEvent(&sdlEvent) != 0){
		switch(sdlEvent.type){
			case SDL_WINDOWEVENT:
				switch(sdlEvent.window.event){
					case SDL_WINDOWEVENT_FOCUS_LOST:
					case SDL_WINDOWEVENT_MINIMIZED:
						this->dispatchEvent(Event::WINDOW_LEAVE, (void*)(&empty));
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_RESTORED:
						this->dispatchEvent(Event::WINDOW_ENTER, (void*)(&empty));
						break;
				}
				break;
			case SDL_KEYDOWN: {
				EventArgs_KEY_DOWN args = { sdlEvent.key.keysym, sdlEvent.key.repeat };
				this->dispatchEvent(Event::KEY_DOWN, (void*)(&args));
			} break;
			case SDL_KEYUP: {
				EventArgs_KEY_UP args = { sdlEvent.key.keysym };
				this->dispatchEvent(Event::KEY_UP, (void*)(&args));
			} break;
			case SDL_MOUSEMOTION: {
				EventArgs_MOUSE_DRAG args = { //used for MOUSE_MOVE (E+N), MOUSE_DRAG, MOUSE_ENTER, MOUSE_LEAVE
					sdlEvent.motion.x,
					static_cast<int>(this->application->getWindowHeight()-sdlEvent.motion.y),
					sdlEvent.motion.xrel,
					-sdlEvent.motion.yrel,
					this->mouse_down_button
				};
				this->dispatchEvent(Event::MOUSE_MOVE, (void*)(&args));
					
				if(this->mouse_down){
					this->dispatchEvent(Event::MOUSE_DRAG, (void*)(&args));
				}	
				
				this->dispatchEvent(NodeEvent::MOUSE_MOVE, [](Node& n, void* argptr){
						NodeEventArgs_MOUSE_MOVE args = *((NodeEventArgs_MOUSE_MOVE*)argptr);
						return n.containsAbs(glm::vec2(args.x, args.y));
					}, (void*)(&args));
				
				this->dispatchEvent(NodeEvent::MOUSE_ENTER, [](Node& n, void* argptr){
						NodeEventArgs_MOUSE_ENTER args = *((NodeEventArgs_MOUSE_ENTER*)argptr);
						return !n.containsAbs(glm::vec2(args.x-args.xrel, args.y-args.yrel)) && n.containsAbs(glm::vec2(args.x,args.y));
					}, (void*)(&args));
				
				this->dispatchEvent(NodeEvent::MOUSE_LEAVE, [](Node& n, void* argptr){
						NodeEventArgs_MOUSE_LEAVE args = *((NodeEventArgs_MOUSE_LEAVE*)argptr);
						return n.containsAbs(glm::vec2(args.x-args.xrel, args.y-args.yrel)) && !n.containsAbs(glm::vec2(args.x,args.y));
					}, (void*)(&args));
				} break;
			case SDL_MOUSEBUTTONDOWN: {
				if(!isInsideClickRadius(sdlEvent.button.x, sdlEvent.button.y) || this->mouse_click_time + MOUSE_CLICK_MAX_TIME <= SDL_GetTicks()){
					this->mouse_click_count = 0;
				}
				this->mouse_click_x = sdlEvent.button.x;
				this->mouse_click_y = sdlEvent.button.y;
				this->mouse_click_time = SDL_GetTicks();
				this->mouse_down = true;
				this->mouse_down_button = sdlEvent.button.button;
				
				EventArgs_MOUSE_DOWN args = { //used for Event + NodeEvent :: MOUSE_DOWN
					sdlEvent.button.x, 
					static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y), 
					sdlEvent.button.button
				};
				
				this->dispatchEvent(Event::MOUSE_DOWN, (void*)(&args));
				this->dispatchEvent(NodeEvent::MOUSE_DOWN, [](Node& n, void* argptr){
						NodeEventArgs_MOUSE_DOWN args = *((NodeEventArgs_MOUSE_DOWN*)argptr);
						return n.containsAbs(glm::vec2(args.x, args.y));
					}, (void*)(&args));
				} break;
			case SDL_MOUSEBUTTONUP: {
				EventArgs_MOUSE_CLICK args = { //used for MOUSE_UP, MOUSE_CLICK (Event + NodeEvent)
					sdlEvent.button.x, 
					static_cast<int>(this->application->getWindowHeight()-sdlEvent.button.y), 
					sdlEvent.button.button, 
					this->mouse_click_count
				};
				
				this->mouse_down = false;
				this->dispatchEvent(Event::MOUSE_UP, (void*)(&args));
					
				this->dispatchEvent(NodeEvent::MOUSE_UP, [](Node& n, void* argptr){
						NodeEventArgs_MOUSE_UP args = *((NodeEventArgs_MOUSE_UP*)argptr);
						return n.containsAbs(glm::vec2(args.x, args.y));
					}, (void*)(&args));
				
				if(isInsideClickRadius(sdlEvent.button.x, sdlEvent.button.y) && this->mouse_click_time + MOUSE_CLICK_MAX_TIME > SDL_GetTicks()){
					this->mouse_click_count++;
					this->dispatchEvent(Event::MOUSE_CLICK, (void*)(&args));
					this->dispatchEvent(NodeEvent::MOUSE_CLICK, [](Node& n, void* argptr){
							NodeEventArgs_MOUSE_CLICK args = *((NodeEventArgs_MOUSE_CLICK*)argptr);
							return n.containsAbs(glm::vec2(args.x, args.y));
						}, (void*)(&args));
				}else{
					this->mouse_click_count = 0;
				}
			} break;
			case SDL_MOUSEWHEEL: {
				int dir = (sdlEvent.wheel.direction==SDL_MOUSEWHEEL_NORMAL)?1:-1;
				EventArgs_MOUSE_SCROLL args = {sdlEvent.wheel.x*dir, sdlEvent.wheel.y*dir};
				this->dispatchEvent(Event::MOUSE_SCROLL, (void*)(&args));
			} break;
			case SDL_QUIT:
				this->dispatchEvent(Event::QUIT, (void*)(&empty));
				break;
		}
	}
}

bool EventDispatcher::isInsideClickRadius(int x, int y){
	glm::vec2 dpi = this->application->getDPI();
	return abs(x - this->mouse_click_x) <= MOUSE_CLICK_MAX_SHIFT*dpi.x &&
				abs(y - this->mouse_click_y) <= MOUSE_CLICK_MAX_SHIFT*dpi.y;
}

bool EventDispatcher::isSceneMember(Scene* s, Node* n){
	return &(n->getScene()) == s;
}

Scene* EventDispatcher::getActiveScene(){
	return this->application->getScene().get();
}

EventDispatcher::~EventDispatcher(){}
