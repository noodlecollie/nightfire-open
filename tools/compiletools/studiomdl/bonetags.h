#pragma once

#include "tags.h"

#define BONE_TAG_APPLY_GAIT "applygait"

void ClearAllBoneTags(void);
void AddBoneTag(const char* boneName, const char* tagName);
const tagitem_t* GetBoneTags(const char* boneName);
qboolean HasBoneTag(const char* boneName, const char* tagName);
