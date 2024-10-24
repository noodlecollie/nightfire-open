/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#pragma once

#include "BuildPlatform/Typedefs.h"
#include "MathLib/vec3.h"

/*
==============================================================================

ALIAS MODELS

Alias models are position independent, so the cache manager can move them.
==============================================================================
*/

#define IDALIASHEADER (('O' << 24) + ('P' << 16) + ('D' << 8) + 'I')  // little-endian "IDPO"
#define ALIAS_VERSION 6

#define MAXALIASVERTS 2048
#define MAXALIASFRAMES 256
#define MAXALIASTRIS 4096
#define MAX_SKINS 32

// client-side model flags
#define ALIAS_ROCKET 0x0001  // leave a trail
#define ALIAS_GRENADE 0x0002  // leave a trail
#define ALIAS_GIB 0x0004  // leave a trail
#define ALIAS_ROTATE 0x0008  // rotate (bonus items)
#define ALIAS_TRACER 0x0010  // green split trail
#define ALIAS_ZOMGIB 0x0020  // small blood trail
#define ALIAS_TRACER2 0x0040  // orange split trail + rotate
#define ALIAS_TRACER3 0x0080  // purple trail

// must match definition in sprite.h
#ifndef SYNCTYPE_T
#define SYNCTYPE_T
typedef enum
{
	ST_SYNC = 0,
	ST_RAND
} synctype_t;
#endif

typedef enum
{
	ALIAS_SINGLE = 0,
	ALIAS_GROUP
} aliasframetype_t;

typedef enum
{
	ALIAS_SKIN_SINGLE = 0,
	ALIAS_SKIN_GROUP
} aliasskintype_t;

typedef struct
{
	byte v[3];
	byte lightnormalindex;
} trivertex_t;

typedef struct
{
	int firstpose;
	int numposes;
	trivertex_t bboxmin;
	trivertex_t bboxmax;
	float interval;
	char name[16];
} maliasframedesc_t;

typedef struct
{
	int ident;
	int version;
	vec3_t scale;
	vec3_t scale_origin;
	float boundingradius;
	vec3_t eyeposition;
	int numskins;
	int skinwidth;
	int skinheight;
	int numverts;
	int numtris;
	int numframes;
	int synctype;
	int flags;
	float size;

	int reserved[8];  // VBO offsets

	int numposes;
	int poseverts;
	trivertex_t* posedata;  // numposes * poseverts trivert_t
	int* commands;  // gl command list with embedded s/t
	unsigned short gl_texturenum[MAX_SKINS][4];
	unsigned short fb_texturenum[MAX_SKINS][4];
	unsigned short gl_reserved0[MAX_SKINS][4];  // detail tex
	unsigned short gl_reserved1[MAX_SKINS][4];  // normalmap
	unsigned short gl_reserved2[MAX_SKINS][4];  // glossmap

	maliasframedesc_t frames[1];  // variable sized
} aliashdr_t;

typedef struct
{
	int ident;
	int version;
	vec3_t scale;
	vec3_t scale_origin;
	float boundingradius;
	vec3_t eyeposition;
	int numskins;
	int skinwidth;
	int skinheight;
	int numverts;
	int numtris;
	int numframes;
	synctype_t synctype;
	int flags;
	float size;
} daliashdr_t;

typedef struct
{
	int onseam;
	int s;
	int t;
} stvert_t;

typedef struct dtriangle_s
{
	int facesfront;
	int vertindex[3];
} dtriangle_t;

#define DT_FACES_FRONT 0x0010
#define ALIAS_ONSEAM 0x0020

typedef struct
{
	trivertex_t bboxmin;  // lightnormal isn't used
	trivertex_t bboxmax;  // lightnormal isn't used
	char name[16];  // frame name from grabbing
} daliasframe_t;

typedef struct
{
	int numframes;
	trivertex_t bboxmin;  // lightnormal isn't used
	trivertex_t bboxmax;  // lightnormal isn't used
} daliasgroup_t;

typedef struct
{
	int numskins;
} daliasskingroup_t;

typedef struct
{
	float interval;
} daliasinterval_t;

typedef struct
{
	float interval;
} daliasskininterval_t;

typedef struct
{
	aliasframetype_t type;
} daliasframetype_t;

typedef struct
{
	aliasskintype_t type;
} daliasskintype_t;
