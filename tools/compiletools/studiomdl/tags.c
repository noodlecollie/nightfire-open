#include <stdlib.h>
#include "tags.h"
#include "cmdlib.h"

struct tagitem_s
{
	char* name;
	struct tagitem_s* next;
};

tagitem_t* Tag_Create(const char* tagName)
{
	if ( !tagName || !(*tagName) )
	{
		return NULL;
	}

	char* nameCopy = copystring(tagName);

	if ( !nameCopy )
	{
		return NULL;
	}

	tagitem_t* chain = (tagitem_t*)calloc(1, sizeof(tagitem_t));
	chain->name = nameCopy;

	return chain;
}

void Tag_DeleteChain(tagitem_t* chain)
{
	while ( chain )
	{
		tagitem_t* next = chain->next;

		if ( chain->name )
		{
			free(chain->name);
		}

		free(chain);
		chain = next;
	}
}

tagitem_t* Tag_AppendToChain(tagitem_t* chain, const char* tagName)
{
	tagitem_t* next = Tag_Create(tagName);

	if ( !next )
	{
		return NULL;
	}

	if ( chain )
	{
		chain->next = next;
	}

	return next;
}

qboolean Tag_ExistsInChain(const tagitem_t* chain, const char* tagName)
{
	while ( chain )
	{
		if ( strcmp(chain->name, tagName) == 0 )
		{
			return true;
		}

		chain = chain->next;
	}

	return false;
}

const char* Tag_GetName(const tagitem_t* item)
{
	return item ? item->name : NULL;
}
