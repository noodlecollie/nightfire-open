#include "cmdlib.h"
#include "filelib.h"
#include "messages.h"
#include "hlassert.h"
#include "log.h"
#include "mathlib.h"
#include "bspfile.h"
#include "scriplib.h"
#include "blockmem.h"
#include "checksum.h"
#include "texturecollection.h"
#include "texturecollectionreader.h"
#include "texturecollectionwriter.h"
#include "texturedirectorylisting.h"
#include "miptexwrapper.h"
#include "nfopenheader.h"

//=============================================================================

int		g_max_map_miptex = DEFAULT_MAX_MAP_MIPTEX;
int		g_max_map_lightdata = DEFAULT_MAX_MAP_LIGHTDATA;
bool		g_found_extradata = false;

int		g_nummodels;
dmodel_t		g_dmodels[MAX_MAP_MODELS];
int		g_dmodels_checksum;

int		g_visdatasize;
byte		g_dvisdata[MAX_MAP_VISIBILITY];
int		g_dvisdata_checksum;

int		g_lightdatasize;
byte*		g_dlightdata;
int		g_dlightdata_checksum;

int		g_deluxdatasize;
byte*		g_ddeluxdata;
int		g_ddeluxdata_checksum;

TextureCollection g_TextureCollection;
int g_TextureCollectionChecksum;
TextureDirectoryListing g_TexDirListing;

int		g_entdatasize;
char		g_dentdata[MAX_MAP_ENTSTRING];
int		g_dentdata_checksum;

int		g_numleafs;
dleaf_t		g_dleafs[MAX_MAP_LEAFS];
int		g_dleafs_checksum;

int		g_numplanes;
dplane_t		g_dplanes[MAX_INTERNAL_MAP_PLANES];
int		g_dplanes_checksum;

int		g_numvertexes;
dvertex_t		g_dvertexes[MAX_MAP_VERTS];
int		g_dvertexes_checksum;

int		g_numnodes;
dnode_t		g_dnodes[MAX_MAP_NODES];
int		g_dnodes_checksum;

int		g_numtexinfo;
#ifdef HLCSG_HLBSP_REDUCETEXTURE
texinfo_t		g_texinfo[MAX_INTERNAL_MAP_TEXINFO];
#else
texinfo_t		g_texinfo[MAX_MAP_TEXINFO];
#endif
int		g_texinfo_checksum;

int		g_numfaces;
dface_t		g_dfaces[MAX_MAP_FACES];
int		g_dfaces_checksum;

#ifdef ZHLT_XASH2
int		g_numclipnodes[MAX_MAP_HULLS - 1];
dclipnode_t	g_dclipnodes[MAX_MAP_HULLS - 1][MAX_MAP_CLIPNODES];
int		g_dclipnodes_checksum[MAX_MAP_HULLS - 1];
#else
int		g_numclipnodes;
dclipnode_t	g_dclipnodes[MAX_MAP_CLIPNODES];
int		g_dclipnodes_checksum;
#endif

int		g_numedges;
dedge_t		g_dedges[MAX_MAP_EDGES];
int		g_dedges_checksum;

int		g_nummarksurfaces;
unsigned short	g_dmarksurfaces[MAX_MAP_MARKSURFACES];
int		g_dmarksurfaces_checksum;

int		g_numsurfedges;
int		g_dsurfedges[MAX_MAP_SURFEDGES];
int		g_dsurfedges_checksum;

int		g_numentities;
entity_t		g_entities[MAX_MAP_ENTITIES];

#ifdef ZHLT_PARANOIA_BSP
int		g_numfaceinfo;
dfaceinfo_t	g_dfaceinfo[MAX_MAP_FACEINFO];
int		g_dfaceinfo_checksum;

int		g_numnormals;
dnormal_t		g_dnormals[MAX_MAP_VERTS];
int		g_dnormals_checksum;

int		g_numcubemaps;
dcubemap_t	g_dcubemaps[MAX_MAP_CUBEMAPS];
int		g_dcubemaps_checksum;

int		g_numleaflights;
dleafsample_t	g_dleaflights[MAX_MAP_LEAFLIGHTS];
int		g_dleaflights_checksum;

int		g_numworldlights;
dworldlight_t	g_dworldlights[MAX_MAP_WORLDLIGHTS];
int		g_dworldlights_checksum;
#endif  // ZHLT_PARANOIA_BSP

#ifdef ZHLT_NFOPEN
size_t g_numclientmodels = 0;
dnfopenclientents_model_t g_clientmodels[NFOPEN_CLIENT_ENT_MAX_MODELS];
int g_clientmodels_checksum = 0;

size_t g_numclientsounds = 0;
dnfopenclientents_sound_t g_clientsounds[NFOPEN_CLIENT_ENT_MAX_SOUNDS];
int g_clientsounds_checksum = 0;
#endif  // ZHLT_NFOPEN

// can be overrided from hlcsg
vec3_t g_hull_size[MAX_MAP_HULLS][2] =
{
	{ // 0x0x0
	{ 0, 0, 0 },	{ 0, 0, 0 }
	},
	{ // 32x32x72
	{-16, -16, -36 },	{ 16, 16, 36 }
	},
	{ // 64x64x64
	{-32, -32, -32 },	{ 32, 32, 32 }
	},
	{ // 32x32x36
	{-16, -16, -18 },	{ 16, 16, 18 }
	}
};

static const void* CompileClientEntitiesLump(size_t& numBytes, bool force = false)
{
	static uint8_t data[NFOPEN_CLIENT_ENT_LUMP_MAX_SIZE];
	static size_t dataLength = 0;

	if ( dataLength > 0 && !force )
	{
		numBytes = dataLength;
		return data;
	}

	numBytes = 0;

	if ( g_numclientmodels > NFOPEN_CLIENT_ENT_MAX_MODELS )
	{
		Error("%zu client models exceeded max of %zu\n", g_numclientmodels, NFOPEN_CLIENT_ENT_MAX_MODELS);
		return nullptr;
	}

	if ( g_numclientsounds > NFOPEN_CLIENT_ENT_MAX_SOUNDS )
	{
		Error("%zu client sounds exceeded max of %zu\n", g_numclientsounds, NFOPEN_CLIENT_ENT_MAX_SOUNDS);
		return nullptr;
	}

	uint8_t* cursor = data;

	dnfopenclientents_header_t* header = reinterpret_cast<dnfopenclientents_header_t*>(cursor);
	header->version = NFOPEN_CLIENT_ENT_HEADER_VERSION;
	cursor += sizeof(*header);

	header->modelOffsetFromBeginningOfHeader = static_cast<uint32_t>(cursor - data);
	header->modelCount = static_cast<uint32_t>(g_numclientmodels);

	for ( size_t index = 0; index < g_numclientmodels; ++index )
	{
		dnfopenclientents_model_t* model = reinterpret_cast<dnfopenclientents_model_t*>(cursor);
		*model = g_clientmodels[index];
		cursor += sizeof(*model);
	}

	header->soundOffsetFromBeginningOfHeader = static_cast<uint32_t>(cursor - data);
	header->soundCount = static_cast<uint32_t>(g_numclientsounds);

	for ( size_t index = 0; index < g_numclientsounds; ++index )
	{
		dnfopenclientents_sound_t* sound = reinterpret_cast<dnfopenclientents_sound_t*>(cursor);
		*sound = g_clientsounds[index];
		cursor += sizeof(*sound);
	}

	dataLength = static_cast<size_t>(cursor - data);
	numBytes = dataLength;
	return data;
}

static void ReadClientEntitiesLump(const void* inData, size_t inLength)
{
	if ( inLength < sizeof(dnfopenclientents_header_t) )
	{
		Error("Client entities lump was too short to read\n");
		return;
	}

	const dnfopenclientents_header_t* header = reinterpret_cast<const dnfopenclientents_header_t*>(inData);

	if ( header->version != NFOPEN_CLIENT_ENT_HEADER_VERSION )
	{
		Error("Expected client entities lump version %u but got %u\n", NFOPEN_CLIENT_ENT_HEADER_VERSION, header->version);
		return;
	}

	const size_t totalSize = sizeof(dnfopenclientents_header_t) +
		(header->modelCount * sizeof(dnfopenclientents_model_t)) +
		(header->soundCount * sizeof(dnfopenclientents_sound_t));

	if ( inLength < totalSize )
	{
		Error("Expected client entities lump length to be %zu but got %zu\n", totalSize, inLength);
		return;
	}

	if ( header->modelOffsetFromBeginningOfHeader + (header->modelCount * sizeof(dnfopenclientents_model_t)) > inLength )
	{
		Error("Client entities lump corrupted: models exceeded available data\n");
		return;
	}

	if ( header->soundOffsetFromBeginningOfHeader + (header->soundCount * sizeof(dnfopenclientents_sound_t)) > inLength )
	{
		Error("Client entities lump corrupted: sounds exceeded available data\n");
		return;
	}

	if ( inLength > totalSize )
	{
		Warning("Expected client entities lump length to be %zu but got %zu, ignoring extra data\n", totalSize, inLength);
	}

	g_numclientmodels = static_cast<size_t>(header->modelCount);

	const dnfopenclientents_model_t* modelsBase = reinterpret_cast<const dnfopenclientents_model_t*>(
		static_cast<const uint8_t*>(inData) + header->modelOffsetFromBeginningOfHeader);

	for ( size_t index = 0; index < g_numclientmodels; ++index )
	{
		g_clientmodels[index] = modelsBase[index];
	}

	g_numclientsounds = static_cast<size_t>(header->soundCount);

	const dnfopenclientents_sound_t* soundsBase = reinterpret_cast<const dnfopenclientents_sound_t*>(
		static_cast<const uint8_t*>(inData) + header->soundOffsetFromBeginningOfHeader);

	for ( size_t index = 0; index < g_numclientsounds; ++index )
	{
		g_clientsounds[index] = soundsBase[index];
	}
}

/*
 * ===============
 * CompressVis
 * ===============
*/
int CompressVis( const byte *src, const unsigned int src_length, byte *dest, unsigned int dest_length )
{
	byte		*dest_p = dest;
	unsigned int	j, current_length = 0;

	for( j = 0; j < src_length; j++ )
	{
		current_length++;
		hlassume( current_length <= dest_length, assume_COMPRESSVIS_OVERFLOW );

		*dest_p = src[j];
		dest_p++;

		if( src[j] )
		{
			continue;
		}

		unsigned char	rep = 1;

		for( j++; j < src_length; j++ )
		{
			if( src[j] || rep == 255 )
			{
				break;
			}
			else
			{
				rep++;
			}
		}

		current_length++;
		hlassume( current_length <= dest_length, assume_COMPRESSVIS_OVERFLOW );

		*dest_p = rep;
		dest_p++;
		j--;
	}

	return dest_p - dest;
}


/*
 * ===============
 * DecompressVis
 * ===============
*/
void DecompressVis( const byte* src, byte* const dest, const unsigned int dest_length )
{
	unsigned int	current_length = 0;
	int		c;
	byte*		out;
	int		row;

#ifdef ZHLT_DecompressVis_FIX
	row = (g_dmodels[0].visleafs + 7) >> 3; // same as the length used by VIS program in CompressVis
	// The wrong size will cause DecompressVis to spend extremely long time once the source pointer
	// runs into the invalid area in g_dvisdata (for example, in BuildFaceLights, some faces could hang for a few seconds), and sometimes to crash.
#else
	row = (g_numleafs + 7) >> 3;
#endif
	out = dest;

	do
	{
#ifdef ZHLT_DecompressVis_FIX
		hlassume( src - g_dvisdata < g_visdatasize, assume_DECOMPRESSVIS_OVERFLOW );
#endif
		if( *src )
		{
			current_length++;
			hlassume( current_length <= dest_length, assume_DECOMPRESSVIS_OVERFLOW );

			*out = *src;
			out++;
			src++;
			continue;
		}

#ifdef ZHLT_DecompressVis_FIX
		hlassume (&src[1] - g_dvisdata < g_visdatasize, assume_DECOMPRESSVIS_OVERFLOW);
#endif
		c = src[1];
		src += 2;

		while( c )
		{
			current_length++;
			hlassume( current_length <= dest_length, assume_DECOMPRESSVIS_OVERFLOW );

			*out = 0;
			out++;
			c--;

			if( out - dest >= row )
			{
				return;
			}
		}
	} while( out - dest < row );
}

//
// =====================================================================================
//

// =====================================================================================
//  SwapBSPFile
//      byte swaps all data in a bsp file
// =====================================================================================
static void SwapBSPFile( const bool todisk )
{
	int		i, j, c;
	dmodel_t*		d;
	dmiptexlump_t*	mtl;

	// models
	for( i = 0; i < g_nummodels; i++ )
	{
		d = &g_dmodels[i];

		for (j = 0; j < MAX_MAP_HULLS; j++)
		{
			d->headnode[j] = LittleLong(d->headnode[j]);
		}

		d->visleafs = LittleLong(d->visleafs);
		d->firstface = LittleLong(d->firstface);
		d->numfaces = LittleLong(d->numfaces);

		for (j = 0; j < 3; j++)
		{
			d->mins[j] = LittleFloat(d->mins[j]);
			d->maxs[j] = LittleFloat(d->maxs[j]);
			d->origin[j] = LittleFloat(d->origin[j]);
		}
	}

	//
	// vertexes
	//
	for( i = 0; i < g_numvertexes; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			g_dvertexes[i].point[j] = LittleFloat( g_dvertexes[i].point[j] );
		}
	}

	//
	// planes
	//
	for( i = 0; i < g_numplanes; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			g_dplanes[i].normal[j] = LittleFloat( g_dplanes[i].normal[j] );
		}

		g_dplanes[i].dist = LittleFloat( g_dplanes[i].dist );
		g_dplanes[i].type = (planetypes)LittleLong( g_dplanes[i].type );
	}

	//
	// texinfos
	//
	for( i = 0; i < g_numtexinfo; i++ )
	{
		for( j = 0; j < 8; j++ )
		{
			g_texinfo[i].vecs[j/2][j%4] = LittleFloat( g_texinfo[i].vecs[j/2][j%4] );
		}

		g_texinfo[i].miptex = LittleLong( g_texinfo[i].miptex );
#ifdef ZHLT_PARANOIA_BSP
		g_texinfo[i].flags = LittleShort( g_texinfo[i].flags );
		g_texinfo[i].faceinfo = LittleShort( g_texinfo[i].faceinfo );
#else
		g_texinfo[i].flags = LittleLong( g_texinfo[i].flags );
#endif
	}

	//
	// faces
	//
	for( i = 0; i < g_numfaces; i++ )
	{
		g_dfaces[i].texinfo = LittleShort( g_dfaces[i].texinfo );
		g_dfaces[i].planenum = LittleShort( g_dfaces[i].planenum );
		g_dfaces[i].side = LittleShort( g_dfaces[i].side );
		g_dfaces[i].lightofs = LittleLong( g_dfaces[i].lightofs );
		g_dfaces[i].firstedge = LittleLong( g_dfaces[i].firstedge );
		g_dfaces[i].numedges = LittleShort( g_dfaces[i].numedges );
	}

	//
	// nodes
	//
	for( i = 0; i < g_numnodes; i++ )
	{
		g_dnodes[i].planenum = LittleLong( g_dnodes[i].planenum );

		for( j = 0; j < 3; j++ )
		{
			g_dnodes[i].mins[j] = LittleShort( g_dnodes[i].mins[j] );
			g_dnodes[i].maxs[j] = LittleShort( g_dnodes[i].maxs[j] );
		}

		g_dnodes[i].children[0] = LittleShort( g_dnodes[i].children[0] );
		g_dnodes[i].children[1] = LittleShort( g_dnodes[i].children[1] );
		g_dnodes[i].firstface = LittleShort( g_dnodes[i].firstface );
		g_dnodes[i].numfaces = LittleShort( g_dnodes[i].numfaces );
	}

	//
	// leafs
	//
	for( i = 0; i < g_numleafs; i++ )
	{
		g_dleafs[i].contents = LittleLong( g_dleafs[i].contents );

		for( j = 0; j < 3; j++ )
		{
			g_dleafs[i].mins[j] = LittleShort( g_dleafs[i].mins[j] );
			g_dleafs[i].maxs[j] = LittleShort( g_dleafs[i].maxs[j] );
		}

		g_dleafs[i].firstmarksurface = LittleShort( g_dleafs[i].firstmarksurface );
		g_dleafs[i].nummarksurfaces = LittleShort( g_dleafs[i].nummarksurfaces );
		g_dleafs[i].visofs = LittleLong( g_dleafs[i].visofs );
	}

	//
	// clipnodes
	//
#ifdef ZHLT_XASH2
	for( int hull = 1; hull < MAX_MAP_HULLS; hull++ )
	{
		for( i = 0; i < g_numclipnodes[hull - 1]; i++ )
		{
			g_dclipnodes[hull - 1][i].planenum = LittleLong( g_dclipnodes[hull - 1][i].planenum );
			g_dclipnodes[hull - 1][i].children[0] = LittleShort( g_dclipnodes[hull - 1][i].children[0] );
			g_dclipnodes[hull - 1][i].children[1] = LittleShort( g_dclipnodes[hull - 1][i].children[1] );
		}
	}
#else
	for( i = 0; i < g_numclipnodes; i++ )
	{
		g_dclipnodes[i].planenum = LittleLong( g_dclipnodes[i].planenum );
		g_dclipnodes[i].children[0] = LittleShort( g_dclipnodes[i].children[0] );
		g_dclipnodes[i].children[1] = LittleShort( g_dclipnodes[i].children[1] );
	}
#endif

	// Textures do not need to be byte-swapped, as this is handled automatically on import/export.

	//
	// marksurfaces
	//
	for( i = 0; i < g_nummarksurfaces; i++ )
	{
		g_dmarksurfaces[i] = LittleShort( g_dmarksurfaces[i] );
	}

	//
	// surfedges
	//
	for( i = 0; i < g_numsurfedges; i++ )
	{
		g_dsurfedges[i] = LittleLong( g_dsurfedges[i] );
	}

	//
	// edges
	//
	for( i = 0; i < g_numedges; i++ )
	{
		g_dedges[i].v[0] = LittleShort( g_dedges[i].v[0] );
		g_dedges[i].v[1] = LittleShort( g_dedges[i].v[1] );
	}

#ifdef ZHLT_PARANOIA_BSP
	//
	// faceinfo
	//
	for( i = 0; i < g_numfaceinfo; i++ )
	{
		g_dfaceinfo[i].texture_step = LittleShort( g_dfaceinfo[i].texture_step );
		g_dfaceinfo[i].max_extent = LittleShort( g_dfaceinfo[i].max_extent );
		g_dfaceinfo[i].groupid = LittleShort( g_dfaceinfo[i].groupid );
	}

	//
	// normals
	//
	for( i = 0; i < g_numnormals; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			g_dnormals[i].normal[j] = LittleFloat( g_dnormals[i].normal[j] );
		}
	}

	//
	// cubemaps
	//
	for( i = 0; i < g_numcubemaps; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			g_dcubemaps[i].origin[j] = LittleShort( g_dcubemaps[i].origin[j] );
		}

		g_dcubemaps[i].size = LittleShort( g_dcubemaps[i].size );
	}

	//
	// leaflights
	//
	for( i = 0; i < g_numleaflights; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			g_dleaflights[i].origin[j] = LittleShort( g_dleaflights[i].origin[j] );
		}

		g_dleaflights[i].leafnum = LittleShort( g_dleaflights[i].leafnum );
	}

	//
	// worldlights
	//
	for( i = 0; i < g_numworldlights; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			g_dworldlights[i].origin[j] = LittleShort( g_dworldlights[i].origin[j] );
			g_dworldlights[i].intensity[j] = LittleFloat( g_dworldlights[i].intensity[j] );
			g_dworldlights[i].normal[j] = LittleFloat( g_dworldlights[i].normal[j] );
		}

		g_dworldlights[i].stopdot = LittleFloat( g_dworldlights[i].stopdot );
		g_dworldlights[i].stopdot2 = LittleFloat( g_dworldlights[i].stopdot2 );
		g_dworldlights[i].fade = LittleFloat( g_dworldlights[i].fade );
		g_dworldlights[i].radius = LittleFloat( g_dworldlights[i].radius );
		g_dworldlights[i].facenum = LittleShort( g_dworldlights[i].facenum );
		g_dworldlights[i].leafnum = LittleShort( g_dworldlights[i].leafnum );
		g_dworldlights[i].modelnumber = LittleShort( g_dworldlights[i].modelnumber );
	}
#endif
}

// =====================================================================================
//  CopyLump
//      balh
// =====================================================================================
static int CopyLump( int lump, void *dest, int size, const dheader_t *header )
{
	int	length, ofs;

	length = header->lumps[lump].filelen;
	ofs = header->lumps[lump].fileofs;

	if( length % size )
	{
		Error( "LoadBSPFile: Lump size %d was expected to be multiple of %d", length, size );
	}

	// special handling for tex and lightdata to keep things from exploding - KGP
	if( lump == LUMP_LIGHTING && dest == (void*)g_dlightdata )
		hlassume( g_max_map_lightdata > length, assume_MAX_MAP_LIGHTING );

	memcpy( dest, (byte *)header + ofs, length );

	return length / size;
}

#ifdef ZHLT_PARANOIA_BSP
static int CopyExtraLump( int lump, void* dest, int size, const dheader_t* const header )
{

	dextrahdr_t*	extrahdr = (dextrahdr_t *)((byte *)header + sizeof( dheader_t ));
	int		length, ofs;

	length = extrahdr->lumps[lump].filelen;
	ofs = extrahdr->lumps[lump].fileofs;

	if( length % size )
	{
		Error( "LoadBSPFile: odd lump size" );
	}

	// special handling for lightvectors to keep things from exploding - G-cont
	if( lump == LUMP_LIGHTVECS && dest == (void*)g_ddeluxdata )
		hlassume( g_max_map_lightdata > length, assume_MAX_MAP_LIGHTING );

	memcpy( dest, (byte *)header + ofs, length );

	return length / size;
}
#endif

// =====================================================================================
//  LoadBSPFile
//      balh
// =====================================================================================
void LoadBSPFile( const char *filename )
{
	dheader_t	*header;

	LoadFile( filename, (char **)&header );
	LoadBSPImage( header );
}

// =====================================================================================
//  LoadBSPImage
//      balh
// =====================================================================================
void LoadBSPImage( dheader_t* const header )
{
	unsigned int     i, hull;

	// swap the header
	for( i = 0; i < sizeof( dheader_t ) / 4; i++ )
	{
		((int*)header)[i] = LittleLong(((int *)header)[i] );
	}

	if( header->version != BSPVERSION )
	{
		Error( "BSP is version %i, not %i", header->version, BSPVERSION );
	}

	g_nummodels = CopyLump( LUMP_MODELS, g_dmodels, sizeof( dmodel_t ), header );
	g_numvertexes = CopyLump( LUMP_VERTEXES, g_dvertexes, sizeof( dvertex_t ), header );
	g_numplanes = CopyLump( LUMP_PLANES, g_dplanes, sizeof( dplane_t ), header );
	g_numleafs = CopyLump( LUMP_LEAFS, g_dleafs, sizeof( dleaf_t ), header );
	g_numnodes = CopyLump( LUMP_NODES, g_dnodes, sizeof( dnode_t ), header );
	g_numtexinfo = CopyLump( LUMP_TEXINFO, g_texinfo, sizeof( texinfo_t ), header );

#ifdef ZHLT_XASH2
	for( hull = 1; hull < MAX_MAP_HULLS; hull++ )
	{
		int lump;

		switch( hull )
		{
		case 1: lump = LUMP_CLIPNODES; break;
		case 2: lump = LUMP_CLIPNODES2; break;
		case 3: lump = LUMP_CLIPNODES3; break;
		default:
			Error( "bad hull number %d", hull );
			break;
		}

		g_numclipnodes[hull - 1] = CopyLump( lump, g_dclipnodes[hull - 1], sizeof( dclipnode_t ), header );
	}
#else
	g_numclipnodes = CopyLump( LUMP_CLIPNODES, g_dclipnodes, sizeof( dclipnode_t ), header );
#endif
	g_numfaces = CopyLump( LUMP_FACES, g_dfaces, sizeof( dface_t ), header );
	g_nummarksurfaces = CopyLump( LUMP_MARKSURFACES, g_dmarksurfaces, sizeof( g_dmarksurfaces[0] ), header );
	g_numsurfedges = CopyLump( LUMP_SURFEDGES, g_dsurfedges, sizeof( g_dsurfedges[0] ), header );
	g_numedges = CopyLump( LUMP_EDGES, g_dedges, sizeof( dedge_t ), header );
	TextureCollectionReader(g_TextureCollection, g_TexDirListing)
		.load(reinterpret_cast<const byte*>(header) + header->lumps[LUMP_TEXTURES].fileofs,
			  header->lumps[LUMP_TEXTURES].filelen);
	g_visdatasize = CopyLump( LUMP_VISIBILITY, g_dvisdata, 1, header );
	g_lightdatasize = CopyLump( LUMP_LIGHTING, g_dlightdata, 1, header );
	g_entdatasize = CopyLump( LUMP_ENTITIES, g_dentdata, 1, header );

#ifdef ZHLT_PARANOIA_BSP
	dextrahdr_t*    extrahdr = (dextrahdr_t *)((byte *)header + sizeof( dheader_t ));

	if( LittleLong( extrahdr->id ) == IDEXTRAHEADER )
	{
		if( LittleLong( extrahdr->version ) != EXTRA_VERSION )
		{
			Error("BSP is extra version %i, not %i", extrahdr->version, EXTRA_VERSION);
		}

		g_found_extradata = true;

		// swap the header
		for( i = 0; i < sizeof( dextrahdr_t ) / 4; i++ )
		{
			((int*)extrahdr)[i] = LittleLong(((int *)extrahdr)[i] );
		}

		// g-cont. copy the extra lumps
		g_numnormals = CopyExtraLump( LUMP_VERTNORMALS, g_dnormals, sizeof( dnormal_t ), header );
		g_deluxdatasize = CopyExtraLump( LUMP_LIGHTVECS, g_ddeluxdata, 1, header );
		g_numcubemaps = CopyExtraLump( LUMP_CUBEMAPS, g_dcubemaps, sizeof( dcubemap_t ), header );
		g_numfaceinfo = CopyExtraLump( LUMP_FACEINFO, g_dfaceinfo, sizeof( dfaceinfo_t ), header );
		g_numleaflights = CopyExtraLump( LUMP_LEAF_LIGHTING, g_dleaflights, sizeof( dleafsample_t ), header );
		g_numworldlights = CopyExtraLump( LUMP_WORLDLIGHTS, g_dworldlights, sizeof( dworldlight_t ), header );
	}
#endif

#ifdef ZHLT_NFOPEN
	{
		dnfopenextraheader_t* nfHeader = NFOpen::GetHeader(header);
		dnfopenextralump_t* lump = NFOpen::LookUpLump(nfHeader, NFOPEN_LUMP_CLIENTENTS);

		ReadClientEntitiesLump(
			reinterpret_cast<const byte*>(nfHeader) + lump->offsetFromBeginningOfExtraHeader,
			static_cast<size_t>(lump->dataLength));
	}
#endif  // ZHLT_NFOPEN

	Free( header );	// everything has been copied out

	//
	// swap everything
	//
	SwapBSPFile( false );

	g_dmodels_checksum = FastChecksum( g_dmodels, g_nummodels * sizeof( g_dmodels[0] ));
	g_dvertexes_checksum = FastChecksum( g_dvertexes, g_numvertexes * sizeof( g_dvertexes[0] ));
	g_dplanes_checksum = FastChecksum( g_dplanes, g_numplanes * sizeof( g_dplanes[0] ));
	g_dleafs_checksum = FastChecksum( g_dleafs, g_numleafs * sizeof( g_dleafs[0] ));
	g_dnodes_checksum = FastChecksum( g_dnodes, g_numnodes * sizeof( g_dnodes[0] ));
	g_texinfo_checksum = FastChecksum( g_texinfo, g_numtexinfo * sizeof( g_texinfo[0] ));

#ifdef ZHLT_XASH2
	for( hull = 1; hull < MAX_MAP_HULLS; hull++ )
	{
		g_dclipnodes_checksum[hull - 1] = FastChecksum( g_dclipnodes[hull - 1], g_numclipnodes[hull - 1] * sizeof( g_dclipnodes[hull - 1][0] ));
	}
#else
	g_dclipnodes_checksum = FastChecksum( g_dclipnodes, g_numclipnodes * sizeof( g_dclipnodes[0] ));
#endif
	g_dfaces_checksum = FastChecksum( g_dfaces, g_numfaces * sizeof( g_dfaces[0] ));
	g_dmarksurfaces_checksum = FastChecksum( g_dmarksurfaces, g_nummarksurfaces * sizeof( g_dmarksurfaces[0] ));
	g_dsurfedges_checksum = FastChecksum( g_dsurfedges, g_numsurfedges * sizeof( g_dsurfedges[0] ));
	g_dedges_checksum = FastChecksum( g_dedges, g_numedges * sizeof( g_dedges[0] ));
	g_TextureCollectionChecksum = TextureCollectionWriter(g_TextureCollection).calculateChecksum();
	g_dvisdata_checksum = FastChecksum( g_dvisdata, g_visdatasize * sizeof( g_dvisdata[0] ));
	g_dlightdata_checksum = FastChecksum( g_dlightdata, g_lightdatasize * sizeof( g_dlightdata[0] ));
	g_dentdata_checksum = FastChecksum( g_dentdata, g_entdatasize * sizeof( g_dentdata[0] ));

#ifdef ZHLT_NFOPEN
	g_clientmodels_checksum = FastChecksum( g_clientmodels, g_numclientmodels * sizeof( g_clientmodels[0] ));
	g_clientsounds_checksum = FastChecksum( g_clientsounds, g_numclientsounds * sizeof( g_clientsounds[0] ));
#endif

#ifdef ZHLT_PARANOIA_BSP
	if( g_found_extradata )
	{
		g_dnormals_checksum = FastChecksum( g_dnormals, g_numnormals * sizeof( g_dnormals[0] ));
		g_ddeluxdata_checksum = FastChecksum( g_ddeluxdata, g_deluxdatasize * sizeof( g_ddeluxdata[0] ));
		g_dcubemaps_checksum = FastChecksum( g_dcubemaps, g_numcubemaps * sizeof( g_dcubemaps[0] ));
		g_dfaceinfo_checksum = FastChecksum( g_dfaceinfo, g_numfaceinfo * sizeof( g_dfaceinfo[0] ));
		g_dleaflights_checksum = FastChecksum( g_dleaflights, g_numleaflights * sizeof( g_dleaflights[0] ));
		g_dworldlights_checksum = FastChecksum( g_dworldlights, g_numworldlights * sizeof( g_dworldlights[0] ));
	}
#endif
}

//
// =====================================================================================
//

// =====================================================================================
//  AddLump
//      balh
// =====================================================================================
static void AddLump( int lumpnum, const void* data, int len, dheader_t* header, FILE* bspfile )
{
	lump_t* lump = &header->lumps[lumpnum];
	lump->fileofs = LittleLong( ftell( bspfile ));
	lump->filelen = LittleLong( len );
	SafeWrite( bspfile, data, (len + 3) & ~3 );
}

#ifdef ZHLT_PARANOIA_BSP
static void AddExtraLump( int lumpnum, const void* data, int len, dextrahdr_t* header, FILE* bspfile )
{
	lump_t* lump = &header->lumps[lumpnum];
	lump->fileofs = LittleLong( ftell( bspfile ));
	lump->filelen = LittleLong( len );
	SafeWrite( bspfile, data, (len + 3) & ~3 );
}
#endif

#ifdef ZHLT_NFOPEN
static void WriteLumpDescriptor(const dnfopenextralump_t& lump, FILE* bspfile)
{
	SafeWrite(bspfile, &lump, sizeof(lump));
}

static void WriteLumpData(const void* data, uint32_t len, FILE* bspfile)
{
	SafeWrite(bspfile, data, (len + 3) & ~3);
}
#endif  // ZHLT_NFOPEN

// =====================================================================================
//  WriteBSPFile
//      Swaps the bsp file in place, so it should not be referenced again
// =====================================================================================
void WriteBSPFile( const char* const filename )
{
	dheader_t		outheader;
	dheader_t*	header;
#ifdef ZHLT_PARANOIA_BSP
	dextrahdr_t	outextrahdr;
	dextrahdr_t*	extrahdr;
#endif

#ifdef ZHLT_NFOPEN
	dnfopenextraheader_t outNfHeader;
	dnfopenextraheader_t* nfHeader = &outNfHeader;
#endif  // ZHLT_NFOPEN

	FILE*		bspfile;

	// Do this here, before the file gets byte-swapped.
	TextureCollectionWriter writer(g_TextureCollection);
	writer.exportAll();
	const std::vector<int32_t>& textureIndexMap = writer.indexMap();

	for ( uint32_t index = 0; index < g_numtexinfo; ++index )
	{
		texinfo_t* texInfo = &g_texinfo[index];

		if ( texInfo->miptex == -1 )
		{
			// This is fine - hint brushes trigger this.
			continue;
		}

		if ( texInfo->miptex < 0 || texInfo->miptex >= textureIndexMap.size() )
		{
			Warning("Texinfo %u references invalid texture %d, ignoring remap on export.\n", index, texInfo->miptex);
			continue;
		}

		texInfo->miptex = textureIndexMap[texInfo->miptex];
	}

	header = &outheader;
	memset( header, 0, sizeof( dheader_t ));
#ifdef ZHLT_PARANOIA_BSP
	extrahdr = &outextrahdr;
	memset( extrahdr, 0, sizeof( dextrahdr_t ));
#endif

#ifdef ZHLT_NFOPEN
	memset(&outNfHeader, 0, sizeof(outNfHeader));
#endif  // ZHLT_NFOPEN

	SwapBSPFile( true );

	header->version = LittleLong( BSPVERSION );

#ifdef ZHLT_PARANOIA_BSP
	extrahdr->id = LittleLong( IDEXTRAHEADER );
	extrahdr->version = LittleLong( EXTRA_VERSION );
#endif

#ifdef ZHLT_NFOPEN
	nfHeader->id = LittleLong(NFOPEN_EXTRAHEADER_ID);
	nfHeader->version = LittleLong(NFOPEN_EXTRAHEADER_VERSION);
#endif  // ZHLT_NFOPEN

	bspfile = SafeOpenWrite( filename );
	SafeWrite( bspfile, header, sizeof( dheader_t ));		// overwritten later

#ifdef ZHLT_PARANOIA_BSP
	SafeWrite( bspfile, extrahdr, sizeof( dextrahdr_t ));	// overwritten later
#endif

#ifdef ZHLT_NFOPEN
	const size_t nfHeaderOffset = ftell(bspfile);
	SafeWrite(bspfile, nfHeader, sizeof(*nfHeader)); // overwritten later
	WriteLumpDescriptor({NFOPEN_LUMP_CLIENTENTS, 0, 0}, bspfile); // overwritten later
#endif  // ZHLT_NFOPEN

	//       LUMP TYPE          DATA             LENGTH                                  HEADER  BSPFILE
	AddLump( LUMP_PLANES,       g_dplanes,       g_numplanes * sizeof( dplane_t ),       header, bspfile );
	AddLump( LUMP_LEAFS,        g_dleafs,        g_numleafs * sizeof( dleaf_t),          header, bspfile );
	AddLump( LUMP_VERTEXES,     g_dvertexes,     g_numvertexes * sizeof( dvertex_t ),    header, bspfile );
	AddLump( LUMP_NODES,        g_dnodes,        g_numnodes * sizeof( dnode_t ),         header, bspfile );
	AddLump( LUMP_TEXINFO,      g_texinfo,       g_numtexinfo * sizeof( texinfo_t ),     header, bspfile );
	AddLump( LUMP_FACES,        g_dfaces,        g_numfaces * sizeof( dface_t ),         header, bspfile );
#ifdef ZHLT_XASH2
	for( int hull = 1; hull < MAX_MAP_HULLS; hull++ )
	{
		int lump;

		switch( hull )
		{
		case 1: lump = LUMP_CLIPNODES; break;
		case 2: lump = LUMP_CLIPNODES2; break;
		case 3: lump = LUMP_CLIPNODES3; break;
		default:
			Error( "bad hull number %d", hull );
			break;
		}
		AddLump( lump, g_dclipnodes[hull - 1], g_numclipnodes[hull - 1] * sizeof( dclipnode_t ), header, bspfile );
	}
#else
	AddLump( LUMP_CLIPNODES,    g_dclipnodes,    g_numclipnodes * sizeof( dclipnode_t ), header, bspfile );
#endif
	AddLump( LUMP_MARKSURFACES, g_dmarksurfaces, g_nummarksurfaces * sizeof( short ),    header, bspfile );
	AddLump( LUMP_SURFEDGES,    g_dsurfedges,    g_numsurfedges * sizeof( int ),         header, bspfile );
	AddLump( LUMP_EDGES,        g_dedges,        g_numedges * sizeof( dedge_t ),         header, bspfile );
	AddLump( LUMP_MODELS,       g_dmodels,       g_nummodels * sizeof (dmodel_t ),       header, bspfile );

	AddLump( LUMP_LIGHTING,     g_dlightdata,    g_lightdatasize,                        header, bspfile );
	AddLump( LUMP_VISIBILITY,   g_dvisdata,      g_visdatasize,                          header, bspfile );
	AddLump( LUMP_ENTITIES,     g_dentdata,      g_entdatasize,                          header, bspfile );
	AddLump( LUMP_TEXTURES, writer.exportedData().data(), writer.exportedData().size(),  header, bspfile );

#ifdef ZHLT_PARANOIA_BSP
//    Log( "num extra faces %i, num faces %i, num worldlights %i, num ambient lights %i, num extra leafs %i, num leafs %i\n",
//    g_numfaces_extra, g_numfaces, g_numworldlights, g_numleaflights, g_numleafdata, g_numleafs );

	//            LUMP TYPE          DATA             LENGTH                                      EXTRAHDR  BSPFILE
	AddExtraLump( LUMP_VERTNORMALS,  g_dnormals,      g_numnormals * sizeof( dnormal_t ),         extrahdr, bspfile );
	AddExtraLump( LUMP_LIGHTVECS,    g_ddeluxdata,    g_deluxdatasize,                            extrahdr, bspfile );
	AddExtraLump( LUMP_CUBEMAPS,     g_dcubemaps,     g_numcubemaps * sizeof( dcubemap_t ),       extrahdr, bspfile );
	AddExtraLump( LUMP_FACEINFO,     g_dfaceinfo,     g_numfaceinfo * sizeof( dfaceinfo_t ),      extrahdr, bspfile );
	AddExtraLump( LUMP_LEAF_LIGHTING,g_dleaflights,   g_numleaflights * sizeof( dleafsample_t ),  extrahdr, bspfile );
	AddExtraLump( LUMP_WORLDLIGHTS,  g_dworldlights,  g_numworldlights * sizeof( dworldlight_t ), extrahdr, bspfile );
#endif  // ZHLT_PARANOIA_BSP

#ifdef ZHLT_NFOPEN
	const size_t beginClientEntData = ftell(bspfile) - nfHeaderOffset;
	size_t clientEntDataLength = 0;
	const void* clientEntData = CompileClientEntitiesLump(clientEntDataLength, true);
	WriteLumpData(clientEntData, static_cast<uint32_t>(clientEntDataLength), bspfile);
	++nfHeader->numLumps;
#endif  // ZHLT_NFOPEN

	fseek( bspfile, 0, SEEK_SET );
	SafeWrite( bspfile, header, sizeof( dheader_t ));

#ifdef ZHLT_PARANOIA_BSP
	SafeWrite( bspfile, extrahdr, sizeof( dextrahdr_t ));
#endif

#ifdef ZHLT_NFOPEN
	SafeWrite(bspfile, nfHeader, sizeof(*nfHeader));
	WriteLumpDescriptor({NFOPEN_LUMP_CLIENTENTS, static_cast<uint32_t>(beginClientEntData), static_cast<uint32_t>(clientEntDataLength)}, bspfile);
#endif  // ZHLT_NFOPEN

	fclose( bspfile );
}

#ifdef ZHLT_64BIT_FIX

#ifdef PLATFORM_CAN_CALC_EXTENT
// =====================================================================================
//  GetFaceExtents (with PLATFORM_CAN_CALC_EXTENT on)
// =====================================================================================
#ifdef SYSTEM_WIN32
#ifdef VERSION_32BIT
static void CorrectFPUPrecision ()
{
	// Can't compile these under MSVC 6.0
#if _MSC_VER > 1200
	unsigned int currentcontrol;
	if (_controlfp_s (&currentcontrol, 0, 0))
	{
		Warning ("Couldn't get FPU precision");
	}
	else
	{
		unsigned int val = (currentcontrol & _MCW_PC);
		if (val != _PC_53)
		{
			#ifdef _DEBUG
				Warning ("FPU precision is %s. Setting to %s.", (val == _PC_24? "24": val == _PC_64? "64": "invalid"), "53");
			#endif
			if (_controlfp_s (&currentcontrol, _PC_53, _MCW_PC)
				|| (currentcontrol & _MCW_PC) != _PC_53)
			{
				Warning ("Couldn't set FPU precision");
			}
		}
	}
#endif
}
#endif
#ifdef VERSION_64BIT
static void CorrectFPUPrecision ()
{
	// do nothing, because we use SSE registers
}
#endif
#endif

#ifdef SYSTEM_POSIX
static void CorrectFPUPrecision ()
{
	// just leave it to default and see if CalcFaceExtents_test gives us any error
}
#endif

float CalculatePointVecsProduct (const volatile float *point, const volatile float *vecs)
{
	volatile double val;
	volatile double tmp;

	val = (double)point[0] * (double)vecs[0]; // always do one operation at a time and save to memory
	tmp = (double)point[1] * (double)vecs[1];
	val = val + tmp;
	tmp = (double)point[2] * (double)vecs[2];
	val = val + tmp;
	val = val + (double)vecs[3];

	return (float)val;
}

bool CalcFaceExtents_test ()
{
	const int numtestcases = 6;
	volatile float testcases[numtestcases][8] = {
		{1, 1, 1, 1, 0.375 * DBL_EPSILON, 0.375 * DBL_EPSILON, -1, 0},
		{1, 1, 1, 0.375 * DBL_EPSILON, 0.375 * DBL_EPSILON, 1, -1, DBL_EPSILON},
		{DBL_EPSILON, DBL_EPSILON, 1, 0.375, 0.375, 1, -1, DBL_EPSILON},
		{1, 1, 1, 1, 1, 0.375 * FLT_EPSILON, -2, 0.375 * FLT_EPSILON},
		{1, 1, 1, 1, 0.375 * FLT_EPSILON, 1, -2, 0.375 * FLT_EPSILON},
		{1, 1, 1, 0.375 * FLT_EPSILON, 1, 1, -2, 0.375 * FLT_EPSILON}};
	bool ok;

	// If the test failed, please check:
	//   1. whether the calculation is performed on FPU
	//   2. whether the register precision is too low

	CorrectFPUPrecision ();

	ok = true;
	for (int i = 0; i < 6; i++)
	{
		float val = CalculatePointVecsProduct (&testcases[i][0], &testcases[i][3]);
		if (val != testcases[i][7])
		{
			Warning ("internal error: CalcFaceExtents_test failed on case %d (%.20f != %.20f).", i, val, testcases[i][7]);
			ok = false;
		}
	}
	return ok;
}

void GetFaceExtents (int facenum, int mins_out[2], int maxs_out[2])
{
	CorrectFPUPrecision ();

	dface_t *f;
	float mins[2], maxs[2], val;
	int i, j, e;
	dvertex_t *v;
	texinfo_t *tex;
	int bmins[2], bmaxs[2];

	f = &g_dfaces[facenum];

	mins[0] = mins[1] = 999999;
	maxs[0] = maxs[1] = -99999;

#ifdef ZHLT_EMBEDLIGHTMAP
	tex = &g_texinfo[ParseTexinfoForFace (f)];
#else
	tex = &g_texinfo[f->texinfo];
#endif
	int texture_step = GetTextureStep( tex );

	for (i = 0; i < f->numedges; i++)
	{
		e = g_dsurfedges[f->firstedge + i];
		if (e >= 0)
		{
			v = &g_dvertexes[g_dedges[e].v[0]];
		}
		else
		{
			v = &g_dvertexes[g_dedges[-e].v[1]];
		}
		for (j = 0; j < 2; j++)
		{
			// The old code: val = v->point[0] * tex->vecs[j][0] + v->point[1] * tex->vecs[j][1] + v->point[2] * tex->vecs[j][2] + tex->vecs[j][3];
			//   was meant to be compiled for x86 under MSVC (prior to VS 11), so the intermediate values were stored as 64-bit double by default.
			// The new code will produce the same result as the old code, but it's portable for different platforms.
			// See this article for details: Intermediate Floating-Point Precision by Bruce-Dawson http://www.altdevblogaday.com/2012/03/22/intermediate-floating-point-precision/

			// The essential reason for having this ugly code is to get exactly the same value as the counterpart of game engine.
			// The counterpart of game engine is the function CalcFaceExtents in HLSDK.
			// So we must also know how Valve compiles HLSDK. I think Valve compiles HLSDK with VC6.0 in the past.
			val = CalculatePointVecsProduct (v->point, tex->vecs[j]);
			if (val < mins[j])
			{
				mins[j] = val;
			}
			if (val > maxs[j])
			{
				maxs[j] = val;
			}
		}
	}

	for (i = 0; i < 2; i++)
	{
		bmins[i] = (int)floor (mins[i] / texture_step);
		bmaxs[i] = (int)ceil (maxs[i] / texture_step);
	}

	for (i = 0; i < 2; i++)
	{
		mins_out[i] = bmins[i];
		maxs_out[i] = bmaxs[i];
	}
}

// =====================================================================================
//  WriteExtentFile
// =====================================================================================
void WriteExtentFile (const char *const filename)
{
	FILE *f;
	f = fopen (filename, "w");
	if (!f)
	{
		Error ("Error opening %s: %s", filename, strerror(errno));
	}
	fprintf (f, "%i\n", g_numfaces);
	for (int i = 0; i < g_numfaces; i++)
	{
		int mins[2];
		int maxs[2];
		GetFaceExtents (i, mins, maxs);
		fprintf (f, "%i %i %i %i\n", mins[0], mins[1], maxs[0], maxs[1]);
	}
	fclose (f);
}

#else

typedef struct
{
	int mins[2];
	int maxs[2];
} faceextent_t;

bool g_faceextents_loaded = false;
faceextent_t g_faceextents[MAX_MAP_FACES]; //[g_numfaces]

// =====================================================================================
//  LoadExtentFile
// =====================================================================================
void LoadExtentFile (const char *const filename)
{
	FILE *f;
	f = fopen (filename, "r");
	if (!f)
	{
		Error ("Error opening %s: %s", filename, strerror(errno));
	}
	int count;
	int numfaces;
	count = fscanf (f, "%i\n", (int *)&numfaces);
	if (count != 1)
	{
		Error ("LoadExtentFile (line %i): scanf failure", 1);
	}
	if (numfaces != g_numfaces)
	{
		Error ("LoadExtentFile: numfaces(%i) doesn't match g_numfaces(%i)", numfaces, g_numfaces);
	}
	for (int i = 0; i < g_numfaces; i++)
	{
		faceextent_t *e = &g_faceextents[i];
		count = fscanf (f, "%i %i %i %i\n", (int *)&e->mins[0], (int *)&e->mins[1], (int *)&e->maxs[0], (int *)&e->maxs[1]);
		if (count != 4)
		{
			Error ("LoadExtentFile (line %i): scanf failure", i + 2);
		}
	}
	fclose (f);
	g_faceextents_loaded = true;
}

// =====================================================================================
//  GetFaceExtents (with PLATFORM_CAN_CALC_EXTENT off)
// =====================================================================================
// ZHLT_EMBEDLIGHTMAP: the result of "GetFaceExtents" and the values stored in ".ext" file should always be the original extents;
//                     the new extents of the "?_rad" textures should never appear ("?_rad" textures should be transparent to the tools).
//                     As a consequance, the reported AllocBlock might be inaccurate (usually falsely larger), but it accurately predicts the amount of AllocBlock after the embedded lightmaps are deleted.
void GetFaceExtents (int facenum, int mins_out[2], int maxs_out[2])
{
	if (!g_faceextents_loaded)
	{
		Error ("GetFaceExtents: internal error: extent file has not been loaded.");
	}

	faceextent_t *e = &g_faceextents[facenum];
	int i;

	for (i = 0; i < 2; i++)
	{
		mins_out[i] = e->mins[i];
		maxs_out[i] = e->maxs[i];
	}
}
#endif

#endif
//
// =====================================================================================
//
#ifdef ZHLT_CHART_AllocBlock
const int BLOCK_WIDTH = 128;
const int BLOCK_HEIGHT = 128;
typedef struct lightmapblock_s
{
	lightmapblock_s *next;
	bool used;
	int allocated[BLOCK_WIDTH];
}
lightmapblock_t;
void DoAllocBlock (lightmapblock_t *blocks, int w, int h)
{
	lightmapblock_t *block;
	// code from Quake
	int i, j;
	int best, best2;
	int x, y;
	if (w < 1 || h < 1)
	{
		Error ("DoAllocBlock: internal error.");
	}
	for (block = blocks; block; block = block->next)
	{
		best = BLOCK_HEIGHT;
		for (i = 0; i < BLOCK_WIDTH - w; i++)
		{
			best2 = 0;
			for (j = 0; j < w; j++)
			{
				if (block->allocated[i + j] >= best)
					break;
				if (block->allocated[i + j] > best2)
					best2 = block->allocated[i + j];
			}
			if (j == w)
			{
				x = i;
				y = best = best2;
			}
		}
		if (best + h <= BLOCK_HEIGHT)
		{
			block->used = true;
			for (i = 0; i < w; i++)
			{
				block->allocated[x + i] = best + h;
			}
			return;
		}
		if (!block->next)
		{ // need to allocate a new block
			if (!block->used)
			{
				Warning ("CountBlocks: invalid extents %dx%d", w, h);
				return;
			}
			block->next = (lightmapblock_t *)malloc (sizeof (lightmapblock_t));
			hlassume (block->next != NULL, assume_NoMemory);
			memset (block->next, 0, sizeof (lightmapblock_t));
		}
	}
}

int CountBlocks ()
{
#ifdef ZHLT_64BIT_FIX
#if !defined (PLATFORM_CAN_CALC_EXTENT) && !defined (HLRAD)
	return -1; // otherwise GetFaceExtents will error
#endif
#endif
	lightmapblock_t *blocks;
	blocks = (lightmapblock_t *)malloc (sizeof (lightmapblock_t));
	hlassume (blocks != NULL, assume_NoMemory);
	memset (blocks, 0, sizeof (lightmapblock_t));
	int k;
	for (k = 0; k < g_numfaces; k++)
	{
		dface_t *f = &g_dfaces[k];
#ifdef ZHLT_EMBEDLIGHTMAP
		const std::string texNameString = GetTextureByNumber(ParseTexinfoForFace(f));
#else
		const std::string texNameString = GetTextureByNumber(f->texinfo);
#endif
		const char *texname =  texNameString.c_str();
		if (!strncmp (texname, SPECIALTEX_SKY, sizeof(SPECIALTEX_SKY) - 1) //sky, no lightmap allocation.
			|| !strncmp (texname, SPECIALTEX_LIQUID_PREFIX, sizeof(SPECIALTEX_LIQUID_PREFIX) - 1)
			|| !strncasecmp (texname, SPECIALTEX_LIQUID_SUFFIX_WATER, sizeof(SPECIALTEX_LIQUID_SUFFIX_WATER) - 1)
			|| !strncasecmp (texname, SPECIALTEX_LIQUID_SUFFIX_LASER, sizeof(SPECIALTEX_LIQUID_SUFFIX_LASER) - 1) //water, no lightmap allocation.
#ifdef ZHLT_EMBEDLIGHTMAP
			|| (g_texinfo[ParseTexinfoForFace (f)].flags & TEX_SPECIAL) //aaatrigger, I don't know.
#else
			|| (g_texinfo[f->texinfo].flags & TEX_SPECIAL) //aaatrigger, I don't know.
#endif
			)
		{
			continue;
		}

		int texture_step = GetTextureStep( f );
		int max_extent = GetSurfaceExtent( f );
		int extents[2];
		vec3_t point;
		{
#ifdef ZHLT_64BIT_FIX
			int bmins[2];
			int bmaxs[2];
			int i;
			GetFaceExtents (k, bmins, bmaxs);
			for (i = 0; i < 2; i++)
			{
				extents[i] = (bmaxs[i] - bmins[i]) * texture_step;
			}

			VectorClear (point);
			if (f->numedges > 0)
			{
				int e = g_dsurfedges[f->firstedge];
				dvertex_t *v = &g_dvertexes[g_dedges[abs (e)].v[e >= 0? 0: 1]];
				VectorCopy (v->point, point);
			}
#else
			float mins[2], maxs[2];
			int bmins[2], bmaxs[2];
			texinfo_t *tex;
			tex = &g_texinfo[f->texinfo];
			mins[0] = mins[1] = 999999;
			maxs[0] = maxs[1] = -99999;
			VectorClear (point);
			int i;
			for (i = 0; i < f->numedges; i++)
			{
				int e;
				dvertex_t *v;
				int j;
				e = g_dsurfedges[f->firstedge + i];
				if (e >= 0)
				{
					v = &g_dvertexes[g_dedges[e].v[0]];
				}
				else
				{
					v = &g_dvertexes[g_dedges[-e].v[1]];
				}
				if (i == 0)
				{
					VectorCopy (v->point, point);
				}
				for (j = 0; j < 2; j++)
				{
					float val = v->point[0] * tex->vecs[j][0] + v->point[1] * tex->vecs[j][1]
						+ v->point[2] * tex->vecs[j][2] + tex->vecs[j][3];
					if (val < mins[j])
					{
						mins[j] = val;
					}
					if (val > maxs[j])
					{
						maxs[j] = val;
					}
				}
			}
			for (i = 0; i < 2; i++)
			{
				bmins[i] = floor (mins[i] / texture_step);
				bmaxs[i] = ceil (maxs[i] / texture_step);
				extents[i] = (bmaxs[i] - bmins[i]) * texture_step;
			}
#endif
		}
		if (extents[0] < 0 || extents[1] < 0 || extents[0] > qmax (512, max_extent * texture_step) || extents[1] > qmax (512, max_extent * texture_step))
			// the default restriction from the engine is 512, but place 'max (512, max_extent * texture_step)' here in case someone raise the limit
		{
			Warning ("Bad surface extents %d/%d at position (%.0f,%.0f,%.0f)", extents[0], extents[1], point[0], point[1], point[2]);
			continue;
		}
		DoAllocBlock (blocks, (extents[0] / texture_step) + 1, (extents[1] / texture_step) + 1);
	}
	int count = 0;
	lightmapblock_t *next;
	for (; blocks; blocks = next)
	{
		if (blocks->used)
		{
			count++;
		}
		next = blocks->next;
		free (blocks);
	}
	return count;
}
#endif

#define ENTRIES(a)		(sizeof(a)/sizeof(*(a)))
#define ENTRYSIZE(a)	(sizeof(*(a)))

// =====================================================================================
//  ArrayUsage
//      blah
// =====================================================================================
static int ArrayUsage( const char *szItem, const int items, const int maxitems, const int itemsize )
{
	float	percentage = maxitems ? items * 100.0 / maxitems : 0.0;

	Log("%-13s %7i/%-7i %8i/%-8i (%4.1f%%)\n", szItem, items, maxitems, items * itemsize, maxitems * itemsize, percentage );

	return items * itemsize;
}

// =====================================================================================
//  GlobUsage
//      pritn out global ussage line in chart
// =====================================================================================
static int GlobUsage( const char *szItem, const int itemstorage, const int maxstorage )
{
	float	percentage = maxstorage ? itemstorage * 100.0 / maxstorage : 0.0;

	Log("%-13s    [variable]   %8i/%-8i (%4.1f%%)\n", szItem, itemstorage, maxstorage, percentage );

	return itemstorage;
}


// =====================================================================================
//  PrintBSPFileSizes
//      Dumps info about current file
// =====================================================================================
void PrintBSPFileSizes( void )
{
	int	numtextures = g_TextureCollection.count();
	int	totalmemory = 0;
#ifdef ZHLT_CHART_AllocBlock
	int	numallocblocks = CountBlocks();
	int	maxallocblocks = 64;
#endif

	Log( "\n");
	Log( "Object names  Objects/Maxobjs  Memory / Maxmem  Fullness\n" );
	Log( "------------  ---------------  ---------------  --------\n" );

	totalmemory += ArrayUsage( "models", g_nummodels, ENTRIES( g_dmodels ), ENTRYSIZE( g_dmodels ));
	totalmemory += ArrayUsage( "planes", g_numplanes, MAX_MAP_PLANES, ENTRYSIZE( g_dplanes ));
	totalmemory += ArrayUsage( "vertexes", g_numvertexes, ENTRIES(g_dvertexes), ENTRYSIZE( g_dvertexes ));
	totalmemory += ArrayUsage( "nodes", g_numnodes, ENTRIES(g_dnodes), ENTRYSIZE( g_dnodes ));
#ifdef HLCSG_HLBSP_REDUCETEXTURE
	totalmemory += ArrayUsage( "texinfos", g_numtexinfo, MAX_MAP_TEXINFO, ENTRYSIZE( g_texinfo ));
#else
	totalmemory += ArrayUsage( "texinfos", g_numtexinfo, ENTRIES( g_texinfo ), ENTRYSIZE( g_texinfo ));
#endif
	totalmemory += ArrayUsage( "faces", g_numfaces, ENTRIES( g_dfaces ), ENTRYSIZE( g_dfaces ));
#ifdef ZHLT_WARNWORLDFACES
	totalmemory += ArrayUsage( "* worldfaces", (g_nummodels > 0 ? g_dmodels[0].numfaces: 0 ), MAX_MAP_WORLDFACES, 0 );
#endif
#ifdef ZHLT_XASH2
	for( int hull = 1; hull < MAX_MAP_HULLS; hull++ )
	{
		char buffer[32];
		sprintf( buffer, "clipnodes%d", hull );
		totalmemory += ArrayUsage( buffer, g_numclipnodes[hull - 1], ENTRIES( g_dclipnodes[hull - 1] ), ENTRYSIZE( g_dclipnodes[hull - 1] ));
	}
#else
	totalmemory += ArrayUsage( "clipnodes", g_numclipnodes, ENTRIES( g_dclipnodes ), ENTRYSIZE( g_dclipnodes ));
#endif
#ifdef ZHLT_MAX_MAP_LEAFS
	totalmemory += ArrayUsage( "leaves", g_numleafs, MAX_MAP_LEAFS, ENTRYSIZE( g_dleafs ));
	totalmemory += ArrayUsage( "* worldleaves", ( g_nummodels > 0 ? g_dmodels[0].visleafs : 0 ), MAX_MAP_LEAFS_ENGINE, 0 );
#else
	totalmemory += ArrayUsage( "leaves", g_numleafs, ENTRIES( g_dleafs ), ENTRYSIZE( g_dleafs ));
#endif
	totalmemory += ArrayUsage( "marksurfaces", g_nummarksurfaces, ENTRIES( g_dmarksurfaces ), ENTRYSIZE( g_dmarksurfaces ));
	totalmemory += ArrayUsage( "surfedges", g_numsurfedges, ENTRIES( g_dsurfedges ), ENTRYSIZE( g_dsurfedges ));
	totalmemory += ArrayUsage( "edges", g_numedges, ENTRIES( g_dedges ), ENTRYSIZE( g_dedges ));

	totalmemory += GlobUsage( "texdata", g_TextureCollection.totalBytesInUse(), g_max_map_miptex );
	totalmemory += GlobUsage( "lightdata", g_lightdatasize, g_max_map_lightdata );
	totalmemory += GlobUsage( "visdata", g_visdatasize, sizeof( g_dvisdata ));
	totalmemory += GlobUsage( "entdata", g_entdatasize, sizeof( g_dentdata ));
#ifdef ZHLT_CHART_AllocBlock
#ifdef ZHLT_64BIT_FIX
	if( numallocblocks == -1 )
	{
		Log( "* AllocBlock    [ not available to the " PLATFORM_VERSIONSTRING " version ]\n" );
	}
	else
	{
#endif
	totalmemory += ArrayUsage( "* AllocBlock", numallocblocks, maxallocblocks, 0 );
#ifdef ZHLT_64BIT_FIX
	}
#endif
#endif

#ifdef ZHLT_NFOPEN
	{
		size_t numBytes = 0;
		CompileClientEntitiesLump(numBytes);
		totalmemory += GlobUsage("cliententdata", numBytes, NFOPEN_CLIENT_ENT_LUMP_MAX_SIZE);
	}
#endif  // ZHLT_NFOPEN

#ifdef ZHLT_PARANOIA_BSP
	if( g_found_extradata )
	{
		totalmemory += ArrayUsage( "normals", g_numnormals, ENTRIES( g_dnormals ), ENTRYSIZE( g_dnormals ));
		totalmemory += GlobUsage( "deluxdata", g_deluxdatasize, g_max_map_lightdata );
		totalmemory += ArrayUsage( "cubemaps", g_numcubemaps, ENTRIES( g_dcubemaps ), ENTRYSIZE( g_dcubemaps ));
		totalmemory += ArrayUsage( "faceinfo", g_numfaceinfo, ENTRIES( g_dfaceinfo ), ENTRYSIZE( g_dfaceinfo ));
		totalmemory += ArrayUsage( "ambient cubes", g_numleaflights, ENTRIES( g_dleaflights ), ENTRYSIZE( g_dleaflights ));
		totalmemory += ArrayUsage( "direct lights", g_numworldlights, ENTRIES( g_dworldlights ), ENTRYSIZE( g_dworldlights ));
	}
#endif
	Log( "%i textures referenced\n", numtextures );

	Log( "=== Total BSP file data space used: %d bytes ===\n", totalmemory );
}


#ifdef ZHLT_EMBEDLIGHTMAP
// =====================================================================================
//  ParseImplicitTexinfoFromTexture
//      purpose: get the actual texinfo for a face. the tools shouldn't directly use f->texinfo after embedlightmap is done
// =====================================================================================
int ParseImplicitTexinfoFromTexture( int miptex )
{
	int texinfo;
	int numtextures = g_TextureCollection.count();
	int offset;
	int size;
	const miptex_t *mt;
	char name[16];

	if (miptex < 0 || miptex >= numtextures)
	{
		Warning ("ParseImplicitTexinfoFromTexture: internal error: invalid texture number %d.", miptex);
		return -1;
	}

	const MiptexWrapper* wrapper = g_TextureCollection.miptexAt(miptex);
	if ( !wrapper )
	{
		return -1;
	}

	safe_strncpy (name, wrapper->name(), sizeof(name));
	if (!(strlen (name) >= 6 && !strncasecmp (&name[1], "_rad", 4) && '0' <= name[5] && name[5] <= '9'))
	{
		return -1;
	}

	texinfo = atoi (&name[5]);
	if (texinfo < 0 || texinfo >= g_numtexinfo)
	{
		Warning ("Invalid index of original texinfo: %d parsed from texture name '%s'.", texinfo, name);
		return -1;
	}

	return texinfo;
}

int ParseTexinfoForFace (const dface_t *f)
{
	int texinfo;
	int miptex;
	int texinfo2;

	texinfo = f->texinfo;
	miptex = g_texinfo[texinfo].miptex;
	if (miptex != -1)
	{
		texinfo2 = ParseImplicitTexinfoFromTexture (miptex);
		if (texinfo2 != -1)
		{
			texinfo = texinfo2;
		}
	}

	return texinfo;
}

// =====================================================================================
//  DeleteEmbeddedLightmaps
//      removes all "?_rad*" textures that are created by hlrad
//      this function does nothing if the map has no textures with name "?_rad*"
// =====================================================================================
void DeleteEmbeddedLightmaps( void )
{
	int countrestoredfaces = 0;
	int countremovedtexinfos = 0;
	int countremovedtextures = 0;
	int i;
	int numtextures = g_TextureCollection.count();

	// Step 1: parse the original texinfo index stored in each "?_rad*" texture
	//         and restore the texinfo for the faces that have had their lightmap embedded

	for (i = 0; i < g_numfaces; i++)
	{
		dface_t *f = &g_dfaces[i];
		int texinfo;

		texinfo = ParseTexinfoForFace (f);
		if (texinfo != f->texinfo)
		{
			f->texinfo = texinfo;
			countrestoredfaces++;
		}
	}

	// Step 2: remove redundant texinfo
	{
		bool *texinfoused = (bool *)malloc (g_numtexinfo * sizeof (bool));
		hlassume (texinfoused != NULL, assume_NoMemory);

		for (i = 0; i < g_numtexinfo; i++)
		{
			texinfoused[i] = false;
		}
		for (i = 0; i < g_numfaces; i++)
		{
			dface_t *f = &g_dfaces[i];

			if (f->texinfo < 0 || f->texinfo >= g_numtexinfo)
			{
				continue;
			}
			texinfoused[f->texinfo] = true;
		}
		for (i = g_numtexinfo - 1; i > -1; i--)
		{
			texinfo_t *info = &g_texinfo[i];

			if (texinfoused[i])
			{
				break; // still used by a face; should not remove this texinfo
			}
			if (info->miptex < 0 || info->miptex >= numtextures)
			{
				break; // invalid; should not remove this texinfo
			}
			if (ParseImplicitTexinfoFromTexture (info->miptex) == -1)
			{
				break; // not added by hlrad; should not remove this texinfo
			}
			countremovedtexinfos++;
		}
		g_numtexinfo = i + 1; // shrink g_texinfo
		free (texinfoused);
	}

	// Step 3: remove redundant textures
	{
		int numremaining; // number of remaining textures
		bool *textureused = (bool *)malloc (numtextures * sizeof (bool));
		hlassume (textureused != NULL, assume_NoMemory);

		for (i = 0; i < numtextures; i++)
		{
			textureused[i] = false;
		}
		for (i = 0; i < g_numtexinfo; i++)
		{
			texinfo_t *info = &g_texinfo[i];

			if (info->miptex < 0 || info->miptex >= numtextures)
			{
				continue;
			}
			textureused[info->miptex] = true;
		}
		for (i = numtextures - 1; i > -1; i--)
		{
			if (textureused[i] || ParseImplicitTexinfoFromTexture (i) == -1)
			{
				break; // should not remove this texture
			}
			countremovedtextures++;
		}
		numremaining = i + 1;
		free (textureused);

		if (numremaining < numtextures)
		{
			// numremaining was calculated by walking back from the end of the texture list,
			// so we can just truncate to this new count.
			g_TextureCollection.truncate(numremaining);
		}
	}

	if (countrestoredfaces > 0 || countremovedtexinfos > 0 || countremovedtextures > 0)
	{
		Log ("DeleteEmbeddedLightmaps: restored %d faces, removed %d texinfos and %d textures.\n",
				countrestoredfaces, countremovedtexinfos, countremovedtextures);
	}
}
#endif

int GetSurfaceExtent( const dface_t *f )
{
#ifdef ZHLT_PARANOIA_BSP
	hlassert( f != NULL );

	texinfo_t *tex = &g_texinfo[f->texinfo];

	if( tex->faceinfo == -1 )
		return MAX_SURFACE_EXTENT;

	int max_extent = g_dfaceinfo[tex->faceinfo].max_extent;

	// check bounds
	if( max_extent >= MIN_CUSTOM_SURFACE_EXTENT && max_extent <= MAX_CUSTOM_SURFACE_EXTENT )
		return max_extent;
#endif
	return MAX_SURFACE_EXTENT;
}

int GetSurfaceExtent( const texinfo_t *tex )
{
#ifdef ZHLT_PARANOIA_BSP
	hlassert( tex != NULL );

	if( tex->faceinfo == -1 )
		return MAX_SURFACE_EXTENT;

	int max_extent = g_dfaceinfo[tex->faceinfo].max_extent;

	// check bounds
	if( max_extent >= MIN_CUSTOM_SURFACE_EXTENT && max_extent <= MAX_CUSTOM_SURFACE_EXTENT )
		return max_extent;
#endif
	return MAX_SURFACE_EXTENT;
}

int GetTextureStep( const dface_t *f )
{
#ifdef ZHLT_PARANOIA_BSP
	hlassert( f != NULL );

	texinfo_t *tex = &g_texinfo[f->texinfo];

	if( tex->faceinfo == -1 )
		return TEXTURE_STEP;

	int texture_step = g_dfaceinfo[tex->faceinfo].texture_step;

	// check bounds
	if( texture_step >= MIN_CUSTOM_TEXTURE_STEP && texture_step <= MAX_CUSTOM_TEXTURE_STEP )
		return texture_step;
#endif
	return TEXTURE_STEP;
}

int GetTextureStep( const texinfo_t *tex )
{
#ifdef ZHLT_PARANOIA_BSP
	hlassert( tex != NULL );

	if( tex->faceinfo == -1 )
		return TEXTURE_STEP;

	int texture_step = g_dfaceinfo[tex->faceinfo].texture_step;

	// check bounds
	if( texture_step >= MIN_CUSTOM_TEXTURE_STEP && texture_step <= MAX_CUSTOM_TEXTURE_STEP )
		return texture_step;
#endif
	return TEXTURE_STEP;
}

// =====================================================================================
//  ParseEpair
//      entity key/value pairs
// =====================================================================================
epair_t* ParseEpair( void )
{
    epair_t*        e;

    e = (epair_t*)Alloc(sizeof(epair_t));

    if (strlen(g_token) >= MAX_KEY - 1)
        Error("ParseEpair: Key token too long (%i > MAX_KEY)", (int)strlen(g_token));

    e->key = _strdup(g_token);
    GetToken(false);

    if (strlen(g_token) >= MAX_VAL - 1) //MAX_VALUE //vluzacn
        Error("ParseEpar: Value token too long (%i > MAX_VALUE)", (int)strlen(g_token));

    e->value = _strdup(g_token);

    return e;
}

/*
 * ================
 * ParseEntity
 * ================
 */

#ifdef ZHLT_INFO_COMPILE_PARAMETERS
// AJM: each tool should have its own version of GetParamsFromEnt which parseentity calls
extern void     GetParamsFromEnt(entity_t* mapent);
#endif

bool ParseEntity( void )
{
    epair_t*        e;
    entity_t*       mapent;

    if (!GetToken(true))
    {
        return false;
    }

    if (strcmp(g_token, "{"))
    {
        Error("ParseEntity: { not found");
    }

    if (g_numentities == MAX_MAP_ENTITIES)
    {
        Error("g_numentities == MAX_MAP_ENTITIES");
    }

    mapent = &g_entities[g_numentities];
    g_numentities++;

    while (1)
    {
        if (!GetToken(true))
        {
            Error("ParseEntity: EOF without closing brace");
        }
        if (!strcmp(g_token, "}"))
        {
            break;
        }
        e = ParseEpair();
        e->next = mapent->epairs;
        mapent->epairs = e;
    }

#ifdef ZHLT_INFO_COMPILE_PARAMETERS // AJM
    if (!strcmp(ValueForKey(mapent, "classname"), "info_compile_parameters"))
    {
        Log("Map entity info_compile_parameters detected, using compile settings\n");
        GetParamsFromEnt(mapent);
    }
#endif
#ifdef ZHLT_ENTITY_LIGHTSURFACE
	// ugly code
	if (!strncmp(ValueForKey (mapent, "classname"), "light", 5) && *ValueForKey (mapent, "_tex"))
	{
		SetKeyValue (mapent, "convertto", ValueForKey (mapent, "classname"));
		SetKeyValue (mapent, "classname", "light_surface");
	}
#endif
#ifdef ZHLT_ENTITY_LIGHTSHADOW
	if (!strcmp (ValueForKey (mapent, "convertfrom"), "light_shadow")
	#ifdef ZHLT_ENTITY_LIGHTBOUNCE
		|| !strcmp (ValueForKey (mapent, "convertfrom"), "light_bounce")
	#endif
		)
	{
		SetKeyValue (mapent, "convertto", ValueForKey (mapent, "classname"));
		SetKeyValue (mapent, "classname", ValueForKey (mapent, "convertfrom"));
		SetKeyValue (mapent, "convertfrom", "");
	}
#endif
#ifdef ZHLT_ENTITY_INFOSUNLIGHT
	if (!strcmp (ValueForKey (mapent, "classname"), "light_environment") &&
		!strcmp (ValueForKey (mapent, "convertfrom"), "info_sunlight"))
	{
		while (mapent->epairs)
		{
			DeleteKey (mapent, mapent->epairs->key);
		}
		memset (mapent, 0, sizeof(entity_t));
		g_numentities--;
		return true;
	}
	if (!strcmp (ValueForKey (mapent, "classname"), "light_environment") &&
		IntForKey (mapent, "_fake"))
	{
		SetKeyValue (mapent, "classname", "info_sunlight");
	}
#endif
	return true;
}

#if defined(ZHLT_NFOPEN)
void MakeClientEntity_Model(const entity_t& entity)
{
	if ( g_numclientmodels >= NFOPEN_CLIENT_ENT_MAX_MODELS )
	{
		Error("Exceeded max number of client models (%zu)\n", NFOPEN_CLIENT_ENT_MAX_MODELS);
		return;
	}

	dnfopenclientents_model_t* outModel = &g_clientmodels[g_numclientmodels++];

	VectorCopy(entity.origin, outModel->origin);
	safe_strncpy(outModel->model, ValueForKey(&entity, "model"), sizeof(outModel->model));

	if ( sscanf(ValueForKey(&entity, "angles"), "%f %f %f", &outModel->angles[0], &outModel->angles[1], &outModel->angles[2]) != 3 )
	{
		VectorClear(outModel->angles);
	}

	int fixedLightColour[3];

	if ( sscanf(ValueForKey(&entity, "fixedlight"), "%d %d %d", &fixedLightColour[0], &fixedLightColour[1], &fixedLightColour[2]) != 3 )
	{
		outModel->fixedLightColour[0] = static_cast<uint8_t>(bound(0, fixedLightColour[0], 255));
		outModel->fixedLightColour[1] = static_cast<uint8_t>(bound(0, fixedLightColour[1], 255));
		outModel->fixedLightColour[2] = static_cast<uint8_t>(bound(0, fixedLightColour[2], 255));
	}

	safe_strncpy(outModel->sequenceName, ValueForKey(&entity, "sequencename"), sizeof(outModel->sequenceName));

	int body = IntForKey(&entity, "body");

	if ( body < 0 )
	{
		body = 0;
	}

	outModel->body = body;

	int skin = IntForKey(&entity, "skin");

	if ( skin < 0 )
	{
		skin = 0;
	}

	outModel->skin = skin;

	if ( sscanf(ValueForKey(&entity, "lightingoffset"),
				"%f %f %f",
				&outModel->lightingOriginOffset[0],
				&outModel->lightingOriginOffset[1],
				&outModel->lightingOriginOffset[2]) != 3 )
	{
		VectorClear(outModel->lightingOriginOffset);
	}
}

void MakeClientEntity_Sound(const entity_t& entity)
{
	if ( g_numclientsounds >= NFOPEN_CLIENT_ENT_MAX_SOUNDS )
	{
		Error("Exceeded max number of client sounds (%zu)\n", NFOPEN_CLIENT_ENT_MAX_SOUNDS);
		return;
	}

	dnfopenclientents_sound_t* outSound = &g_clientsounds[g_numclientsounds++];

	VectorCopy(entity.origin, outSound->origin);
	safe_strncpy(outSound->sound, ValueForKey(&entity, "sound"), sizeof(outSound->sound));

	float minDelay = FloatForKey(&entity, "mindelay");

	if ( minDelay < 0.0f )
	{
		minDelay = 0.0f;
	}

	outSound->minRetriggerDelaySecs = minDelay;

	float maxDelay = FloatForKey(&entity, "maxdelay");

	if ( maxDelay < 0.0f )
	{
		maxDelay = 0.0f;
	}

	outSound->maxRetriggerDelaySecs = maxDelay;

	if ( ValueForKey(&entity, "volume", true) )
	{
		float volume = FloatForKey(&entity, "volume") * 255.0f;

		if ( volume < 0.0f )
		{
			volume = 0.0f;
		}
		else if ( volume > 255.0f )
		{
			volume = 255.0f;
		}

		outSound->volume = static_cast<uint8_t>(volume);
	}
	else
	{
		outSound->volume = 255;
	}
}
#endif  // defined(ZHLT_NFOPEN)

// =====================================================================================
//  ParseEntities
//      Parses the dentdata string into entities
// =====================================================================================
void ParseEntities( void )
{
	g_numentities = 0;
	ParseFromMemory( g_dentdata, g_entdatasize );
	while( ParseEntity( ));
}

// =====================================================================================
//  UnparseEntities
//      Generates the dentdata string from all the entities
// =====================================================================================
#ifdef ZHLT_ENTITY_INFOSUNLIGHT
int anglesforvector (float angles[3], const float vector[3])
{
	float z = vector[2], r = sqrt (vector[0] * vector[0] + vector[1] * vector[1]);
	float tmp;
	if (sqrt (z*z + r*r) < NORMAL_EPSILON)
	{
		return -1;
	}
	else
	{
		tmp = sqrt (z*z + r*r);
		z /= tmp, r /= tmp;
		if (r < NORMAL_EPSILON)
		{
			if (z < 0)
			{
				angles[0] = -90, angles[1] = 0;
			}
			else
			{
				angles[0] = 90, angles[1] = 0;
			}
		}
		else
		{
			angles[0] = atan (z / r) / Q_PI * 180;
			float x = vector[0], y = vector[1];
			tmp = sqrt (x*x + y*y);
			x /= tmp, y /= tmp;
			if (x < -1 + NORMAL_EPSILON)
			{
				angles[1] = -180;
			}
			else
			{
				if (y >= 0)
				{
					angles[1] = 2 * atan (y / (1+x)) / Q_PI * 180;
				}
				else
				{
					angles[1] = 2 * atan (y / (1+x)) / Q_PI * 180 + 360;
				}
			}
		}
	}
	angles[2] = 0;
	return 0;
}
#endif
void            UnparseEntities()
{
    char*           buf;
    char*           end;
    epair_t*        ep;
    char            line[MAXTOKEN];
    int             i;

    buf = g_dentdata;
    end = buf;
    *end = 0;

#ifdef ZHLT_ENTITY_INFOSUNLIGHT
	for (i = 0; i < g_numentities; i++)
	{
		entity_t *mapent = &g_entities[i];
		if (!strcmp (ValueForKey (mapent, "classname"), "info_sunlight") ||
			!strcmp (ValueForKey (mapent, "classname"), "light_environment") )
		{
			float vec[3] = {0,0,0};
			{
				sscanf (ValueForKey (mapent, "angles"), "%f %f %f", &vec[0], &vec[1], &vec[2]);
				float pitch = FloatForKey(mapent, "pitch");
				if (pitch)
					vec[0] = pitch;

				const char *target = ValueForKey (mapent, "target");
				if (target[0])
				{
					entity_t *targetent = FindTargetEntity (target);
					if (targetent)
					{
						float origin1[3] = {0,0,0}, origin2[3] = {0,0,0}, normal[3];
						sscanf (ValueForKey (mapent, "origin"), "%f %f %f", &origin1[0], &origin1[1], &origin1[2]);
						sscanf (ValueForKey (targetent, "origin"), "%f %f %f", &origin2[0], &origin2[1], &origin2[2]);
						VectorSubtract (origin2, origin1, normal);
						anglesforvector (vec, normal);
					}
				}
			}
			char stmp[1024];
			safe_snprintf (stmp, 1024, "%g %g %g", vec[0], vec[1], vec[2]);
			SetKeyValue (mapent, "angles", stmp);
			DeleteKey (mapent, "pitch");

			if (!strcmp (ValueForKey (mapent, "classname"), "info_sunlight"))
			{
				if (g_numentities == MAX_MAP_ENTITIES)
				{
					Error("g_numentities == MAX_MAP_ENTITIES");
				}
				entity_t *newent = &g_entities[g_numentities++];
				newent->epairs = mapent->epairs;
				SetKeyValue (newent, "classname", "light_environment");
				SetKeyValue (newent, "_fake", "1");
				mapent->epairs = NULL;
			}
		}
	}
#endif
#ifdef ZHLT_ENTITY_LIGHTSHADOW
    for (i = 0; i < g_numentities; i++)
	{
		entity_t *mapent = &g_entities[i];
		if (!strcmp (ValueForKey (mapent, "classname"), "light_shadow")
	#ifdef ZHLT_ENTITY_LIGHTBOUNCE
			|| !strcmp (ValueForKey (mapent, "classname"), "light_bounce")
	#endif
			)
		{
			SetKeyValue (mapent, "convertfrom", ValueForKey (mapent, "classname"));
			SetKeyValue (mapent, "classname", (*ValueForKey (mapent, "convertto")? ValueForKey (mapent, "convertto"): "light"));
			SetKeyValue (mapent, "convertto", "");
		}
	}
#endif
#ifdef ZHLT_ENTITY_LIGHTSURFACE
	// ugly code
	for (i = 0; i < g_numentities; i++)
	{
		entity_t *mapent = &g_entities[i];
		if (!strcmp (ValueForKey (mapent, "classname"), "light_surface"))
		{
			if (!*ValueForKey (mapent, "_tex"))
			{
				SetKeyValue (mapent, "_tex", "                ");
			}
			const char *newclassname = ValueForKey (mapent, "convertto");
			if (!*newclassname)
			{
				SetKeyValue (mapent, "classname", "light");
			}
			else if (strncmp (newclassname, "light", 5))
			{
				Error ("New classname for 'light_surface' should begin with 'light' not '%s'.\n", newclassname);
			}
			else
			{
				SetKeyValue (mapent, "classname", newclassname);
			}
			SetKeyValue (mapent, "convertto", "");
		}
	}
#endif
#ifdef HLCSG_OPTIMIZELIGHTENTITY
#ifdef HLCSG
	extern bool g_nolightopt;
	if (!g_nolightopt)
	{
		int i, j;
		int count = 0;
		bool *lightneedcompare = (bool *)malloc (g_numentities * sizeof (bool));
		hlassume (lightneedcompare != NULL, assume_NoMemory);
		memset (lightneedcompare, 0, g_numentities * sizeof(bool));
		for (i = g_numentities - 1; i > -1; i--)
		{
			entity_t *ent = &g_entities[i];
			const char *classname = ValueForKey (ent, "classname");
			const char *targetname = ValueForKey (ent, "targetname");
			int style = IntForKey (ent, "style");
			if (!targetname[0] || strcmp (classname, "light") && strcmp (classname, "light_spot") && strcmp (classname, "light_environment"))
				continue;
			for (j = i + 1; j < g_numentities; j++)
			{
				if (!lightneedcompare[j])
					continue;
				entity_t *ent2 = &g_entities[j];
				const char *targetname2 = ValueForKey (ent2, "targetname");
				int style2 = IntForKey (ent2, "style");
				if (style == style2 && !strcmp (targetname, targetname2))
					break;
			}
			if (j < g_numentities)
			{
				DeleteKey (ent, "targetname");
				count++;
			}
			else
			{
				lightneedcompare[i] = true;
			}
		}
		if (count > 0)
		{
			Log ("%d redundant named lights optimized.\n", count);
		}
		free (lightneedcompare);
	}
#endif
#endif

#if defined(ZHLT_NFOPEN)
	// Wow, this function is pretty horrible. Let's just stick
	// another entities pass in here, the code's too dirty to fix up.
	for (i = 0; i < g_numentities; i++)
	{
		entity_t* mapent = &g_entities[i];

		if ( strcmp(ValueForKey(mapent, "classname"), "client_model") == 0 )
		{
			MakeClientEntity_Model(*mapent);
			DeleteAllKeys(mapent);
		}
		else if ( strcmp(ValueForKey(mapent, "classname"), "client_sound") == 0 )
		{
			MakeClientEntity_Sound(*mapent);
			DeleteAllKeys(mapent);
		}
	}
#endif  // defined(ZHLT_NFOPEN)

    for (i = 0; i < g_numentities; i++)
    {
        ep = g_entities[i].epairs;
        if (!ep)
        {
            continue;                                      // ent got removed
        }

        strcat(end, "{\n");
        end += 2;

        for (ep = g_entities[i].epairs; ep; ep = ep->next)
        {
            sprintf(line, "\"%s\" \"%s\"\n", ep->key, ep->value);
            strcat(end, line);
            end += strlen(line);
        }
        strcat(end, "}\n");
        end += 2;

        if (end > buf + MAX_MAP_ENTSTRING)
        {
            Error("Entity text too long");
        }
    }
    g_entdatasize = end - buf + 1;
}

// =====================================================================================
//  SetKeyValue
//      makes a keyvalue
// =====================================================================================
#ifdef ZHLT_DELETEKEY
void			DeleteKey(entity_t* ent, const char* const key)
{
	epair_t **pep;
	for (pep = &ent->epairs; *pep; pep = &(*pep)->next)
	{
		if (!strcmp ((*pep)->key, key))
		{
			epair_t *ep = *pep;
			*pep = ep->next;
			Free(ep->key);
			Free(ep->value);
			Free(ep);
			return;
		}
	}
}

void DeleteAllKeys(entity_t* ent)
{
	epair_t* pair = ent->epairs;
	ent->epairs = nullptr;

	while ( pair )
	{
		epair_t* next = pair->next;

		Free(pair->key);
		Free(pair->value);
		Free(pair);

		pair = next;
	}
}
#endif
void            SetKeyValue(entity_t* ent, const char* const key, const char* const value)
{
    epair_t*        ep;

#ifdef ZHLT_DELETEKEY
	if (!value[0])
	{
		DeleteKey (ent, key);
		return;
	}
#endif
    for (ep = ent->epairs; ep; ep = ep->next)
    {
        if (!strcmp(ep->key, key))
        {
#ifdef ZHLT_DELETEKEY
			char *value2 = strdup (value);
			Free (ep->value);
			ep->value = value2;
#else
            Free(ep->value);
            ep->value = strdup(value);
#endif
            return;
        }
    }
    ep = (epair_t*)Alloc(sizeof(*ep));
    ep->next = ent->epairs;
    ent->epairs = ep;
    ep->key = strdup(key);
    ep->value = strdup(value);
}

// =====================================================================================
//  ValueForKey
//      returns the value for a passed entity and key
// =====================================================================================
const char* ValueForKey( const entity_t* const ent, const char* const key, bool check )
{
	epair_t*	ep;

	for( ep = ent->epairs; ep; ep = ep->next )
	{
		if (!strcmp(ep->key, key))
		{
			return ep->value;
		}
	}

	if( check )
		return NULL;
	return "";
}

// =====================================================================================
//  IntForKey
// =====================================================================================
int             IntForKey(const entity_t* const ent, const char* const key)
{
    return atoi(ValueForKey(ent, key));
}

// =====================================================================================
//  FloatForKey
// =====================================================================================
vec_t           FloatForKey(const entity_t* const ent, const char* const key)
{
    return atof(ValueForKey(ent, key));
}

// =====================================================================================
//  GetVectorForKey
//      returns value for key in vec[0-2]
// =====================================================================================
void            GetVectorForKey(const entity_t* const ent, const char* const key, vec3_t vec)
{
    const char*     k;
    double          v1, v2, v3;

    k = ValueForKey(ent, key);
    // scanf into doubles, then assign, so it is vec_t size independent
    v1 = v2 = v3 = 0;
    sscanf(k, "%lf %lf %lf", &v1, &v2, &v3);
    vec[0] = v1;
    vec[1] = v2;
    vec[2] = v3;
}

// =====================================================================================
//  FindTargetEntity
//
// =====================================================================================
entity_t *FindTargetEntity(const char* const target)
{
    int             i;
    const char*     n;

    for (i = 0; i < g_numentities; i++)
    {
        n = ValueForKey(&g_entities[i], "targetname");
        if (!strcmp(n, target))
        {
            return &g_entities[i];
        }
    }

    return NULL;
}

extern entity_t* FindEntityByClassname(const char* const classname)
{
    for (int entIndex = 0; entIndex < g_numentities; ++entIndex)
    {
        const char* classnameValue = ValueForKey(&g_entities[entIndex], "classname");

        if (strcmp(classname, classnameValue) == 0)
        {
            return &g_entities[entIndex];
        }
    }

    return NULL;
}


void dtexdata_init( void )
{
	g_TextureCollection.clear();
	g_dlightdata = (byte *)AllocBlock( g_max_map_lightdata );
	hlassume( g_dlightdata != NULL, assume_NoMemory );
	g_ddeluxdata = (byte *)AllocBlock( g_max_map_lightdata );
	hlassume( g_ddeluxdata != NULL, assume_NoMemory );
}

void CDECL dtexdata_free( void )
{
	g_TextureCollection.clear();
	FreeBlock( g_dlightdata );
	g_dlightdata = NULL;
	FreeBlock( g_ddeluxdata );
	g_ddeluxdata = NULL;
}

std::string GetTextureByNumber(int texturenumber)
{
    const texinfo_t* info = (texturenumber >= 0 && texturenumber < g_numtexinfo) ? &g_texinfo[texturenumber] : NULL;
    return info ? g_TextureCollection.itemName(info->miptex) : std::string();
}

// =====================================================================================
//  EntityForModel
//      returns entity addy for given modelnum
// =====================================================================================
entity_t*       EntityForModel(const int modnum)
{
    int             i;
    const char*     s;
    char            name[16];

    sprintf(name, "*%i", modnum);
    // search the entities for one using modnum
    for (i = 0; i < g_numentities; i++)
    {
        s = ValueForKey(&g_entities[i], "model");
        if (!strcmp(s, name))
        {
            return &g_entities[i];
        }
    }

    return &g_entities[0];
}
