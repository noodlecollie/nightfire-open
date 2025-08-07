/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#pragma once

#include "CompileTools/cmdlib.h"
#include "MathLib/vec2.h"
#include "MathLib/vec3.h"

typedef struct
{
	vec3_t point;
	vec2_t coord;
} trivert_t;

typedef struct
{
	trivert_t verts[3];
} poly_t;

typedef struct
{
	char name[100];
	char materialname[100];
	poly_t* triangles;
	int numtriangles;
} polyset_t;

bool ASE_Load(const char* filename, bool meshanims);
int ASE_GetNumSurfaces(void);
polyset_t* ASE_GetSurfaceAnimation(int ndx, int* numFrames, int skipFrameStart, int skipFrameEnd, int maxFrames);
const char* ASE_GetSurfaceName(int ndx);
void ASE_Free(void);

bool MakeBrushFor3Points(mapent_t* mapent, side_t* mainSide, short entindex, trivert_t* a, trivert_t* b, trivert_t* c);
bool MakeBrushFor4Points(
	mapent_t* mapent,
	side_t* mainSide,
	short entindex,
	trivert_t* a,
	trivert_t* b,
	trivert_t* c,
	trivert_t* d);
