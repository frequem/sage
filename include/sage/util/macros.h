#ifndef _SAGE_MACROS_H
#define _SAGE_MACROS_H

#include <sage/util/config.h>
#include <cstring>
#include <cstdlib>

#define PRNT(pre, post, format, ...) { \
	char str[strlen(pre)+strlen(format)+strlen(post)+1]; \
	strcpy(str, pre); \
	strcat(str, format); \
	strcat(str, post); \
	printf(str, __VA_ARGS__); \
}

#define LOG(...) if(DEBUG){ PRNT("DEBUG: ", "\n", __VA_ARGS__, ""); }
#define ASSERT(condition, ...) if(!(condition)){ PRNT("ERROR: ", "\nExiting...\n", __VA_ARGS__, ""); exit(EXIT_FAILURE); }

#endif // _SAGE_MACROS_H
