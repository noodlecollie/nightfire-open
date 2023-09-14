#pragma once

#pragma once

#include "EnginePublicAPI/edict.h"
#include "EnginePublicAPI/eiface.h"
#include "EngineInternalAPI/ref_api.h"
#include "EnginePublicAPI/studio.h"

#define LM_SAMPLE_SIZE 16
#define LM_SAMPLE_EXTRASIZE 8

#define MAX_MAP_WADS 256  // max wads that can be referenced per one map

#define CHECKVISBIT(vis, b) ((b) >= 0 ? (byte)((vis)[(b) >> 3] & (1 << ((b)&7))) : (byte) false)
#define SETVISBIT(vis, b) (void)((b) >= 0 ? (byte)((vis)[(b) >> 3] |= (1 << ((b)&7))) : (byte) false)
#define CLEARVISBIT(vis, b) (void)((b) >= 0 ? (byte)((vis)[(b) >> 3] &= ~(1 << ((b)&7))) : (byte) false)

#define REFPVS_RADIUS 2.0f  // radius for rendering
#define FATPVS_RADIUS 8.0f  // FatPVS use radius smaller than the FatPHS
#define FATPHS_RADIUS 16.0f

#define WORLD_INDEX (1)  // world index is always 1
#define BOX_POINT_COUNT 8

typedef struct consistency_s
{
	const char* filename;
	int orig_index;
	int check_type;
	qboolean issound;
	int value;
	vec3_t mins;
	vec3_t maxs;
} consistency_t;

#define FCRC_SHOULD_CHECKSUM BIT(0)
#define FCRC_CHECKSUM_DONE BIT(1)

typedef struct
{
	int flags;
	CRC32_t initialCRC;
} model_info_t;

// values for model_t's needload
#define NL_UNREFERENCED 0  // this model can be freed after sequence precaching is done
#define NL_NEEDS_LOADED 1
#define NL_PRESENT 2

typedef struct hullnode_s
{
	struct hullnode_s* next;
	struct hullnode_s* prev;
} hullnode_t;

typedef struct winding_s
{
	const mplane_t* plane;
	struct winding_s* pair;
	hullnode_t chain;
	int numpoints;
	vec3_t p[4];  // variable sized
} winding_t;

typedef struct
{
	hullnode_t polys;
	uint num_polys;
} hull_model_t;

typedef struct world_static_s
{
	qboolean loading;  // true if worldmodel is loading
	int flags;  // misc flags

	// mapstats info
	char message[2048];  // just for debug
	char compiler[256];  // map compiler
	char generator[256];  // map editor

	hull_model_t* hull_models;
	int num_hull_models;

	// out pointers to light data
	color24* deluxedata;  // deluxemap data pointer
	byte* shadowdata;  // occlusion data pointer

	// visibility info
	size_t visbytes;  // cluster size
	size_t fatbytes;  // fatpvs size

	// world bounds
	vec3_t mins;  // real accuracy world bounds
	vec3_t maxs;
	vec3_t size;

	// tree visualization stuff
	int recursion_level;
	int max_recursion;
} world_static_t;

typedef struct
{
	// Points should be constructed where the min/max on X
	// alternates every other index, min/max on Y alternates
	// every 2 indices, and min/max on Z alternates every 4 indices.
	vec3_t points[BOX_POINT_COUNT];
} Mod_BoxPoints;
