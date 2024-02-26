#pragma once

typedef struct bonetag_s
{
	char* name;
	struct bonetag_s* next;
} bonetag_t;

void ClearAllBoneTags(void);
void AddBoneTag(const char* boneName, const char* tagName);
const bonetag_t* GetBoneTags(const char* boneName);
