#ifndef _SAGE_OGL_H
#define _SAGE_OGL_H

#ifdef __ANDROID__
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <SDL2/SDL_opengles2.h>
#else
	#include <GL/glew.h>
	#include <GL/glu.h>
	#include <SDL2/SDL_opengl.h>
#endif //__ANDROID__


#endif //_SAGE_OGL_H
