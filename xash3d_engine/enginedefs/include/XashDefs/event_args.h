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

struct edict_s;

// Event was invoked with stated origin
#define FEVENT_ORIGIN (1 << 0)

// Event was invoked with stated angles
#define FEVENT_ANGLES (1 << 1)

typedef struct event_args_s
{
	int flags;

	// Transmitted
	int entindex;

	float origin[3];
	float angles[3];
	float velocity[3];

	int ducking;

	float fparam1;
	float fparam2;

	int iparam1;
	int iparam2;

	int bparam1;
	int bparam2;

	float vecparam1[3];

	void* localUserData;  // Static data passed in on the client.
} event_args_t;

typedef struct event_fire_args_s
{
	int flags;
	const struct edict_s* invoker;
	unsigned short eventIndex;
	float delay;
	const float* vec3Origin;
	const float* vec3Angles;
	float fparam1;
	float fparam2;
	int iparam1;
	int iparam2;
	int bparam1;
	int bparam2;
	const float* vec3param1;
} event_fire_args_t;
