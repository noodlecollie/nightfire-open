#pragma once

#include "cmdlib.h"

typedef struct bonetag_s
{
	char* name;
	struct bonetag_s* next;
} bonetag_t;

#define BONE_TAG_APPLY_GAIT "applygait"

void ClearAllBoneTags(void);
void AddBoneTag(const char* boneName, const char* tagName);
const bonetag_t* GetBoneTags(const char* boneName);
qboolean HasBoneTag(const char* boneName, const char* tagName);
