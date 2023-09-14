#include <stdint.h>
#include "common/textureproperties.h"
#include "EnginePublicAPI/surfaceprops.h"
#include "CRTLib/crtlib.h"

#define LIST_ITEM(enum, str) str,
static const char* const SurfacePropertyStrings[SurfaceProp__Count] = {SURFACEPROP_ENTRY_LIST};
#undef LIST_ITEM

// Linear, but the number of surface types is low anyway so it *shouldn't* matter.
static SurfaceProp FindMatchingSurfaceProp(const char* str)
{
	for ( uint32_t index = 0; index < SurfaceProp__Count; ++index )
	{
		if ( Q_strcmp(SurfacePropertyStrings[index], str) == 0 )
		{
			return (SurfaceProp)index;
		}
	}

	return SurfaceProp__Count;
}

static qboolean ParseSurfaceProperty(texture_t* texture, const char* value)
{
	SurfaceProp property = FindMatchingSurfaceProp(value);

	if ( property == SurfaceProp__Count )
	{
		return false;
	}

	texture->surfaceType = property;
	return true;
}

qboolean TextureProperties_Parse(texture_t* texture, const char* key, const char* value)
{
	if ( !texture || !key || !value )
	{
		return false;
	}

	if ( Q_strcmp(key, TEXTURE_PROPERTY_SURFACEPROP) == 0 )
	{
		return ParseSurfaceProperty(texture, value);
	}

	return false;
}
