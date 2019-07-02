#ifndef _SAGE_CONFIG_H
#define _SAGE_CONFIG_H

#define FPS 60

//DepthPeelRenderer
#define DEPTH_PEELING_PASSES 2

//ShaderCache
#define SHADER_VERTEX_POSTFIX "_vertex.glsl"
#define SHADER_FRAGMENT_POSTFIX "_fragment.glsl"

//ThreadManager
#define MAX_THREAD_COUNT 100

//EventDispatcher
//maximum distance from mouse_down to mouse_up where a click is still registered, unweighted, must be multiplied by dpi
#ifndef __ANDROID__
	#define MOUSE_CLICK_MAX_SHIFT 0.04 
#else
	#define MOUSE_CLICK_MAX_SHIFT 0.04*2.5
#endif
#define MOUSE_CLICK_MAX_TIME 500 //in milliseconds

#endif // _SAGE_CONFIG_H
