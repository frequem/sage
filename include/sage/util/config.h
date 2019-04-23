#ifndef _SAGE_CONFIG_H
#define _SAGE_CONFIG_H

#define FPS 60
#define DEBUG 1

//ShaderCache config
#define SHADER_VERTEX_POSTFIX "_vertex.glsl"
#define SHADER_FRAGMENT_POSTFIX "_fragment.glsl"

//ThreadManager config
#define MAX_THREAD_COUNT 100

//EventDispatcher config
#define MOUSE_CLICK_MAX_SHIFT 5 //maximum distance from mouse_down to mouse_up where a click is still registered

#endif // _SAGE_CONFIG_H
