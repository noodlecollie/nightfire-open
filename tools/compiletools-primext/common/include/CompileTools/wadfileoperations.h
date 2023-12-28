#pragma once

#include "BuildPlatform/Typedefs.h"
#include "CompileTools/basefs.h"
#include "CompileTools/engine/wadfile.h"

#define WAD3_NAMELEN 16

struct stringlist_s;

#pragma pack(push, 1)
typedef struct
{
	int ident;  // should be WAD3
	int numlumps;  // num files
	int infotableofs;  // LUT offset
} dwadinfo_t;

typedef struct
{
	int filepos;  // file offset in WAD
	int disksize;  // compressed or uncompressed
	int size;  // uncompressed
	int8_t type;
	int8_t attribs;  // file attribs
	int8_t img_type;  // IMG_*
	int8_t pad;
	char name[WAD3_NAMELEN];  // must be null terminated
} dlumpinfo_t;
#pragma pack(pop)

typedef struct wfile_s
{
	char filename[256];
	int infotableofs;
	int numlumps;
	int mode;
	file_t* fsptr;
	int fdhandle;
	time_t filetime;
	dlumpinfo_t* lumps;
} wfile_t;

typedef struct wadtype_s
{
	const char* ext;
	int type;
} wadtype_t;

extern const wadtype_t wad_hints[];

qboolean W_AllowWadFilesInPacks(void);
void W_SetAllowWadFilesInPacks(qboolean allow);
bool W_HasFSPointer(wfile_t* wad);

wfile_t* W_Open(const char* filename, const char* mode, int* error = NULL, qboolean ext_path = true);
byte* W_ReadLump(wfile_t* wad, dlumpinfo_t* lump, size_t* lumpsizeptr);
byte* W_LoadLump(wfile_t* wad, const char* lumpname, size_t* lumpsizeptr, int type);
int W_SaveLump(wfile_t* wad, const char* lump, const void* data, size_t datasize, int type, char attribs = ATTR_NONE);
void W_SearchForFile(wfile_t* wad, const char* pattern, struct stringlist_s* resultlist);
dlumpinfo_t* W_FindLump(wfile_t* wad, const char* name, int matchtype);
dlumpinfo_t* W_FindMiptex(wfile_t* wad, const char* name);
dlumpinfo_t* W_FindLmptex(wfile_t* wad, const char* name);
int W_TypeFromExt(const char* lumpname);
const char* W_ExtFromType(int lumptype);
char W_HintFromSuf(const char* lumpname);
int W_GetHandle(wfile_t* wad);
void W_Close(wfile_t* wad);
