#include <sage/util/EventDispatcher.h>
#include <sage/view/Node.h>
#include <sage/util/config.h>
#include <cstdlib>

using namespace sage;

EventDispatcher::EventDispatcher(){}

void EventDispatcher::handleEvents(float delta){
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
				this->dispatchEvent(Event::MOUSE_MOVE, static_cast<int>(sdlEvent.motion.x), static_cast<int>(-sdlEvent.motion.y),
					static_cast<int>(sdlEvent.motion.xrel), static_cast<int>(-sdlEvent.motion.yrel));
				break;
			case SDL_MOUSEBUTTONDOWN:
				this->mouse_click_x = sdlEvent.button.x;
				this->mouse_click_y = sdlEvent.button.y;
				
				this->dispatchEvent(Event::MOUSE_DOWN, static_cast<int>(sdlEvent.button.button), 
					static_cast<int>(sdlEvent.button.x), static_cast<int>(sdlEvent.button.y));
				break;
			case SDL_MOUSEBUTTONUP:
				this->dispatchEvent(Event::MOUSE_UP, static_cast<int>(sdlEvent.button.button), 
					static_cast<int>(sdlEvent.button.x), static_cast<int>(sdlEvent.button.y));
					
				if(abs(sdlEvent.button.x - this->mouse_click_x) <= MOUSE_CLICK_MAX_SHIFT &&
					abs(sdlEvent.button.y - this->mouse_click_y) <= MOUSE_CLICK_MAX_SHIFT){
					this->dispatchEvent(Event::MOUSE_CLICK, static_cast<int>(sdlEvent.button.button), static_cast<int>(sdlEvent.button.clicks),
						static_cast<int>(sdlEvent.button.x), static_cast<int>(sdlEvent.button.y));
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

EventDispatcher::~EventDispatcher(){}
