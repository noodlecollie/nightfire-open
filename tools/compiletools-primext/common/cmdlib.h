#pragma once

#include "BuildPlatform/Decorators.h"

NORETURN void COM_FatalError(const char* error, ...);
char* COM_ExpandArg(const char* path);
double I_FloatTime(void);
char* copystring(const char* str);
void freestring(char* str);
