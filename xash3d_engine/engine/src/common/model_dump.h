/*
model_dump.h - For dumping data about models to the console.
Copyright (C) 2017 NoodleCollie

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef MODEL_DUMP_H
#define MODEL_DUMP_H

#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/com_model.h"
#include "common/common.h"

typedef enum
{
	Flag_ModelGeneral = (1 << 0),
	Flag_ModelPlanes = (1 << 1),
	Flag_ModelNodes = (1 << 2),
	Flag_ModelLeaves = (1 << 3),
	Flag_ModelVertices = (1 << 4),
	Flag_ModelEdges = (1 << 5),
	Flag_ModelSurfEdges = (1 << 6),
	Flag_ModelMarkSurfaces = (1 << 7),
	Flag_ModelTexInfos = (1 << 8),
	Flag_ModelSurfaces = (1 << 9),
	Flag_ModelClipNodes = (1 << 10),
	Flag_ModelHulls = (1 << 11),
	Flag_ModelTextures = (1 << 12),
	Flag_ModelEntities = (1 << 13),
	Flag_ModelSubModels = (1 << 14),
	Flag_ModelOther = (1 << 15),

	DumpModelDataFlagCount = 16,
	DumpModelAllDataFlags = ~0
} DumpModelDataFlag_t;

const char* DumpModelDataFlagString(size_t index);

// Not re-entrant!!
void DumpModelData(const char* fileName, const model_t* model, unsigned int flags);
void DumpFaceAsWavefrontObj(const char* fileName, const model_t* model, const msurface_t* surface);
void DumpLeafBoundsAsWavefrontObj(const char* fileName, const model_t* model, const mleaf_t* leaf, qboolean nodeBounds);

#endif  // MODEL_DUMP_H
