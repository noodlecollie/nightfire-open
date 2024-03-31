#include "bonetags.h"
#include "cmdlib.h"

typedef struct bonetaglist_s
{
	char* boneName;
	tagitem_t* tags;
} bonetaglist_t;

typedef struct bonetagtable_s
{
	bonetaglist_t** tagLists;
	size_t numLists;
} bonetagtable_t;

bonetagtable_t g_Table;

static void FreeBoneTagList(bonetaglist_t* list)
{
	if ( !list )
	{
		return;
	}

	if ( list->boneName )
	{
		free(list->boneName);
	}

	Tag_DeleteChain(list->tags);
	free(list);
}

static bonetaglist_t* FindBoneTagListByBoneName(const char* boneName)
{
	if ( !boneName )
	{
		return NULL;
	}

	for ( size_t index = 0; index < g_Table.numLists; ++index )
	{
		bonetaglist_t* list = g_Table.tagLists[index];

		if ( strcmp(list->boneName, boneName) == 0 )
		{
			return list;
		}
	}

	return NULL;
}

static bonetaglist_t* CreateBoneTagList(const char* boneName)
{
	if ( !boneName || !(*boneName) )
	{
		return NULL;
	}

	g_Table.tagLists = realloc(g_Table.tagLists, (++g_Table.numLists) * sizeof(bonetaglist_t*));
	bonetaglist_t** list = &g_Table.tagLists[g_Table.numLists - 1];

	*list = (bonetaglist_t*)calloc(1, sizeof(bonetaglist_t));
	(*list)->boneName = copystring(boneName);

	return *list;
}

static void AddTagToList(bonetaglist_t* list, const char* tagName)
{
	if ( !list || !tagName || !(*tagName) )
	{
		return;
	}

	tagitem_t* item = Tag_AppendToChain(list->tags, tagName);

	if ( !list->tags )
	{
		list->tags = item;
	}
}

void ClearAllBoneTags(void)
{
	if ( !g_Table.tagLists )
	{
		return;
	}

	for ( size_t index = 0; index < g_Table.numLists; ++index )
	{
		FreeBoneTagList(g_Table.tagLists[index]);
	}

	free(g_Table.tagLists);
	g_Table.numLists = 0;
}

void AddBoneTag(const char* boneName, const char* tagName)
{
	if ( !boneName || !(*boneName) || !tagName || !(*tagName) )
	{
		return;
	}

	bonetaglist_t* list = FindBoneTagListByBoneName(boneName);

	if ( !list )
	{
		list = CreateBoneTagList(boneName);
	}

	if ( Tag_ExistsInChain(list->tags, tagName) )
	{
		// No duplicates allowed.
		return;
	}

	AddTagToList(list, tagName);
}

const tagitem_t* GetBoneTags(const char* boneName)
{
	if ( !boneName || !(*boneName) )
	{
		return NULL;
	}

	bonetaglist_t* list = FindBoneTagListByBoneName(boneName);
	return list ? list->tags : NULL;
}

qboolean HasBoneTag(const char* boneName, const char* tagName)
{
	if ( !boneName || !(*boneName) || !tagName || !(*tagName) )
	{
		return false;
	}

	bonetaglist_t* list = FindBoneTagListByBoneName(boneName);

	if ( !list )
	{
		return false;
	}

	return Tag_ExistsInChain(list->tags, tagName);
}
