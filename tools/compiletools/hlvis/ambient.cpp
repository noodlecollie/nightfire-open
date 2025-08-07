#include "vis.h"
#include "stringlib.h"
#include "miptexwrapper.h"

/*

Some textures (sky, water, slime, lava) are considered ambient sound emiters.
Find an aproximate distance to the nearest emiter of each class for each leaf.

*/

#ifdef ZHLT_CALC_AMBIENT_SOUNDS

#define BOGUS_RANGE		65535

/*
====================
SurfaceBBox
====================
*/
void SurfaceBBox( dface_t *s, vec3_t mins, vec3_t maxs )
{
	int	e, vi;
	vec3_t	v;

	mins[0] = mins[1] = mins[2] =  BOGUS_RANGE;
	maxs[0] = maxs[1] = maxs[2] = -BOGUS_RANGE;

	for( int i = 0; i < s->numedges; i++ )
	{
		e = g_dsurfedges[s->firstedge + i];

		if( e >= 0 ) vi = g_dedges[e].v[0];
		else vi = g_dedges[-e].v[1];

		VectorCopy( g_dvertexes[vi].point, v );

		mins[0] = qmin( mins[0], v[0] );
		maxs[0] = qmax( maxs[0], v[0] );
		mins[1] = qmin( mins[1], v[1] );
		maxs[1] = qmax( maxs[1], v[1] );
		mins[2] = qmin( mins[2], v[2] );
		maxs[2] = qmax( maxs[2], v[2] );
	}
}

/*
====================
CalcAmbientSounds
====================
*/
void CalcAmbientSounds( void )
{
	int	i, j, k, l;
	dleaf_t	*leaf, *hit;
	byte	*vis;
	dface_t	*surf;
	vec3_t	mins, maxs;
	vec_t	d, maxd;
	int	ambient_type;
	texinfo_t	*info;
	vec_t	dists[NUM_AMBIENTS];
	vec_t	vol;

	Log( "---- Calc Ambient Sounds ----\n");

	for( i = 0; i< g_portalleafs; i++ )
	{
		leaf = &g_dleafs[i+1];

		// clear ambients
		for( j = 0; j < NUM_AMBIENTS; j++ )
			dists[j] = BOGUS_RANGE;

		vis = &g_uncompressed[i * g_bitbytes];

		for( j = 0; j< g_portalleafs; j++ )
		{
			if( !( vis[j>>3] & (1<<( j & 7 ))) )
				continue;

			// check this leaf for sound textures
			hit = &g_dleafs[j+1];

			for( k = 0; k < hit->nummarksurfaces; k++ )
			{
				surf = &g_dfaces[g_dmarksurfaces[hit->firstmarksurface + k]];
				info = &g_texinfo[surf->texinfo];

				const std::string nameString = g_TextureCollection.itemName(info->miptex);
				const char* name = nameString.c_str();

				if( !Q_strnicmp(name, SPECIALTEX_SKY, sizeof(SPECIALTEX_SKY) - 1) )
				{
					ambient_type = AMBIENT_SKY;
				}
				else if( Q_strnicmp(name, SPECIALTEX_LIQUID_PREFIX, sizeof(SPECIALTEX_LIQUID_PREFIX) - 1) == 0 )
				{
					if( !Q_strnicmp(name, SPECIALTEX_LIQUID_WATER, sizeof(SPECIALTEX_LIQUID_WATER) - 1) )
					{
						ambient_type = AMBIENT_WATER;
					}
					else if( !Q_strnicmp(name, SPECIALTEX_LIQUID_SLIME, sizeof(SPECIALTEX_LIQUID_SLIME) - 1) )
					{
						ambient_type = AMBIENT_SLIME;
					}
					else if( !Q_strnicmp(name, SPECIALTEX_LIQUID_LAVA, sizeof(SPECIALTEX_LIQUID_LAVA) - 1) )
					{
						ambient_type = AMBIENT_LAVA;
					}
					else
					{
						continue;
					}
				}
				else
				{
					continue;
				}

				// find distance from source leaf to polygon
				SurfaceBBox( surf, mins, maxs );
				maxd = 0;
				for( l = 0; l < 3; l++ )
				{
					if( mins[l] > leaf->maxs[l] )
						d = mins[l] - leaf->maxs[l];
					else if( maxs[l] < leaf->mins[l] )
						d = leaf->mins[l] - mins[l];
					else d = 0;

					if( d > maxd )
						maxd = d;
				}

				if( maxd < dists[ambient_type] )
					dists[ambient_type] = maxd;
			}
		}

		for( j = 0; j < NUM_AMBIENTS; j++ )
		{
			if( dists[j] < 100 )
			{
				vol = 1.0;
			}
			else
			{
				vol = 1.0 - dists[j] * 0.002;
				if( vol < 0 )
					vol = 0;
				else if( vol > 1.0 )
					vol = 1.0;
			}

			leaf->ambient_level[j] = vol * 255;
		}
	}
}

#endif
