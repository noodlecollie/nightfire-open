#pragma once

#include <stddef.h>
#include "cmdlib.h"

typedef struct tagitem_s tagitem_t;

tagitem_t* Tag_Create(const char* tagName);
void Tag_DeleteChain(tagitem_t* chain);
tagitem_t* Tag_AppendToChain(tagitem_t* chain, const char* tagName);
qboolean Tag_ExistsInChain(const tagitem_t* chain, const char* tagName);
const char* Tag_GetName(const tagitem_t* item);
size_t Tag_Count(const tagitem_t* chain);
tagitem_t* Tag_GetNext(tagitem_t* item);
const tagitem_t* Tag_ConstGetNext(const tagitem_t* item);
