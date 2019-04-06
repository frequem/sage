#ifndef _SAGE_MACROS_H
#define _SAGE_MACROS_H

#include <sage/util/config.h>
#include <cstring>
#include <cstdlib>

#ifdef __ANDROID__
	#include <android/log.h>
	
	#define PRINT_MSG(type, post, format, ...) __android_log_print(ANDROID_LOG_DEBUG, "SAGE", type ": " format post "%s", __VA_ARGS__)
#else	
	#define PRINT_MSG(type, post, format, ...) printf(type ": " format post "%s\n", __VA_ARGS__)
#endif

#define LOG(...) if(DEBUG){ PRINT_MSG("DEBUG", "", __VA_ARGS__, ""); }
#define ASSERT(condition, ...) if(!(condition)){ PRINT_MSG("ERROR", " Exiting...", __VA_ARGS__, ""); exit(EXIT_FAILURE); }

#endif // _SAGE_MACROS_H
