#include "csg.h"
#include "texturecollection.h"
#include "texturecollectionreader.h"
#include "pngtexturepath.h"

#define MAXWADNAME 16

// =====================================================================================
//  CleanupName
// =====================================================================================
static void     CleanupName(const char* const in, char* out)
{
    int             i;

    for (i = 0; i < MAXWADNAME; i++)
    {
        if (!in[i])
        {
            break;
        }

        out[i] = toupper(in[i]);
    }

    for (; i < MAXWADNAME; i++)
    {
        out[i] = 0;
    }
}

// =====================================================================================
//  FindMiptex
//      Find and allocate a texture into the lump data
// =====================================================================================
static int FindMiptex(const char* const name)
{
    int32_t index = TextureDirectoryListing::INVALID_TEXTURE_INDEX;

    ThreadLock();
    index = g_TexDirListing.assignTextureIndex(name);
    ThreadUnlock();

    return index;
}

#ifdef ZHLT_PARANOIA_BSP
// =====================================================================================
//  FaceinfoForTexinfo
// =====================================================================================
short FaceinfoForTexinfo( const char *landname, const int in_texture_step, const int in_max_extent, const int groupid )
{
	byte	texture_step = bound( MIN_CUSTOM_TEXTURE_STEP, in_texture_step, MAX_CUSTOM_TEXTURE_STEP );
	byte	max_extent = bound( MIN_CUSTOM_SURFACE_EXTENT, in_max_extent, MAX_CUSTOM_SURFACE_EXTENT );
	short	i;

	ThreadLock();

	dfaceinfo_t *fi = g_dfaceinfo;

	for( i = 0; i < g_numfaceinfo; i++, fi++ )
	{
		if( stricmp( landname, fi->landname) != 0 )
		{
			continue;
		}

		if( fi->texture_step != texture_step )
		{
			continue;
		}

		if( fi->max_extent != max_extent )
		{
			continue;
		}

		if( fi->groupid != groupid )
		{
			continue;
		}

		ThreadUnlock();
		return i;
	}

	hlassume( g_numfaceinfo < MAX_MAP_FACEINFO, assume_MAX_FACEINFO );

	// allocate tne entry
	CleanupName( landname, fi->landname );
	fi->texture_step = texture_step;
	fi->max_extent = max_extent;
	fi->groupid = groupid;

	g_numfaceinfo++;
	ThreadUnlock();

	return i;
}
#endif

// =====================================================================================
//  WriteMiptex
// =====================================================================================
void WriteMiptex()
{
    std::vector<std::string> referencedTextures;
    g_TexDirListing.referencedTextureList(referencedTextures);

    if ( referencedTextures.size() < 1 )
    {
        // Should never happen if we have a valid map.
        Error("WriteMiptex: No textures referenced by map.");
    }

    // *Should* always be zero.
    const uint32_t baseIndex = g_TextureCollection.count();
    g_TextureCollection.allocateAndAppend(referencedTextures.size(), TextureCollection::ItemType::PngOnDisk);

    for ( uint32_t index = 0; index < referencedTextures.size(); ++index )
    {
        PNGTexturePath* const texture = g_TextureCollection.pngTextureAt(index);
        hlassert(texture);

        texture->setPath(referencedTextures[index]);
    }
}

//==========================================================================

// =====================================================================================
//  TexinfoForBrushTexture
// =====================================================================================
int TexinfoForBrushTexture(const plane_t* const plane, brush_texture_t* bt, const vec3_t origin
#ifdef ZHLT_HIDDENSOUNDTEXTURE
					, bool shouldhide
#endif
					)
{
    vec3_t          vecs[2];
    int             sv, tv;
    vec_t           ang, sinv, cosv;
    vec_t           ns, nt;
    texinfo_t       tx;
    texinfo_t*      tc;
    int             i, j, k;

#ifdef HLCSG_HLBSP_VOIDTEXINFO
    // Don't return a texinfo if the texture is null.
	if (!strncasecmp(bt->name, BRUSHKEY_NULL, sizeof(BRUSHKEY_NULL) - 1) || !strncasecmp(bt->name, SPECIALTEX_NODRAW, sizeof(SPECIALTEX_NODRAW) - 1))
	{
		return -1;
	}
#endif

    memset(&tx, 0, sizeof(tx));

#if !defined(HLCSG_CUSTOMHULL) && defined(HLCSG_PRECISIONCLIP)
    // If texture is bevel, set bevel flag and replace texture with null.
	if(!strncmp(bt->name,BRUSHKEY_BEVEL,sizeof(BRUSHKEY_BEVEL) - 1))
	{
		tx.flags |= TEX_BEVEL;
		safe_strncpy(bt->name,BRUSHKEY_NULL,sizeof(BRUSHKEY_NULL));
	}
#endif

	tx.miptex = FindMiptex(bt->name);

#ifdef ZHLT_PARANOIA_BSP
    tx.faceinfo = bt->faceinfo;
#endif

    // set the special flag
    if (strncasecmp(bt->name, SPECIALTEX_INTERNAL_PREFIX, sizeof(SPECIALTEX_INTERNAL_PREFIX) - 1) == 0
        || strncasecmp(bt->name, SPECIALTEX_SKY, sizeof(SPECIALTEX_SKY) - 1) == 0

// =====================================================================================
//Cpt_Andrew - Env_Sky Check
// =====================================================================================
        || strncasecmp(bt->name, SPECIALTEX_ENV_SKY, sizeof(SPECIALTEX_ENV_SKY) - 1) == 0
// =====================================================================================

#ifndef HLCSG_CUSTOMHULL
        || strncasecmp(bt->name, SPECIALTEX_CLIP, sizeof(SPECIALTEX_CLIP) - 1) == 0
        || strncasecmp(bt->name, SPECIALTEX_PLAYERCLIP, sizeof(SPECIALTEX_PLAYERCLIP) - 1) == 0
        || strncasecmp(bt->name, SPECIALTEX_NPCCLIP, sizeof(SPECIALTEX_NPCCLIP) - 1) == 0
        || strncasecmp(bt->name, SPECIALTEX_ENEMYCLIP, sizeof(SPECIALTEX_ENEMYCLIP) - 1) == 0
#endif
        || strncasecmp(bt->name, SPECIALTEX_ORIGIN, sizeof(SPECIALTEX_ORIGIN) - 1) == 0
#ifdef ZHLT_NULLTEX // AJM
        || strncasecmp(bt->name, SPECIALTEX_NULL, sizeof(SPECIALTEX_NULL) - 1) == 0
        || strncasecmp(bt->name, SPECIALTEX_NODRAW, sizeof(SPECIALTEX_NODRAW) - 1) == 0
#endif
        || strncasecmp(bt->name, SPECIALTEX_AAATRIGGER, sizeof(SPECIALTEX_AAATRIGGER) - 1) == 0
       )
    {
		// actually only 'sky' and 'aaatrigger' needs this. --vluzacn
        tx.flags |= TEX_SPECIAL;
    }
#ifdef ZHLT_HIDDENSOUNDTEXTURE
	if (shouldhide)
	{
		tx.flags |= TEX_SHOULDHIDE;
	}
#endif

    if (bt->txcommand)
    {
        memcpy(tx.vecs, bt->vects.quark.vects, sizeof(tx.vecs));
        if (origin[0] || origin[1] || origin[2])
        {
            tx.vecs[0][3] += DotProduct(origin, tx.vecs[0]);
            tx.vecs[1][3] += DotProduct(origin, tx.vecs[1]);
        }
    }
    else
    {
        // Do some absolute bullshit to calculate the texture vectors here.
        if ( g_nMapFileVersion > 0 && g_nMapFileVersion < 220)
        {
            Error("Map version %d is not supported.", g_nMapFileVersion);
#if 0
            TextureAxisFromPlane(plane, vecs[0], vecs[1]);
#endif
        }

        if (!bt->vects.valve.scale[0])
        {
            bt->vects.valve.scale[0] = 1;
        }
        if (!bt->vects.valve.scale[1])
        {
            bt->vects.valve.scale[1] = 1;
        }

        if ( g_nMapFileVersion > 0 && g_nMapFileVersion < 220 )
        {
            Error("Map version %d is not supported.", g_nMapFileVersion);
#if 0
            // rotate axis
            if (bt->vects.valve.rotate == 0)
            {
                sinv = 0;
                cosv = 1;
            }
            else if (bt->vects.valve.rotate == 90)
            {
                sinv = 1;
                cosv = 0;
            }
            else if (bt->vects.valve.rotate == 180)
            {
                sinv = 0;
                cosv = -1;
            }
            else if (bt->vects.valve.rotate == 270)
            {
                sinv = -1;
                cosv = 0;
            }
            else
            {
                ang = bt->vects.valve.rotate / 180 * Q_PI;
                sinv = sin(ang);
                cosv = cos(ang);
            }

            if (vecs[0][0])
            {
                sv = 0;
            }
            else if (vecs[0][1])
            {
                sv = 1;
            }
            else
            {
                sv = 2;
            }

            if (vecs[1][0])
            {
                tv = 0;
            }
            else if (vecs[1][1])
            {
                tv = 1;
            }
            else
            {
                tv = 2;
            }

            for (i = 0; i < 2; i++)
            {
                ns = cosv * vecs[i][sv] - sinv * vecs[i][tv];
                nt = sinv * vecs[i][sv] + cosv * vecs[i][tv];
                vecs[i][sv] = ns;
                vecs[i][tv] = nt;
            }

            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    tx.vecs[i][j] = vecs[i][j] / bt->vects.valve.scale[i];
                }
            }
#endif
        }
        else
        {
            vec_t           scale;

            scale = 1 / bt->vects.valve.scale[0];
            VectorScale(bt->vects.valve.UAxis, scale, tx.vecs[0]);

            scale = 1 / bt->vects.valve.scale[1];
            VectorScale(bt->vects.valve.VAxis, scale, tx.vecs[1]);
        }

        tx.vecs[0][3] = bt->vects.valve.shift[0] + DotProduct(origin, tx.vecs[0]);
        tx.vecs[1][3] = bt->vects.valve.shift[1] + DotProduct(origin, tx.vecs[1]);
    }

    //
    // find the g_texinfo
    //
    ThreadLock();
    tc = g_texinfo;
    for (i = 0; i < g_numtexinfo; i++, tc++)
    {
        if ( tc->miptex != tx.miptex )
        {
            continue;
        }

#ifdef ZHLT_PARANOIA_BSP
        if (tc->faceinfo != tx.faceinfo)
        {
            continue;
        }
#endif

        if (tc->flags != tx.flags)
        {
            continue;
        }
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < 4; k++)
            {
                if (tc->vecs[j][k] != tx.vecs[j][k])
                {
                    goto skip;
                }
            }
        }
        ThreadUnlock();
        return i;
skip:;
    }

#ifdef HLCSG_HLBSP_REDUCETEXTURE
    hlassume(g_numtexinfo < MAX_INTERNAL_MAP_TEXINFO, assume_MAX_MAP_TEXINFO);
#else
    hlassume(g_numtexinfo < MAX_MAP_TEXINFO, assume_MAX_MAP_TEXINFO);
#endif

    *tc = tx;
    g_numtexinfo++;
    ThreadUnlock();
    return i;
}

#ifdef HLCSG_HLBSP_VOIDTEXINFO
// Before WriteMiptex(), for each texinfo in g_texinfo, .miptex is a string rather than texture index, so this function should be used instead of GetTextureByNumber.
std::string GetTextureByNumber_CSG(int texturenumber)
{
    if ( texturenumber < 0 || texturenumber >= g_numtexinfo )
    {
        return std::string("");
    }

    const texinfo_t* texInfo = &g_texinfo[texturenumber];
	return g_TexDirListing.texturePath(texInfo->miptex);
}
#endif
