#pragma once

#include "BuildPlatform/Decorators.h"

NORETURN void COM_Assert(const char* file, int line, const char* error, ...);
NORETURN void COM_FatalError(const char* error, ...);
char* COM_ExpandArg(const char* path);
double I_FloatTime(void);
char* copystring(const char* str);
void freestring(char* str);

#define ASSERT(exp) \
	do \
	{ \
		if ( !(exp) ) \
		{ \
			COM_Assert(__FILE__, __LINE__, #exp); \
		} \
	} \
	while ( 0 )
