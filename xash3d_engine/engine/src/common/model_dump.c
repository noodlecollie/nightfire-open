/*
model_dump.c - For dumping data about models to the console.
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

#include <stdint.h>
#include "common/model_dump.h"
#include "common/common.h"
#include "bsp/generic/viscompress.h"
#include "MathLib/vec3.h"
#include "EnginePublicAPI/eiface.h"
#include "common/fscallback.h"
#include "common/engine_mempool.h"

// For utility:
#define MAX_INDENT 31
static char currentIndent[MAX_INDENT + 1];
static size_t currentIndentSize = 0;

#define ARRCOUNT(_arr, _type) (sizeof(_arr) / sizeof(_type))
#define WRITEL(_msg) FS_Printf(outFile, "%s" _msg "\n", currentIndent)
#define WRITELF(_msg, ...) FS_Printf(outFile, "%s" _msg "\n", currentIndent, __VA_ARGS__)
#define CHECK_FLAG(_val, _flag, _func) \
	{ \
		if ( (_val) & (_flag) ) \
		{ \
			_func; \
		} \
	}

#define FMT_VEC3 "(%f %f %f)"
#define ARG_VEC3(_vec) (_vec)[0], (_vec)[1], (_vec)[2]

#define FMT_VEC4 "(%f %f %f %f)"
#define ARG_VEC4(_vec) (_vec)[0], (_vec)[1], (_vec)[2], (_vec)[3]

#define FMT_VEC2_SHORT "(%d %d)"
#define ARG_VEC2_SHORT(_vec) (_vec)[0], (_vec)[1]

#define FMT_PLANE "(%f %f %f %f)"
#define ARG_PLANE(_plane) (_plane)->normal[0], (_plane)->normal[1], (_plane)->normal[2], (_plane)->dist

#define FMT_ENUM "%s (%d)"
#define ARG_ENUM(_strings, _index) \
	(_index) >= 0 && (size_t)(_index) < (ARRCOUNT(_strings, char*)) ? (_strings)[(_index)] : "INVALID INDEX", (_index)

#define FMT_BOUNDS FMT_VEC3 " - " FMT_VEC3
#define ARG_BOUNDS(_min, _max) ARG_VEC3(_min), ARG_VEC3(_max)
#define ARG_BOUNDS_CONSEC(_minMax) ARG_BOUNDS(&(_minMax)[0], &(_minMax)[3])

#define FMT_POINTER_INDEX "[%zu] {0x%p}"
#define ARG_POINTER_INDEX(_item, _base, _typeSize) IndexOfPointer((_item), (_base), (_typeSize)), (_item)

#define FMT_HEX_INT "0x%08X"

static const char* const String_modtype_t[] = {"Invalid", "Brush", "Sprite", "Alias", "Studio"};

static const char* const String_planeType[] = {
	"Normal along X",
	"Normal along Y",
	"Normal along Z",
	"Normal closest to X",
	"Normal closest to Y",
	"Normal closest to Z"};

static const char* const String_surfaceFlags[] = {
	"NoCull",      "PlaneBack", "DrawSky",   "WaterCSG",  "DrawTurb",  "DrawTiled", "Conveyor",  "Underwater",
	"Transparent", "Unknown9",  "Unknown10", "Unknown11", "Unknown12", "Unknown13", "Unknown14", "Unknown15",
	"Unknown16",   "Unknown17", "Unknown18", "Unknown19", "Unknown20", "Unknown21", "Unknown22", "Unknown23",
	"Unknown24",   "Unknown25", "Unknown26", "Unknown27", "Unknown28", "Unknown29", "Unknown30", "Reflect",
};

typedef enum
{
	TypeIsSigned = (1 << 7),

	UnsignedChar = 0,
	SignedChar = UnsignedChar | TypeIsSigned,

	UnsignedShort = 1,
	SignedShort = UnsignedShort | TypeIsSigned,

	UnsignedInt = 2,
	SignedInt = UnsignedInt | TypeIsSigned
} IntegerType_t;

static inline size_t IntegerTypeSize(IntegerType_t type)
{
	switch ( type & (0x000000FF & ~TypeIsSigned) )
	{
		case UnsignedChar:
		{
			return sizeof(unsigned char);
		}

		case UnsignedShort:
		{
			return sizeof(unsigned short);
		}

		case UnsignedInt:
		{
			return sizeof(unsigned int);
		}

		default:
		{
			return 0;
		}
	}
}

// Shared between functions.
file_t* outFile = NULL;

static inline const char* BoolString(qboolean value)
{
	return value ? "true" : "false";
}

// itemSize is the size of the item being pointed to.
static inline size_t IndexOfPointer(const void* item, const void* base, const size_t itemSize)
{
	return (item && base && itemSize) ? (((size_t)item - (size_t)base) / itemSize) : 0;
}

static const char* ArrayString_Int(const void* base, IntegerType_t type, size_t count)
{
	static char buffer[256];
	static const size_t bufferLength = sizeof(buffer);

	size_t bufferIndex;
	size_t item;
	const unsigned char* charBase;
	size_t typeSize;
	const char* formatString;

	typeSize = IntegerTypeSize(type);

	if ( typeSize < 1 )
	{
		return NULL;
	}

	charBase = (const unsigned char*)base;
	formatString = type & TypeIsSigned ? " %d" : " %u";
	bufferIndex = 0;

	// For convenience:
#define LOCAL_PRINT(...) \
	{ \
		bufferIndex += Q_snprintf(&buffer[bufferIndex], bufferLength - bufferIndex - 1, __VA_ARGS__); \
	}

	LOCAL_PRINT("[");

	for ( item = 0; item < count && bufferIndex < bufferLength - 1; ++item )
	{
		const unsigned char* offset = charBase + (item * typeSize);

		switch ( type & (0x000000FF & ~TypeIsSigned) )
		{
			case UnsignedChar:
			{
				LOCAL_PRINT(formatString, *offset);
				break;
			}

			case UnsignedShort:
			{
				LOCAL_PRINT(formatString, *((unsigned short*)offset));
				break;
			}

			case UnsignedInt:
			{
				LOCAL_PRINT(formatString, *((unsigned int*)offset));
				break;
			}

			default:
			{
				break;
			}
		}
	}

	LOCAL_PRINT(" ]");

#undef LOCAL_PRINT

	buffer[bufferLength - 1] = '\0';
	return buffer;
}

static const char* FlagsString(unsigned int inFlags, const char* const* flagStrings, int numFlags)
{
	static char buffer[256];
	static const size_t bufferLength = sizeof(buffer);

	size_t charsWritten = 0;
	int flag;

	buffer[0] = '\0';

	for ( flag = 0; flag < numFlags; ++flag )
	{
		int flagMask = 1 << flag;

		if ( inFlags & flagMask )
		{
			if ( charsWritten > 0 )
			{
				charsWritten += Q_strncat(buffer, " | ", bufferLength - charsWritten - 1);
			}

			charsWritten += Q_strncat(buffer, flagStrings[flag], bufferLength - charsWritten - 1);
		}
	}

	buffer[bufferLength - 1] = '\0';
	return buffer;
}

inline static const char* TextureNameForSurface(const msurface_t* surface)
{
	return (surface && surface->texinfo && surface->texinfo->texture) ? surface->texinfo->texture->name : "NULL";
}

typedef void (*ItemPrintFunc)(int, const void*, const model_t*);

static inline void
PrintItems(const model_t* model, const void* items, const int count, const size_t itemSize, ItemPrintFunc printFunc)
{
	int index;

	if ( !printFunc || !items || count < 1 || itemSize < 1 )
	{
		return;
	}

	for ( index = 0; index < count; ++index )
	{
		const void* itemPtr = (const void*)((unsigned char*)items + (index * itemSize));
		WRITELF("[%d] {0x%p}", index, itemPtr);
		printFunc(index, itemPtr, model);
	}
}

static inline void ResetIndent(void)
{
	memset(currentIndent, 0, sizeof(currentIndent));
	currentIndentSize = 0;
}

static void IncrementIndent(void)
{
	if ( currentIndentSize >= MAX_INDENT )
	{
		return;
	}

	currentIndent[currentIndentSize] = ' ';
	++currentIndentSize;
}

static void DecrementIndent(void)
{
	if ( currentIndentSize == 0 )
	{
		return;
	}

	currentIndent[currentIndentSize - 1] = '\0';
	--currentIndentSize;
}

static void DumpModelItems(
	const model_t* model,
	const char* itemName,
	const void* items,
	const int count,
	const size_t itemSize,
	ItemPrintFunc printFunc)
{
	if ( !model || !printFunc || !itemName || !items || count < 1 || itemSize < 1 )
	{
		WRITELF("DumpModelItems: NULL for '%s'", itemName);
		return;
	}

	WRITELF("%d %s:", count, itemName);
	WRITEL("");

	IncrementIndent();
	PrintItems(model, items, count, itemSize, printFunc);
	DecrementIndent();
}

static void DumpGeneralModelData(const model_t* model)
{
	if ( !model )
	{
		WRITEL("DumpGeneralModelData: NULL");
		return;
	}

	WRITELF("Name: %s", model->name);
	WRITELF("Needs load: %s", BoolString(model->needload));
	WRITELF("Type: " FMT_ENUM, ARG_ENUM(String_modtype_t, model->type));
	WRITELF("Num frames: %d", model->numframes);
	WRITELF("Valid mempool: %s", BoolString((qboolean)model->mempool));
	WRITELF("Flags: " FMT_HEX_INT, model->flags);
	WRITELF("Bounds: " FMT_BOUNDS, ARG_BOUNDS(model->mins, model->maxs));
	WRITELF("Radius: %f", model->radius);
	WRITELF("First model surface: %d", model->firstmodelsurface);
	WRITELF("Num model surfaces: %d", model->nummodelsurfaces);
	WRITEL("");
}

static void DumpPlane(int index, const void* data, const model_t* model)
{
	const mplane_t* plane;

	(void)index;
	(void)model;

	if ( !data )
	{
		WRITEL("DumpPlane: NULL");
		return;
	}

	plane = (const mplane_t*)data;

	WRITELF("Normal: " FMT_VEC3, ARG_VEC3(plane->normal));
	WRITELF("Distance: %f", plane->dist);
	WRITELF("Type: " FMT_ENUM, ARG_ENUM(String_planeType, plane->type));
	WRITELF("X axis positive: %s", BoolString(plane->signbits & 0x1));
	WRITELF("Y axis positive: %s", BoolString(plane->signbits & 0x2));
	WRITELF("Z axis positive: %s", BoolString(plane->signbits & 0x4));
	WRITEL("");
}

static void DumpLeaf(int index, const void* data, const model_t* model)
{
	const mleaf_t* leaf;

	(void)index;

	if ( !data )
	{
		WRITEL("DumpLeaf: NULL");
		return;
	}

	leaf = (const mleaf_t*)data;

	WRITELF("Contents: %d", leaf->contents);
	WRITELF("Bounds: " FMT_BOUNDS, ARG_BOUNDS_CONSEC(leaf->minmaxs));
	WRITELF("Parent node: " FMT_POINTER_INDEX, ARG_POINTER_INDEX(leaf->parent, model->nodes, sizeof(mnode_t)));
	WRITELF(
		"Ambient sound level: %s",
		ArrayString_Int(leaf->ambient_sound_level, UnsignedChar, ARRCOUNT(leaf->ambient_sound_level, byte)));

	// Might want to expand pointers into actual data at some point.
	WRITELF(
		"Compressed vis offset: " FMT_POINTER_INDEX,
		ARG_POINTER_INDEX(leaf->compressed_vis, model->visdata, sizeof(byte)));

	// TODO: Output properly.
	WRITELF("Efrags: %p", leaf->efrags);

	WRITELF(
		"First marksurface: " FMT_POINTER_INDEX,
		ARG_POINTER_INDEX(leaf->firstmarksurface, model->marksurfaces, sizeof(msurface_t*)));
	WRITELF("Num marksurfaces: %d", leaf->nummarksurfaces);
	WRITEL("");
}

static void DumpVertices(const model_t* model)
{
	int vertexIndex;

	if ( !model )
	{
		WRITEL("DumpVertices: NULL");
		return;
	}

	WRITELF("%d vertices:", model->numvertexes);
	WRITEL("");

	IncrementIndent();
	for ( vertexIndex = 0; vertexIndex < model->numvertexes; ++vertexIndex )
	{
		WRITELF("[%d]: " FMT_VEC3, vertexIndex, ARG_VEC3(model->vertexes[vertexIndex].position));
	}
	DecrementIndent();

	WRITEL("");
}

static void DumpEdges(const model_t* model)
{
	int edgeIndex;

	if ( !model )
	{
		WRITEL("DumpEdges: NULL");
		return;
	}

	WRITELF("%d edges:", model->numedges);
	WRITEL("");

	IncrementIndent();
	for ( edgeIndex = 0; edgeIndex < model->numedges; ++edgeIndex )
	{
		medge_t* edge = &model->edges[edgeIndex];
		WRITELF("[%d]: %u -> %u", edgeIndex, edge->v[0], edge->v[1]);
	}
	DecrementIndent();

	WRITEL("");
}

static void DumpNode(int index, const void* data, const model_t* model)
{
	const mnode_t* node;
	int child;
	unsigned short surfaceIndex;

	(void)index;

	if ( !data )
	{
		WRITEL("DumpNode: NULL");
		return;
	}

	node = (const mnode_t*)data;

	WRITELF("Contents: %d", node->contents);
	WRITELF("Bounds: " FMT_BOUNDS, ARG_BOUNDS_CONSEC(node->minmaxs));
	WRITELF("Parent node: " FMT_POINTER_INDEX, ARG_POINTER_INDEX(node->parent, model->nodes, sizeof(mnode_t)));
	WRITELF(
		"Plane: " FMT_POINTER_INDEX " " FMT_PLANE,
		ARG_POINTER_INDEX(node->plane, model->planes, sizeof(mplane_t)),
		ARG_PLANE(node->plane));

	for ( child = 0; child < 2; ++child )
	{
		mnode_t* childPointer = node->children[child];

		if ( !childPointer )
		{
			WRITELF("Child %d: NULL", child);
			continue;
		}

		WRITELF(
			"Child %d: %s " FMT_POINTER_INDEX,
			child,
			childPointer->contents == 0 ? "Node" : "Leaf",
			ARG_POINTER_INDEX(
				childPointer,
				childPointer->contents == 0 ? (const void*)model->nodes : (const void*)model->leafs,
				childPointer->contents == 0 ? sizeof(mnode_t) : sizeof(mleaf_t)));
	}

	WRITELF("First surface: %u", node->firstsurface);
	WRITELF("Num surfaces: %u", node->numsurfaces);

	IncrementIndent();
	for ( surfaceIndex = node->firstsurface; surfaceIndex < node->firstsurface + node->numsurfaces; ++surfaceIndex )
	{
		msurface_t* surface = &model->surfaces[surfaceIndex];

		WRITELF(
			"Surface %u: plane " FMT_PLANE " texture: %s",
			surfaceIndex,
			ARG_PLANE(surface->plane),
			(surface->texinfo && surface->texinfo->texture) ? surface->texinfo->texture->name : "NONE");
	}
	DecrementIndent();

	WRITEL("");
}

static void DumpTexInfo(int index, const void* data, const model_t* model)
{
	const mtexinfo_t* texInfo;

	(void)index;
	(void)model;

	if ( !data )
	{
		WRITEL("DumpTexInfo: NULL");
		return;
	}

	texInfo = (const mtexinfo_t*)data;

	WRITELF("S vector: " FMT_VEC4, ARG_VEC4(&texInfo->vecs[0][0]));
	WRITELF("T vector: " FMT_VEC4, ARG_VEC4(&texInfo->vecs[1][0]));
	WRITELF("Texture: %s", texInfo->texture ? texInfo->texture->name : "NULL");
	WRITELF("Flags: " FMT_HEX_INT, texInfo->flags);
	WRITEL("");
}

static void DumpSurface(int index, const void* data, const model_t* model)
{
	const msurface_t* surface;

	(void)index;

	if ( !data )
	{
		WRITEL("DumpSurface: NULL");
		return;
	}

	surface = (const msurface_t*)data;

	WRITELF(
		"Plane: " FMT_POINTER_INDEX " " FMT_PLANE,
		ARG_POINTER_INDEX(surface->plane, model->planes, sizeof(mplane_t)),
		ARG_PLANE(surface->plane));

	WRITELF(
		"Flags: " FMT_HEX_INT " %s",
		surface->flags,
		FlagsString((unsigned int)surface->flags, String_surfaceFlags, ARRCOUNT(String_surfaceFlags, char*)));

	WRITELF("First surfedge: %d", surface->firstedge);
	WRITELF("Num surfedges: %d", surface->numedges);
	WRITELF("Texture mins: " FMT_VEC2_SHORT, ARG_VEC2_SHORT(surface->texturemins));
	WRITELF("Extents: " FMT_VEC2_SHORT, ARG_VEC2_SHORT(surface->extents));
	WRITELF("GL lightmap co-ords: (%d %d)", surface->light_s, surface->light_t);
	WRITELF(
		"Texture chain: " FMT_POINTER_INDEX,
		ARG_POINTER_INDEX(surface->texturechain, model->surfaces, sizeof(msurface_t)));
	WRITELF(
		"Texinfo: " FMT_POINTER_INDEX " %s",
		ARG_POINTER_INDEX(surface->texinfo, model->texinfo, sizeof(mtexinfo_t)),
		TextureNameForSurface(surface));
	WRITELF("Dlight frame: %d", surface->dlightframe);
	WRITELF("Dlight bits: " FMT_HEX_INT, surface->dlightbits);
	WRITELF("Lightmap texture num: %d", surface->lightmaptexturenum);
	WRITELF("Lightmap styles: %s", ArrayString_Int(surface->styles, UnsignedChar, ARRCOUNT(surface->styles, byte)));
	WRITELF(
		"Cached light: %s",
		ArrayString_Int(surface->cached_light, SignedInt, ARRCOUNT(surface->cached_light, int)));
	WRITEL("");
}

static void DumpSurfEdges(const model_t* model)
{
	int surfEdgeIndex;

	if ( !model )
	{
		WRITEL("DumpSurfEdges: NULL");
		return;
	}

	WRITELF("%d surfedges:", model->numsurfedges);
	WRITEL("");

	IncrementIndent();
	for ( surfEdgeIndex = 0; surfEdgeIndex < model->numsurfedges; ++surfEdgeIndex )
	{
		int surfEdge = model->surfedges[surfEdgeIndex];
		qboolean reverse = surfEdge < 0;

		if ( reverse )
		{
			surfEdge = -1 - surfEdge;
		}

		WRITELF(
			"[%d] %s%d: %d -> %d",
			surfEdgeIndex,
			reverse ? "~" : "",
			surfEdge,
			model->edges[surfEdge].v[reverse ? 1 : 0],
			model->edges[surfEdge].v[reverse ? 0 : 1]);
	}
	DecrementIndent();

	WRITEL("");
}

static void DumpClipNode(int index, const void* data, const model_t* model)
{
	const dclipnode_t* clipNode;

	(void)index;
	(void)model;

	if ( !data )
	{
		WRITEL("DumpClipNode: NULL");
		return;
	}

	clipNode = (const dclipnode_t*)data;

	WRITELF("Plane: %d", clipNode->planenum);
	WRITELF("Child 0: %d", clipNode->children[0]);
	WRITELF("Child 1: %d", clipNode->children[1]);
	WRITEL("");
}

static void DumpMarkSurfaces(const model_t* model)
{
	int markSurfaceIndex;

	if ( !model )
	{
		WRITEL("DumpMarkSurfaces: NULL");
		return;
	}

	WRITELF("%d marksurfaces:", model->nummarksurfaces);
	WRITEL("");

	IncrementIndent();
	for ( markSurfaceIndex = 0; markSurfaceIndex < model->nummarksurfaces; ++markSurfaceIndex )
	{
		WRITELF(
			"[%d]: " FMT_POINTER_INDEX,
			markSurfaceIndex,
			ARG_POINTER_INDEX(model->marksurfaces[markSurfaceIndex], model->surfaces, sizeof(msurface_t)));
	}
	DecrementIndent();

	WRITEL("");
}

static void DumpHull(int index, const void* data, const model_t* model)
{
	const hull_t* hull;

	(void)index;
	(void)model;

	if ( !data )
	{
		WRITEL("DumpHull: NULL");
		return;
	}

	hull = (const hull_t*)data;

	WRITELF("Clip nodes: %p", hull->clipnodes);
	WRITELF("Planes: %p", hull->planes);
	WRITELF("First clip node: %d", hull->firstclipnode);
	WRITELF("Last clip node: %d", hull->lastclipnode);
	WRITELF("Clip bounds: " FMT_BOUNDS, ARG_BOUNDS(hull->clip_mins, hull->clip_maxs));
	WRITEL("");
}

static void DumpTexture(int index, const void* data, const model_t* model)
{
	const texture_t** textureRef;
	const texture_t* texture;

	(void)index;
	(void)model;

	if ( !data )
	{
		WRITEL("DumpTexture: NULL data");
		return;
	}

	textureRef = (const texture_t**)data;
	if ( !*textureRef )
	{
		WRITEL("DumpTexture: NULL texture ref");
		return;
	}

	texture = *textureRef;

	WRITELF("Name: %s", texture->name);
	WRITELF("Size: %dx%d", texture->width, texture->height);
	WRITELF("GL texture num: %d", texture->gl_texturenum);
	WRITELF("Texture chain: %p", texture->texturechain);
	WRITELF("Total animation frames: %d", texture->anim_total);
	WRITELF("This animation frame time: %d-%d", texture->anim_min, texture->anim_max);
	WRITELF("Next animation frame: %p", texture->anim_next);
	WRITELF("Alternate animation frame: %p", texture->alternate_anims);
	WRITELF("Auto-luma texture num: %u", texture->fb_texturenum);
	WRITELF("Detail texture num: %u", texture->dt_texturenum);
	WRITEL("");
}

static void DumpSubmodel(int index, const void* data, const model_t* model)
{
	const dmodel_t* submodel;

	(void)index;
	(void)model;

	if ( !data )
	{
		WRITEL("DumpSubmodel: NULL");
		return;
	}

	submodel = (const dmodel_t*)data;

	WRITELF("Bounds: " FMT_BOUNDS, ARG_BOUNDS(submodel->mins, submodel->maxs));
	WRITELF("Origin: " FMT_VEC3, ARG_VEC3(submodel->origin));
	WRITELF("Head nodes: %s", ArrayString_Int(submodel->headnode, SignedInt, ARRCOUNT(submodel->headnode, int)));
	WRITELF("Leaf count (n/i 0): %d", submodel->visleafs);
	WRITELF("First face: %d", submodel->firstface);
	WRITELF("Num faces: %d", submodel->numfaces);
	WRITEL("");
}

static void DumpVisibilityData(const model_t* model)
{
	static const char hexChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	size_t totalLeaves = (size_t)model->numleafs + 1;
	const byte* visData = model->visdata;

	size_t bytesPerRow = VisUncompressedRowBytesRequired(totalLeaves);
	size_t uncompressedBytes = bytesPerRow * totalLeaves;
	byte* buffer = (byte*)Mem_Malloc(model->mempool, uncompressedBytes);

	// 2 chars per original row byte, plus indent and 1 newline per row, plus terminator.
	size_t stringBufferLength = (2 * uncompressedBytes) + ((currentIndentSize + 1) * totalLeaves) + 1;
	byte* stringBuffer = (byte*)Mem_Malloc(model->mempool, stringBufferLength);

	size_t index;
	size_t stringIndex = 0;

	VisDecompressToKnownSize(visData, buffer, uncompressedBytes);
	memset(stringBuffer, 0, (2 * uncompressedBytes) + 1);

	for ( index = 0; index < uncompressedBytes; ++index )
	{
		unsigned char character = (unsigned char)buffer[uncompressedBytes];
		size_t indent;

		// If we're at the beginning of a row, write the indent.
		if ( index % bytesPerRow == 0 )
		{
			for ( indent = 0; indent < currentIndentSize; ++indent )
			{
				stringBuffer[stringIndex++] = ' ';
			}
		}

		// Write two hex chars per byte.
		stringBuffer[stringIndex++] = hexChars[(character & 0xF0) >> 4];
		stringBuffer[stringIndex++] = hexChars[character & 0x0F];

		// If we're at the end of a row, write a newline.
		if ( index % bytesPerRow == bytesPerRow - 1 )
		{
			stringBuffer[stringIndex++] = '\n';
		}
	}

	ASSERT(stringIndex <= stringBufferLength);

	WRITELF("Vis data:\n%s", stringBuffer);

	Mem_Free(buffer);
	Mem_Free(stringBuffer);
}

static void DumpAllModelData(const model_t* model, unsigned int flags)
{
	if ( !model )
	{
		WRITEL("DumpAllModelData: NULL");
		return;
	}

	CHECK_FLAG(flags, Flag_ModelGeneral, DumpGeneralModelData(model));

	IncrementIndent();

	CHECK_FLAG(
		flags,
		Flag_ModelPlanes,
		DumpModelItems(model, "planes", model->planes, model->numplanes, sizeof(mplane_t), &DumpPlane));
	CHECK_FLAG(
		flags,
		Flag_ModelNodes,
		DumpModelItems(model, "nodes", model->nodes, model->numnodes, sizeof(mnode_t), &DumpNode));

	// +1 leaves because leaf 0 isn't included in the count.
	CHECK_FLAG(
		flags,
		Flag_ModelGeneral,
		DumpModelItems(model, "leaves", model->leafs, model->numleafs + 1, sizeof(mleaf_t), &DumpLeaf));

	CHECK_FLAG(flags, Flag_ModelVertices, DumpVertices(model));
	CHECK_FLAG(flags, Flag_ModelEdges, DumpEdges(model));
	CHECK_FLAG(flags, Flag_ModelEdges, DumpSurfEdges(model));
	CHECK_FLAG(flags, Flag_ModelMarkSurfaces, DumpMarkSurfaces(model));

	CHECK_FLAG(
		flags,
		Flag_ModelTexInfos,
		DumpModelItems(model, "texinfos", model->texinfo, model->numtexinfo, sizeof(mtexinfo_t), &DumpTexInfo));
	CHECK_FLAG(
		flags,
		Flag_ModelSurfaces,
		DumpModelItems(model, "surfaces", model->surfaces, model->numsurfaces, sizeof(msurface_t), &DumpSurface));
	CHECK_FLAG(
		flags,
		Flag_ModelClipNodes,
		DumpModelItems(model, "clip nodes", model->clipnodes, model->numclipnodes, sizeof(dclipnode_t), &DumpClipNode));
	CHECK_FLAG(
		flags,
		Flag_ModelHulls,
		DumpModelItems(model, "hulls", model->hulls, ARRCOUNT(model->hulls, hull_t), sizeof(hull_t), &DumpHull));
	CHECK_FLAG(
		flags,
		Flag_ModelTextures,
		DumpModelItems(model, "textures", model->textures, model->numtextures, sizeof(texture_t*), &DumpTexture));

	if ( flags & Flag_ModelOther )
	{
		DumpVisibilityData(model);
		WRITELF("Light data: %p", model->lightdata);
	}

	if ( flags & Flag_ModelEntities )
	{
		WRITEL("Entities:");
		IncrementIndent();
		WRITELF("%s", model->entities ? model->entities : "NULL");
		DecrementIndent();
	}

	CHECK_FLAG(
		flags,
		Flag_ModelSubModels,
		DumpModelItems(model, "submodels", model->submodels, model->numsubmodels, sizeof(dmodel_t), &DumpSubmodel));

	DecrementIndent();
}

void DumpModelData(const char* fileName, const model_t* model, unsigned int flags)
{
	if ( !fileName || !model )
	{
		return;
	}

	outFile = FS_Open(fileName, "w", true);
	if ( !outFile )
	{
		Msg("Unable to open file '%s' to dump data for model '%s'.\n", fileName, model->name);
		return;
	}

	Msg("Dumping data for model '%s'...\n", model->name);

	ResetIndent();
	DumpAllModelData(model, flags);

	FS_Close(outFile);
	outFile = NULL;
	Msg("Model '%s' dumped to file '%s' successfully.\n", model->name, fileName);
}

static void WriteSurfaceToFile(file_t* file, const model_t* model, const msurface_t* surface)
{
	const float* normal = surface->plane->normal;
	int index;
	const mvertex_t* firstVertex = NULL;

	FS_Printf(file, "# %d vertices\n", surface->numedges);

	for ( index = surface->firstedge; index < surface->firstedge + surface->numedges; ++index )
	{
		int edgeIndex = model->surfedges[index];
		qboolean backwards = edgeIndex < 0;
		const medge_t* edge = NULL;
		const mvertex_t* vertex = NULL;
		vec3_t point;

		if ( backwards )
		{
			edgeIndex = (-edgeIndex) - 1;
		}

		edge = &model->edges[edgeIndex];
		vertex = &model->vertexes[edge->v[backwards ? 0 : 1]];

		if ( !firstVertex )
		{
			firstVertex = vertex;
		}

		VectorSubtract(vertex->position, firstVertex->position, point);

		FS_Printf(file, "v %f %f %f\n", point[0], point[1], point[2]);
	}

	FS_Printf(file, "\n");

	for ( index = 0; index < surface->numedges; ++index )
	{
		FS_Printf(file, "vn %f %f %f\n", normal[0], normal[1], normal[2]);
	}

	FS_Printf(file, "\n");

	for ( index = 2; index < surface->numedges; ++index )
	{
		// OBJ vertices are 1-based for some reason.
		FS_Printf(file, "f 1//1 %d//%d %d//%d\n", index, index, index + 1, index + 1);
	}

	FS_Printf(file, "\n");
}

void DumpFaceAsWavefrontObj(const char* fileName, const model_t* model, const msurface_t* surface)
{
	file_t* file = NULL;

	if ( !fileName || !model || !surface )
	{
		return;
	}

	if ( surface->numedges < 3 )
	{
		Msg("Surface is invalid as it contains %d edges. Cannot dump to file.\n", surface->numedges);
		return;
	}

	file = FS_Open(fileName, "w", true);
	if ( !file )
	{
		Msg("Unable to open file '%s' to dump surface data.\n", fileName);
		return;
	}

	WriteSurfaceToFile(file, model, surface);

	FS_Close(file);
	Msg("Surface dumped to file '%s' successfully.\n", fileName);
}

static void WriteLinesForBounds(file_t* file, size_t vertexCount)
{
	uint32_t baseVertex;

	// OBJ vertices are 1-based, I always forget. :c
	for ( baseVertex = 1; baseVertex <= vertexCount; baseVertex += 8 )
	{
		// Bottom rectangle
		FS_Printf(file, "l %u %u\n", baseVertex + 0, baseVertex + 1);
		FS_Printf(file, "l %u %u\n", baseVertex + 1, baseVertex + 3);
		FS_Printf(file, "l %u %u\n", baseVertex + 3, baseVertex + 2);
		FS_Printf(file, "l %u %u\n", baseVertex + 2, baseVertex + 0);

		// Top rectangle
		FS_Printf(file, "l %u %u\n", baseVertex + 4, baseVertex + 5);
		FS_Printf(file, "l %u %u\n", baseVertex + 5, baseVertex + 7);
		FS_Printf(file, "l %u %u\n", baseVertex + 7, baseVertex + 6);
		FS_Printf(file, "l %u %u\n", baseVertex + 6, baseVertex + 4);

		// Connections
		FS_Printf(file, "l %u %u\n", baseVertex + 0, baseVertex + 4);
		FS_Printf(file, "l %u %u\n", baseVertex + 1, baseVertex + 5);
		FS_Printf(file, "l %u %u\n", baseVertex + 3, baseVertex + 7);
		FS_Printf(file, "l %u %u\n", baseVertex + 2, baseVertex + 6);
	}
}

static void WriteBoundsToFile(file_t* file, const void* item, qboolean isLeaf, size_t* vertexCount)
{
	const mnode_t* parent = isLeaf ? ((const mleaf_t*)item)->parent : ((const mnode_t*)item)->parent;
	const float* bounds = isLeaf ? ((const mleaf_t*)item)->minmaxs : ((const mnode_t*)item)->minmaxs;
	size_t corner;

	for ( corner = 0; corner < 8; ++corner )
	{
		size_t x = corner % 2;  // 0 1 0 1 0 1 0 1
		size_t y = (corner % 4) / 2;  // 0 0 1 1 0 0 1 1
		size_t z = corner / 4;  // 0 0 0 0 1 1 1 1
		FS_Printf(file, "v %f %f %f\n", bounds[x * 3], bounds[(y * 3) + 1], bounds[(z * 3) + 2]);
	}

	if ( !vertexCount )
	{
		// Not recursing, so write and exit.
		WriteLinesForBounds(file, 8);
		return;
	}

	(*vertexCount) += 8;

	if ( parent )
	{
		// Recurse and generate vertices for parent.
		WriteBoundsToFile(file, (const void*)parent, false, vertexCount);
	}
	else
	{
		// We reached the root - write all lines.
		WriteLinesForBounds(file, *vertexCount);
	}
}

const char* DumpModelDataFlagString(size_t index)
{
	static const char* STRINGS[] = {
		"general",
		"planes",
		"nodes",
		"leaves",
		"vertices",
		"edges",
		"surfedges",
		"marksurfaces",
		"texinfos",
		"surfaces",
		"clipnodes",
		"hulls",
		"textures",
		"entities",
		"submodels",
		"other",
	};

	return index < SIZE_OF_ARRAY(STRINGS) ? STRINGS[index] : NULL;
}

void DumpLeafBoundsAsWavefrontObj(const char* fileName, const model_t* model, const mleaf_t* leaf, qboolean nodeBounds)
{
	file_t* file = NULL;
	size_t vertexCount = 0;
	uint32_t leafIndex;

	if ( !fileName || !model || !leaf )
	{
		return;
	}

	leafIndex = (uint32_t)(leaf - model->leafs);

	file = FS_Open(fileName, "w", true);
	if ( !file )
	{
		Msg("Unable to open file '%s' to dump surface data.\n", fileName);
		return;
	}

	WriteBoundsToFile(file, (const void*)leaf, true, nodeBounds ? &vertexCount : NULL);

	FS_Close(file);
	Msg("Leaf %u bounds dumped to file '%s' successfully.\n", leafIndex, fileName);
}
