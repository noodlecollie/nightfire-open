#include "bonetags.h"
#include "cmdlib.h"

typedef struct bonetaglist_s
{
	char* boneName;
	bonetag_t* tags;
} bonetaglist_t;

typedef struct bonetagtable_s
{
	bonetaglist_t** tagLists;
	size_t numLists;
} bonetagtable_t;

bonetagtable_t g_Table;

static void FreeBoneTag(bonetag_t* tag)
{
	if ( !tag )
	{
		return;
	}

	if ( tag->name )
	{
		free(tag->name);
	}

	free(tag);
}

static void FreeBoneTagChain(bonetag_t* tag)
{
	while ( tag )
	{
		bonetag_t* next = tag->next;
		FreeBoneTag(tag);
		tag = next;
	}
}

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

	FreeBoneTagChain(list->tags);
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

static bonetag_t* FindLastTagInChain(bonetag_t* tag)
{
	if ( !tag )
	{
		return NULL;
	}

	while ( tag->next )
	{
		tag = tag->next;
	}

	return tag;
}

static qboolean TagExistsInChain(bonetag_t* tag, const char* tagName)
{
	if ( !tagName || !(*tagName) )
	{
		return false;
	}

	while ( tag )
	{
		if ( strcmp(tag->name, tagName) == 0 )
		{
			return true;
		}

		tag = tag->next;
	}

	return false;
}

static void AddTagToList(bonetaglist_t* list, const char* tagName)
{
	if ( !list || !tagName || !(*tagName) )
	{
		return;
	}

	bonetag_t* tag = FindLastTagInChain(list->tags);

	if ( tag )
	{
		bonetag_t* next = (bonetag_t*)calloc(1, sizeof(bonetag_t));
		tag->next = next;
	}
	else
	{
		list->tags = (bonetag_t*)calloc(1, sizeof(bonetag_t));
		tag = list->tags;
	}

	tag->name = copystring(tagName);
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

	if ( TagExistsInChain(list->tags, tagName) )
	{
		// No duplicates allowed.
		return;
	}

	AddTagToList(list, tagName);
}

const bonetag_t* GetBoneTags(const char* boneName)
{
	if ( !boneName || !(*boneName) )
	{
		return NULL;
	}

	bonetaglist_t* list = FindBoneTagListByBoneName(boneName);
	return list ? list->tags : NULL;
}
