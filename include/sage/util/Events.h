#ifndef _SAGE_EVENTS_H
#define _SAGE_EVENTS_H

#include <SDL2/SDL.h>

namespace sage{
	//TODO: KEY_PRESS (delay)
	enum class Event{ WINDOW_LEAVE, WINDOW_ENTER, KEY_DOWN, KEY_UP, MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, MOUSE_MOVE, MOUSE_DRAG, MOUSE_SCROLL, QUIT };
	
	struct EventArgs_WINDOW_LEAVE {};
	struct EventArgs_WINDOW_ENTER {};
	struct EventArgs_KEY_DOWN { SDL_Keysym keysym; int repeat; };
	struct EventArgs_KEY_UP { SDL_Keysym keysym; };
	struct EventArgs_MOUSE_DOWN { int x; int y; int button; };
	struct EventArgs_MOUSE_UP { int x; int y; int button; };
	struct EventArgs_MOUSE_CLICK { int x; int y; int button; int clicks; };
	struct EventArgs_MOUSE_MOVE { int x; int y; int xrel; int yrel; };
	struct EventArgs_MOUSE_DRAG { int x; int y; int xrel; int yrel; int button; };
	struct EventArgs_MOUSE_SCROLL { int x; int y; };
	struct EventArgs_QUIT {};
	
	enum class NodeEvent{ MOUSE_DOWN, MOUSE_UP, MOUSE_CLICK, MOUSE_MOVE, MOUSE_ENTER, MOUSE_LEAVE };
	
	struct NodeEventArgs_MOUSE_DOWN { int x; int y; int button; };
	struct NodeEventArgs_MOUSE_UP { int x; int y; int button; };
	struct NodeEventArgs_MOUSE_CLICK { int x; int y; int button; int clicks; };
	struct NodeEventArgs_MOUSE_MOVE { int x; int y; int xrel; int yrel; };
	struct NodeEventArgs_MOUSE_ENTER { int x; int y; int xrel; int yrel; };
	struct NodeEventArgs_MOUSE_LEAVE { int x; int y; int xrel; int yrel; };
}

#endif //_SAGE_EVENTS_H
