#pragma once

#include "BuildPlatform/Typedefs.h"

typedef struct dllfunc_s
{
	const char* name;
	void** func;
} dllfunc_t;

typedef struct dll_info_s
{
	const char* name;  // name of library
	const dllfunc_t* fcts;  // list of dll exports
	qboolean crash;  // crash if dll not found
	void* link;  // hinstance of loading library
} dll_info_t;
