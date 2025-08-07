#pragma once

#include "BuildPlatform/Typedefs.h"

struct edict_s;

typedef struct
{
	vec3_t normal;
	float dist;
} plane_t;

typedef struct
{
	qboolean allsolid;  // if true, plane is not valid
	qboolean startsolid;  // if true, the initial point was in a solid area
	qboolean inopen;
	qboolean inwater;
	float fraction;  // time completed, 1.0 = didn't hit anything
	vec3_t endpos;  // final position
	plane_t plane;  // surface normal at impact
	struct edict_s* ent;  // entity the surface is on
	int hitgroup;  // 0 == generic, non zero is specific body part
} trace_t;
