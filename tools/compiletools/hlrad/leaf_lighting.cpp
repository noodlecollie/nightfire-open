#include "qrad.h"
#include "stringlib.h"

#ifdef ZHLT_PARANOIA_BSP

// the angle between consecutive g_anorms[] vectors is ~14.55 degrees
#define NUMVERTEXNORMALS	162
#define DIST_EPSILON	(0.03125)
#define MIN_LOCAL_SAMPLES	4
#define MAX_LOCAL_SAMPLES	16		// unsigned byte limit
#define MAX_SAMPLES		32		// enough
#define MAX_LEAF_PLANES	512		// QuArK cylinder :-)

// NFTODO: Make these two common with engine
#define GAMMA		( 2.2f )		// Valve Software gamma
#define INVGAMMA		( 1.0f / GAMMA )	// back to 1.0

vec_t g_anorms[NUMVERTEXNORMALS][3] =
{
#include "anorms.h"
};

static vec3_t g_BoxDirections[6] =
{
	{  1,  0,  0 },
	{ -1,  0,  0 },
	{  0,  1,  0 },
	{  0, -1,  0 },
	{  0,  0,  1 },
	{  0,  0, -1 },
};

// this stores each sample of the ambient lighting
struct ambientsample_t
{
	vec3_t cube[6];
	vec3_t pos;
};

struct ambientlist_t
{
	int		numSamples;
	ambientsample_t	*samples;
};

struct ambientlocallist_t
{
	ambientsample_t	samples[MAX_LOCAL_SAMPLES];
	int		numSamples;
};

struct leafplanes_t
{
	dplane_t		planes[MAX_LEAF_PLANES];
	int		numLeafPlanes;
};

typedef struct
{
	vec3_t	diffuse;
	vec3_t	average;
	float	fraction;
	dface_t	*surf;
	bool	hitsky;
} lightpoint_t;

ambientlist_t	g_leaf_samples[MAX_MAP_LEAFS];
static int	lineartoscreen[1024];	// linear (0..1) to gamma corrected vertex light (0..255)

static void BuildGammaTable( void )
{
	int	i, inf;
	float	g1, g3;
	float	g = bound( 0.5f, GAMMA, 3.0f );

	g = 1.0 / g;
	g1 = GAMMA * g;
	g3 = 0.125f;

	for( i = 0; i < 1024; i++ )
	{
		float	f;

		f = i / 1023.0;

		// shift up
		if( f <= g3 ) f = ( f / g3 ) * 0.125;
		else f = 0.125 + (( f - g3 ) / ( 1.0 - g3 )) * 0.875;

		// convert linear space to desired gamma space
		inf = 255 * pow( f, g );
		lineartoscreen[i] = bound( 0, inf, 255 );
	}
}

// converts 0..1 linear value to screen gamma (0..255)
int LinearToScreenGamma( float f ) { return lineartoscreen[bound( 0, (int)(f * 1023), 1023 )]; }

// TODO: it's CRAZY how much lighting code we share with the engine. It should all be shared code.
float Engine_WorldLightAngle( const dworldlight_t *wl, const vec3_t lnormal, const vec3_t snormal, const vec3_t delta )
{
	float dot, dot2;

	hlassert( wl->emittype == emit_surface );

	dot = DotProduct( snormal, delta );
	if (dot < 0)
		return 0;

	dot2 = -DotProduct (delta, lnormal);
	if (dot2 <= ON_EPSILON/10)
		return 0; // behind light surface

	return dot * dot2;
}

// TODO: it's CRAZY how much lighting code we share with the engine. It should all be shared code.
float Engine_WorldLightDistanceFalloff( const dworldlight_t *wl, const vec3_t delta )
{
	hlassert( wl->emittype == emit_surface );

	// Cull out stuff that's too far
	if (wl->radius != 0)
	{
		if ( DotProduct( delta, delta ) > (wl->radius * wl->radius))
			return 0.0f;
	}

	return InvRSquared(delta);
}

void AddEmitSurfaceLights( const vec3_t vStart, vec3_t lightBoxColor[6] )
{
	vec3_t	wlOrigin;

	for ( int iLight = 0; iLight < g_numworldlights; iLight++ )
	{
		dworldlight_t *wl = &g_dworldlights[iLight];

		// Should this light even go in the ambient cubes?
		if ( !( wl->flags & DWL_FLAGS_INAMBIENTCUBE ) )
			continue;

		hlassert( wl->emittype == emit_surface );

		VectorCopy( wl->origin, wlOrigin );	// short to float

		// Can this light see the point?
		if( TestLine ( vStart, wlOrigin ) != CONTENTS_EMPTY )
			continue;

		// Add this light's contribution.
		vec3_t vDelta, vDeltaNorm;
		VectorSubtract( wlOrigin, vStart, vDelta );
		float flDistanceScale = Engine_WorldLightDistanceFalloff( wl, vDelta );

		VectorCopy( vDelta, vDeltaNorm );
		VectorNormalize( vDeltaNorm );

		float flAngleScale = Engine_WorldLightAngle( wl, wl->normal, vDeltaNorm, vDeltaNorm );

//		if( g_trace_fraction != 1.0f ) Log( "trace fraction %g\n", g_trace_fraction );
		float ratio = flDistanceScale * flAngleScale * g_trace_fraction;
		if ( ratio == 0 ) continue;

		for ( int i = 0; i < 6; i++ )
		{
			float t = DotProduct( g_BoxDirections[i], vDeltaNorm );

			if ( t > 0 )
			{
				VectorMA( lightBoxColor[i], (t * ratio), wl->intensity, lightBoxColor[i] );
			}
		}
	}
}

static void CalcFaceExtents( dface_t *surf, float texturemins[2], float extents[2] )
{
	float	mins[2], maxs[2], val;
	int	bmins[2], bmaxs[2];
	int	i, j, e;
	texinfo_t	*tex;
	dvertex_t	*v;

	tex = g_texinfo + surf->texinfo;
	int texture_step = GetTextureStep( tex );

	mins[0] = mins[1] = 999999;
	maxs[0] = maxs[1] = -999999;

	for( i = 0; i < surf->numedges; i++ )
	{
		e = g_dsurfedges[surf->firstedge + i];

		if( e >= 0 ) v = &g_dvertexes[g_dedges[e].v[0]];
		else v = &g_dvertexes[g_dedges[-e].v[1]];

		for( j = 0; j < 2; j++ )
		{
			val = DotProduct( v->point, tex->vecs[j] ) + tex->vecs[j][3];
			if( val < mins[j] ) mins[j] = val;
			if( val > maxs[j] ) maxs[j] = val;
		}
	}

	for( i = 0; i < 2; i++ )
	{
		bmins[i] = floor( mins[i] / texture_step );
		bmaxs[i] = ceil( maxs[i] / texture_step );

		texturemins[i] = bmins[i] * texture_step;
		extents[i] = (bmaxs[i] - bmins[i]) * texture_step;

		if( !( tex->flags & TEX_SPECIAL ) && extents[i] > 4096 )
			Warning( "Bad surface extents %f\n", extents[i] );
	}
}

static bool R_GetDirectLightFromSurface( dface_t *surf, const vec3_t point, lightpoint_t *info )
{
	int	map, size, s, t;
	int	MAXSAMPLES = MAXLIGHTMAPS;
	float	texturemins[2], extents[2];
	byte	*lm;

	// recalc face extents here
	CalcFaceExtents( surf, texturemins, extents );
	int texture_step = GetTextureStep( surf );

	texinfo_t *tex = g_texinfo + surf->texinfo;

	s = DotProduct( point, tex->vecs[0] ) + tex->vecs[0][3] - texturemins[0];
	t = DotProduct( point, tex->vecs[1] ) + tex->vecs[1][3] - texturemins[1];

	if(( s < 0 || s > extents[0] ) || ( t < 0 || t > extents[1] ))
		return false;

	if( tex->flags & TEX_SPECIAL )
	{
		const std::string texNameString = GetTextureByNumber(surf->texinfo);
		const char *texname =  texNameString.c_str();

		if( !Q_strnicmp( texname, SPECIALTEX_SKY, sizeof(SPECIALTEX_SKY) - 1 ))
		{
			info->hitsky = true;
		}

		return false; // no lightmaps
	}

	if( surf->lightofs == -1 )
		return true;

	s /= texture_step;
	t /= texture_step;

	byte *samples = g_dlightdata + (unsigned int)surf->lightofs;
	lm = samples + (unsigned int)( t * (( extents[0] / texture_step ) + 1 ) + s );
	size = (( extents[0] / texture_step ) + 1 ) * (( extents[1] / texture_step ) + 1 );

	VectorClear( info->diffuse );

	for( map = 0; map < MAXSAMPLES && surf->styles[map] != 255; map++ )
	{
		info->diffuse[0] += (float)lm[0] * 264.0f;
		info->diffuse[1] += (float)lm[1] * 264.0f;
		info->diffuse[2] += (float)lm[2] * 264.0f;
		lm += size; // skip to next lightmap
	}

	info->diffuse[0] = qmin( info->diffuse[0] * (1.0f / 128.0f), 255.0f ) * (1.0f / 255.0f);
	info->diffuse[1] = qmin( info->diffuse[1] * (1.0f / 128.0f), 255.0f ) * (1.0f / 255.0f);
	info->diffuse[2] = qmin( info->diffuse[2] * (1.0f / 128.0f), 255.0f ) * (1.0f / 255.0f);
	VectorClear( info->average );
	lm = samples;

	// also collect the average value
	for( map = 0; map < MAXSAMPLES && surf->styles[map] != 255; map++ )
	{
		for( int i = 0; i < size; i++, lm += 3 )
		{
			info->average[0] += (float)lm[0] * 264.0f;
			info->average[1] += (float)lm[1] * 264.0f;
			info->average[2] += (float)lm[2] * 264.0f;
		}

		VectorScale( info->average, ( 1.0f / (float)size ), info->average );
	}

	info->average[0] = qmin( info->average[0] * (1.0f / 128.0f), 255.0f ) * (1.0f / 255.0f);
	info->average[1] = qmin( info->average[1] * (1.0f / 128.0f), 255.0f ) * (1.0f / 255.0f);
	info->average[2] = qmin( info->average[2] * (1.0f / 128.0f), 255.0f ) * (1.0f / 255.0f);
	info->surf = surf;

	return true;
}

/*
=================
R_RecursiveLightPoint
=================
*/
static bool R_RecursiveLightPoint( const int nodenum, float p1f, float p2f, const vec3_t start, const vec3_t end, lightpoint_t *info )
{
	vec3_t	mid;

	// hit a leaf
	if( nodenum < 0 ) return false;
	dnode_t *node = g_dnodes + nodenum;
	dplane_t *plane = g_dplanes + node->planenum;

	// calculate mid point
	float front = PlaneDiff( start, plane );
	float back = PlaneDiff( end, plane );

	int side = front < 0.0f;
	if(( back < 0.0f ) == side )
		return R_RecursiveLightPoint( node->children[side], p1f, p2f, start, end, info );

	float frac = front / ( front - back );

	float midf = p1f + ( p2f - p1f ) * frac;
	VectorLerp( start, frac, end, mid );

	// co down front side
	if( R_RecursiveLightPoint( node->children[side], p1f, midf, start, mid, info ))
		return true; // hit something

	if(( back < 0.0f ) == side )
		return false;// didn't hit anything

	// check for impact on this node
	for( int i = 0; i < node->numfaces; i++ )
	{
		dface_t *surf = g_dfaces + node->firstface + i;

		if( R_GetDirectLightFromSurface( surf, mid, info ))
		{
			info->fraction = midf;
			return true;
		}
	}

	// go down back side
	return R_RecursiveLightPoint( node->children[!side], midf, p2f, mid, end, info );
}

//-----------------------------------------------------------------------------
// Finds ambient sky lights
//-----------------------------------------------------------------------------
static dworldlight_t* FindAmbientSkyLight( void )
{
	static dworldlight_t *s_pCachedSkylight = NULL;

	// Don't keep searching for the same light.
	if ( !s_pCachedSkylight )
	{
		// find any ambient lights
		for ( int iLight = 0; iLight < g_numworldlights; iLight++ )
		{
			dworldlight_t *wl = &g_dworldlights[iLight];

			if (wl->emittype == emit_skylight)
			{
				s_pCachedSkylight = wl;
				break;
			}
		}
	}

	return s_pCachedSkylight;
}

static void ComputeLightmapColorFromPoint( lightpoint_t *info, dworldlight_t* pSkylight, float scale, vec3_t radcolor, bool average )
{
	vec3_t	color;

	if( !info->surf && info->hitsky )
	{
		if( pSkylight )
		{
			VectorScale( pSkylight->intensity, scale * 0.5f, color );
			VectorScale( pSkylight->intensity, (1.0f/255.0f), color );
			VectorAdd( radcolor, color, radcolor );
		}
		return;
	}

	if( info->surf != NULL )
	{
		texinfo_t *pTexInfo = &g_texinfo[info->surf->texinfo];

		if( average )
		{
			VectorScale( info->average, scale, color );
		}
		else
		{
			VectorScale( info->diffuse, scale, color );
		}

		VectorAdd( radcolor, color, radcolor );
	}
}

//-----------------------------------------------------------------------------
// Computes ambient lighting along a specified ray.
// Ray represents a cone, tanTheta is the tan of the inner cone angle
//-----------------------------------------------------------------------------
void CalcRayAmbientLighting( const vec3_t vStart, const vec3_t vEnd, dworldlight_t *pSkyLight, float tanTheta, vec3_t radcolor )
{
	lightpoint_t info;

	memset( &info, 0, sizeof( lightpoint_t ));
	info.fraction = 1.0f;
	VectorClear( radcolor );

	// Now that we've got a ray, see what surface we've hit
	R_RecursiveLightPoint( 0, 0.0f, 1.0f, vStart, vEnd, &info );

	vec3_t vDelta;

	VectorSubtract( vEnd, vStart, vDelta );

	// compute the approximate radius of a circle centered around the intersection point
	float dist = VectorLength( vDelta ) * tanTheta * info.fraction;

	// until 20" we use the point sample, then blend in the average until we're covering 40"
	// This is attempting to model the ray as a cone - in the ideal case we'd simply sample all
	// luxels in the intersection of the cone with the surface.  Since we don't have surface
	// neighbor information computed we'll just approximate that sampling with a blend between
	// a point sample and the face average.
	// This yields results that are similar in that aliasing is reduced at distance while
	// point samples provide accuracy for intersections with near geometry
	float scaleAvg = RemapValClamped( dist, 20, 40, 0.0f, 1.0f );

	if( !info.surf )
	{
		// don't have luxel UV, so just use average sample
		scaleAvg = 1.0;
	}

	float scaleSample = 1.0f - scaleAvg;

	if( scaleAvg != 0 )
	{
		ComputeLightmapColorFromPoint( &info, pSkyLight, scaleAvg, radcolor, true );
	}

	if( scaleSample != 0 )
	{
		ComputeLightmapColorFromPoint( &info, pSkyLight, scaleSample, radcolor, false );
	}
}

void ComputeAmbientFromSphericalSamples( const vec3_t p1, vec3_t lightBoxColor[6] )
{
	// Figure out the color that rays hit when shot out from this position.
	float tanTheta = tan( VERTEXNORMAL_CONE_INNER_ANGLE );
	dworldlight_t *pSkyLight = FindAmbientSkyLight();
	vec3_t radcolor[NUMVERTEXNORMALS], p2;

	for ( int i = 0; i < NUMVERTEXNORMALS; i++ )
	{
		VectorMA( p1, (65536.0 * 1.74), g_anorms[i], p2 );

		// Now that we've got a ray, see what surface we've hit
		CalcRayAmbientLighting( p1, p2, pSkyLight, tanTheta, radcolor[i] );
	}

	// accumulate samples into radiant box
	for ( int j = 0; j < 6; j++ )
	{
		float t = 0.0f;

		VectorClear( lightBoxColor[j] );

		for( int i = 0; i < NUMVERTEXNORMALS; i++ )
		{
			float c = DotProduct( g_anorms[i], g_BoxDirections[j] );

			if( c > 0.0f )
			{
				VectorMA( lightBoxColor[j], c, radcolor[i], lightBoxColor[j] );
				t += c;
			}
		}

		VectorScale( lightBoxColor[j], ( 1.0 / t ), lightBoxColor[j] );
	}

	// Now add direct light from the emit_surface lights. These go in the ambient cube because
	// there are a ton of them and they are often so dim that they get filtered out by r_worldlightmin.
	AddEmitSurfaceLights( p1, lightBoxColor );
}

bool IsLeafAmbientSurfaceLight( dworldlight_t *wl )
{
	vec3_t dist = { 0.0, 0.0, 512.0 };
	static const float g_flWorldLightMinEmitSurface = 0.005f;
	static const float g_flWorldLightMinEmitSurfaceDistanceRatio = ( InvRSquared( dist ) );

	if ( wl->emittype != emit_surface )
		return false;

	if ( wl->style != 0 )
		return false;

	float intensity = qmax( wl->intensity[0], qmax( wl->intensity[1], wl->intensity[2] ));

	return (intensity * g_flWorldLightMinEmitSurfaceDistanceRatio) < g_flWorldLightMinEmitSurface;
}

static dvertex_t *GetFaceVertex( dface_t *surf, int vertexNum )
{
	dvertex_t *v;

	int e = g_dsurfedges[surf->firstedge + vertexNum];
	if( e >= 0 ) v = &g_dvertexes[g_dedges[e].v[0]];
	else v = &g_dvertexes[g_dedges[-e].v[1]];

	return v;
}

/*
=================
TraceAgainstTriangle

Ray-triangle intersection as per
http://geometryalgorithms.com/Archive/algorithm_0105/algorithm_0105.htm
(original paper by Dan Sunday)
=================
*/
void TraceAgainstTriangle( const vec3_t start, const vec3_t end, const vec3_t a, const vec3_t b, const vec3_t c, float *pFrac, vec3_t pNormal )
{
	float uu, uv, vv, wu, wv, s, t;
	vec3_t u, v, w, n, p;
	float d1, d2, d, frac;
	vec3_t p0, p1, p2;

	VectorCopy( start, p1 );
	VectorCopy( end, p2 );
	VectorCopy( a, p0 );

	// calculate two mostly perpendicular edge directions
	VectorSubtract( b, p0, u );
	VectorSubtract( c, p0, v );

	// we have two edge directions, we can calculate the normal
	CrossProduct( v, u, n );

	if( VectorCompare( n, vec3_origin ))
		return; // degenerate triangle

	VectorSubtract( p2, p1, p );
	VectorSubtract( p1, p0, w );

	d1 = -DotProduct( n, w );
	d2 = DotProduct( n, p );

	if( fabs( d2 ) < 0.0001 )
		return;

	// get intersect point of ray with triangle plane
	frac = d1 / d2;

	if( frac <= 0 ) return;
	if( frac >= *pFrac )
		return; // we have hit something earlier

	// calculate the impact point
	p[0] = p1[0] + (p2[0] - p1[0]) * frac;
	p[1] = p1[1] + (p2[1] - p1[1]) * frac;
	p[2] = p1[2] + (p2[2] - p1[2]) * frac;

	// does p lie inside triangle?
	uu = DotProduct( u, u );
	uv = DotProduct( u, v );
	vv = DotProduct( v, v );

	VectorSubtract( p, p0, w );
	wu = DotProduct( w, u );
	wv = DotProduct( w, v );
	d = uv * uv - uu * vv;

	// get and test parametric coords
	s = (uv * wv - vv * wu) / d;
	if( s < 0.0f || s > 1.0f )
		return; // p is outside

	t = (uv * wu - uu * wv) / d;
	if( t < 0.0 || (s + t) > 1.0 )
		return; // p is outside

	VectorCopy( n, pNormal );
	*pFrac = frac;
}

/*
=================
R_TraceAgainstSurface
=================
*/
static int TraceAgainstFace( const vec3_t start, const vec3_t end, dface_t *surf, float *pFraction, vec3_t pNormal )
{
	float old_frac = *pFraction;

	// clip each triangle individually
	for( int i = 0; i < surf->numedges - 2; i += 3 )
	{
		dvertex_t *v0, *v1, *v2;

		v0 = GetFaceVertex( surf, 0 );
		v1 = GetFaceVertex( surf, i + 1 );
		v2 = GetFaceVertex( surf, i + 2 );

		TraceAgainstTriangle( start, end, v0->point, v1->point, v2->point, pFraction, pNormal );

		if( old_frac > *pFraction )
		{
			dplane_t *plane = g_dplanes + surf->planenum;

			// flip normal is we are on the backside (does it really happen?)...
			if( DotProduct( pNormal, plane->normal ) < 0.0f )
				VectorInverse( pNormal );
			return 1;
		}
	}

	return 0;
}

/*
=================
CastRayInLeaf
=================
*/
bool CastRayInLeaf( const vec3_t start, const vec3_t end, int leafIndex, float *pFraction, vec3_t pNormal )
{
	dleaf_t *pLeaf = g_dleafs + leafIndex;

	VectorClear( pNormal );
	*pFraction = 1.0f;

	if( pLeaf->contents == CONTENTS_SOLID )
	{
		*pFraction = 0.0f;
		return 1;	// hit a solid leaf
          }

	for( int i = 0; i < pLeaf->nummarksurfaces; i++ )
	{
		dface_t *surf = &g_dfaces[g_dmarksurfaces[pLeaf->firstmarksurface + i]];

		TraceAgainstFace( start, end, surf, pFraction, pNormal );
	}

	*pFraction = bound( 0.0f, *pFraction, 1.0f );

	return 0;
}

// Generate a random point in the leaf's bounding volume
// reject any points that aren't actually in the leaf
// do a couple of tracing heuristics to eliminate points that are inside detail brushes
// or underneath displacement surfaces in the leaf
// return once we have a valid point, use the center if one can't be computed quickly
void GenerateLeafSamplePosition( int leafIndex, ambientlocallist_t *list, const leafplanes_t *leafPlanes, vec3_t samplePosition )
{
	dleaf_t *pLeaf = g_dleafs + leafIndex;
	vec3_t vCenter;

	float dx = float(pLeaf->maxs[0] - pLeaf->mins[0]);
	float dy = float(pLeaf->maxs[1] - pLeaf->mins[1]);
	float dz = float(pLeaf->maxs[2] - pLeaf->mins[2]);
	bool bValid = false;

	VectorAverage( pLeaf->mins, pLeaf->maxs, vCenter );

	// place first sample always at center to leaf
	if( list->numSamples == 0 )
	{
		VectorCopy( vCenter, samplePosition );
		return;
	}

	// lock so random float will working properly
	ThreadLock();

	for ( int i = 0; i < 1024 && !bValid; i++ )
	{
		samplePosition[0] = float(pLeaf->mins[0]) + RandomFloat( 0.0, dx );
		samplePosition[1] = float(pLeaf->mins[1]) + RandomFloat( 0.0, dy );
		samplePosition[2] = float(pLeaf->mins[2]) + RandomFloat( 0.0, dz );
		vec3_t vDiff;

		int l;
		for( l = 0; l < list->numSamples; l++ )
		{
			VectorSubtract( samplePosition, list->samples[l].pos, vDiff );
			float flLength = VectorLength( vDiff );
			if( flLength < 32.0f ) break;	// too tight
		}

		if( l != list->numSamples )
			continue;

		bValid = true;

		for ( int k = leafPlanes->numLeafPlanes - 1; --k >= 0 && bValid; )
		{
			float d = DotProduct( leafPlanes->planes[k].normal, samplePosition ) - leafPlanes->planes[k].dist;

			if ( d < DIST_EPSILON )
			{
				// not inside the leaf, try again
				bValid = false;
				break;
			}
		}

		if ( !bValid ) continue;

		for ( int j = 0; j < 6; j++ )
		{
			vec3_t start, normal;

			VectorCopy( samplePosition, start );
			int axis = j % 3;
			start[axis] = (j < 3) ? pLeaf->mins[axis] : pLeaf->maxs[axis];

			float t;
			CastRayInLeaf( samplePosition, start, leafIndex, &t, normal );

			if ( t == 0.0f )
			{
				// inside a func_detail, try again.
				bValid = false;
				break;
			}

			if ( t != 1.0f )
			{
				vec3_t delta;
				VectorSubtract( start, samplePosition, delta );

				if ( DotProduct(delta, normal) > 0.0f )
				{
					// hit backside of displacement, try again.
					bValid = false;
					break;
				}
			}
		}
	}

	ThreadUnlock();

	if ( !bValid )
	{
		// didn't generate a valid sample point, just use the center of the leaf bbox
		VectorAverage( pLeaf->mins, pLeaf->maxs, samplePosition );
	}
}

// gets a list of the planes pointing into a leaf
void GetLeafBoundaryPlanes( leafplanes_t *list, int leafIndex )
{
	int nodeIndex = leafparents[leafIndex];
	int child = -(leafIndex + 1);

	while ( nodeIndex >= 0 )
	{
		dnode_t *pNode = g_dnodes + nodeIndex;
		dplane_t *pNodePlane = g_dplanes + pNode->planenum;

		if ( pNode->children[0] == child )
		{
			// front side
			list->planes[list->numLeafPlanes] = *pNodePlane;
		}
		else
		{
			// back side
			int plane = list->numLeafPlanes;
			list->planes[plane].dist = -pNodePlane->dist;
			list->planes[plane].normal[0] = -pNodePlane->normal[0];
			list->planes[plane].normal[1] = -pNodePlane->normal[1];
			list->planes[plane].normal[2] = -pNodePlane->normal[2];
			list->planes[plane].type = pNodePlane->type;
			list->numLeafPlanes++;
		}

		if( list->numLeafPlanes >= MAX_LEAF_PLANES )
			break; // there was a too many planes

		child = nodeIndex;
		nodeIndex = nodeparents[child];
	}
}

// add the sample to the list.  If we exceed the maximum number of samples, the worst sample will
// be discarded.  This has the effect of converging on the best samples when enough are added.
void AddSampleToList( ambientlocallist_t *list, const vec3_t samplePosition, vec3_t pCube[6] )
{
	int i;
	int index = list->numSamples++;
	VectorCopy( samplePosition, list->samples[index].pos );

	for ( int k = 0; k < 6; k++ )
	{
		VectorCopy( pCube[k], list->samples[index].cube[k] );
	}

	if ( list->numSamples <= MAX_SAMPLES )
		return;

	ambientlocallist_t	oldlist;
	int nearestNeighborIndex = 0;
	float nearestNeighborDist = FLT_MAX;
	float nearestNeighborTotal = 0;

	// do a copy of current list
	memcpy( &oldlist, list, sizeof( ambientlocallist_t ));

	for ( i = 0; i < oldlist.numSamples; i++ )
	{
		int closestIndex = 0;
		float closestDist = FLT_MAX;
		float totalDC = 0;

		for ( int j = 0; j < oldlist.numSamples; j++ )
		{
			if ( j == i ) continue;

			vec3_t vDelta;

			VectorSubtract( oldlist.samples[i].pos, oldlist.samples[j].pos, vDelta );

			float dist = VectorLength( vDelta );
			float maxDC = 0;

			for ( int k = 0; k < 6; k++ )
			{
				// color delta is computed per-component, per cube side
				for (int s = 0; s < 3; s++ )
				{
					float dc = fabs( oldlist.samples[i].cube[k][s] - oldlist.samples[j].cube[k][s] );
					maxDC = qmax( maxDC, dc );
				}

				totalDC += maxDC;
			}

			// need a measurable difference in color or we'll just rely on position
			if ( maxDC < 1e-4f )
			{
				maxDC = 0;
			}
			else if ( maxDC > 1.0f )
			{
				maxDC = 1.0f;
			}

			// selection criteria is 10% distance, 90% color difference
			// choose samples that fill the space (large distance from each other)
			// and have largest color variation
			float distanceFactor = 0.1f + (maxDC * 0.9f);
			dist *= distanceFactor;

			// find the "closest" sample to this one
			if ( dist < closestDist )
			{
				closestDist = dist;
				closestIndex = j;
			}
		}

		// the sample with the "closest" neighbor is rejected
		if ( closestDist < nearestNeighborDist || (closestDist == nearestNeighborDist && totalDC < nearestNeighborTotal) )
		{
			nearestNeighborDist = closestDist;
			nearestNeighborIndex = i;
		}
	}

	list->numSamples = 0;

	// copy the entries back but skip nearestNeighborIndex
	for( i = 0; i < oldlist.numSamples; i++ )
	{
		if( i != nearestNeighborIndex )
		{
			memcpy( &list->samples[list->numSamples], &oldlist.samples[i], sizeof( ambientsample_t ));
			list->numSamples++;
		}
	}
}

// max number of units in gamma space of per-side delta
int CubeDeltaGammaSpace( vec3_t pCube0[6], vec3_t pCube1[6] )
{
	int maxDelta = 0;

	// do this comparison in gamma space to try and get a perceptual basis for the compare
	for ( int i = 0; i < 6; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			int val0 = LinearToScreenGamma( pCube0[i][j] );
			int val1 = LinearToScreenGamma( pCube1[i][j] );
			int delta = abs(val0 - val1);

			if ( delta > maxDelta )
				maxDelta = delta;
		}
	}

	return maxDelta;
}
// reconstruct the ambient lighting for a leaf at the given position in worldspace
// optionally skip one of the entries in the list
void Mod_LeafAmbientColorAtPos( vec3_t pOut[6], const vec3_t pos, ambientlocallist_t *list, int skipIndex )
{
	vec3_t vDelta;
	int i;

	for ( i = 0; i < 6; i++ )
	{
		VectorClear( pOut[i] );
	}

	float totalFactor = 0.0f;

	for ( i = 0; i < list->numSamples; i++ )
	{
		if ( i == skipIndex )
			continue;

		// do an inverse squared distance weighted average of the samples to reconstruct
		// the original function
		VectorSubtract( list->samples[i].pos, pos, vDelta );
		float dist = DotProduct( vDelta, vDelta );
		float factor = 1.0f / (dist + 1.0f);
		totalFactor += factor;

		for ( int j = 0; j < 6; j++ )
		{
			VectorMA( pOut[j], factor, list->samples[i].cube[j], pOut[j] );
		}
	}

	for ( i = 0; i < 6; i++ )
	{
		VectorScale( pOut[i], (1.0f / totalFactor), pOut[i] );
	}
}

// this samples the lighting at each sample and removes any unnecessary samples
void CompressAmbientSampleList( ambientlocallist_t *list )
{
	ambientlocallist_t	oldlist;
	vec3_t		testCube[6];

	// do a copy of current list
	memcpy( &oldlist, list, sizeof( ambientlocallist_t ));
	list->numSamples = 0;

	for ( int i = 0; i < oldlist.numSamples; i++ )
	{
		Mod_LeafAmbientColorAtPos( testCube, oldlist.samples[i].pos, &oldlist, i );

		// at least one sample must be included in the list
		if ( i == 0 || CubeDeltaGammaSpace( testCube, oldlist.samples[i].cube ) >= 3 )
		{
			memcpy( &list->samples[list->numSamples], &oldlist.samples[i], sizeof( ambientsample_t ));
			list->numSamples++;
		}
	}
}

void ComputeAmbientForLeaf( int leafID, ambientlocallist_t *list )
{
	leafplanes_t	leafPlanes;

	leafPlanes.numLeafPlanes = 0;

	GetLeafBoundaryPlanes( &leafPlanes, leafID );

	// this heuristic tries to generate at least one sample per volume (chosen to be similar to the size of a player) in the space
	int xSize = (g_dleafs[leafID].maxs[0] - g_dleafs[leafID].mins[0]) / 64;
	int ySize = (g_dleafs[leafID].maxs[1] - g_dleafs[leafID].mins[1]) / 64;
	int zSize = (g_dleafs[leafID].maxs[2] - g_dleafs[leafID].mins[2]) / 64;

	xSize = qmax( xSize, 1 );
	ySize = qmax( ySize, 1 );
	zSize = qmax( zSize, 1 );

	// generate update 128 candidate samples, always at least one sample
	int volumeCount = xSize * ySize * zSize;

	if( g_fastmode )
		volumeCount *= 0.01;
	else if( !g_extra )
		volumeCount *= 0.05;
	else volumeCount *= 0.1;

	int sampleCount = bound( MIN_LOCAL_SAMPLES, volumeCount, MAX_LOCAL_SAMPLES );

	if ( g_dleafs[leafID].contents == CONTENTS_SOLID )
	{
		// don't generate any samples in solid leaves
		// NOTE: We copy the nearest non-solid leaf
		// sample pointers into this leaf at the end
		return;
	}

	vec3_t cube[6];

	for ( int i = 0; i < sampleCount; i++ )
	{
		// compute each candidate sample and add to the list
		vec3_t samplePosition;
		GenerateLeafSamplePosition( leafID, list, &leafPlanes, samplePosition );
		ComputeAmbientFromSphericalSamples( samplePosition, cube );
		// note this will remove the least valuable sample once the limit is reached
		AddSampleToList( list, samplePosition, cube );
	}

	// remove any samples that can be reconstructed with the remaining data
	CompressAmbientSampleList( list );
}

static void LeafAmbientLighting( int threadnum )
{
	ambientlocallist_t	list;

	while( 1 )
	{
		int leafID = GetThreadWork ();
		if (leafID == -1) break;

		list.numSamples = 0;

		ComputeAmbientForLeaf( leafID, &list );

		// copy to the output array
		g_leaf_samples[leafID].numSamples = list.numSamples;
		g_leaf_samples[leafID].samples = (ambientsample_t *)AllocBlock( sizeof( ambientsample_t ) * list.numSamples );
		memcpy( g_leaf_samples[leafID].samples, list.samples, sizeof( ambientsample_t ) * list.numSamples );
	}
}

static void LeafAmbientLightingSingle( int leafID )
{
	ambientlocallist_t	list;

	list.numSamples = 0;

	if ( g_dleafs[leafID].contents == CONTENTS_SOLID )
	{
		// don't generate any samples in solid leaves
		// NOTE: We copy the nearest non-solid leaf
		// sample pointers into this leaf at the end
		return;
	}

	ComputeAmbientForLeaf( leafID, &list );

	// copy to the output array
	g_leaf_samples[leafID].numSamples = list.numSamples;
	g_leaf_samples[leafID].samples = (ambientsample_t *)AllocBlock( sizeof( ambientsample_t ) * list.numSamples );
	memcpy( g_leaf_samples[leafID].samples, list.samples, sizeof( ambientsample_t ) * list.numSamples );
}

void ComputeLeafAmbientLighting( void )
{
	// Figure out which lights should go in the per-leaf ambient cubes.
	int nInAmbientCube = 0;
	int nSurfaceLights = 0;
	int i;

	// always matched
	memset( g_leaf_samples, 0, sizeof( g_leaf_samples ));

	for ( i = 0; i < g_numworldlights; i++ )
	{
		dworldlight_t *wl = &g_dworldlights[i];

		if ( IsLeafAmbientSurfaceLight( wl ) )
			wl->flags |= DWL_FLAGS_INAMBIENTCUBE;
		else wl->flags &= ~DWL_FLAGS_INAMBIENTCUBE;

		if ( wl->emittype == emit_surface )
			nSurfaceLights++;

		if ( wl->flags & DWL_FLAGS_INAMBIENTCUBE )
			nInAmbientCube++;
	}

	srand( time( NULL ));	// init random generator
	BuildGammaTable();		// init gamma table

	Verbose( "%d of %d (%d%% of) surface lights went in leaf ambient cubes.\n", nInAmbientCube, nSurfaceLights, nSurfaceLights ? ((nInAmbientCube*100) / nSurfaceLights) : 0 );
#if 0
	for ( i = 0; i < g_dmodels[0].visleafs + 1; i++ )
		LeafAmbientLightingSingle( i );
#else
	NamedRunThreadsOnIndividual( g_dmodels[0].visleafs + 1, g_estimate, LeafAmbientLighting );
#endif
	// clear old samples
	g_numleaflights = 0;

	for ( int leafID = 0; leafID < g_dmodels[0].visleafs + 1; leafID++ )
	{
		ambientlist_t *list = &g_leaf_samples[leafID];

		hlassert( list->numSamples <= 255 );

		if( !list->numSamples ) continue;

		// compute the samples in disk format.  Encode the positions in 8-bits using leaf bounds fractions
		for ( int i = 0; i < list->numSamples; i++ )
		{
			hlassume( g_numleaflights <= MAX_MAP_LEAFLIGHTS, assume_MAX_MAP_LEAFLIGHTS );

			dleafsample_t *sample = &g_dleaflights[g_numleaflights];
			vec3_t outcol;
#ifdef ZHLT_LARGERANGE
			for( int j = 0; j < 3; j++ )
			{
				sample->origin[j] = (short)bound( -32767, (int)list->samples[i].pos[j], 32767 );
			}
#else
			VectorCopy( list->samples[i].pos, sample->origin );
#endif
			sample->leafnum = leafID;

			for ( int side = 0; side < 6; side++ )
			{
				sample->ambient.color[side][0] = bound( 0, list->samples[i].cube[side][0] * 255, 255 );
				sample->ambient.color[side][1] = bound( 0, list->samples[i].cube[side][1] * 255, 255 );
				sample->ambient.color[side][2] = bound( 0, list->samples[i].cube[side][2] * 255, 255 );
			}
			g_numleaflights++;
		}

		FreeBlock( list->samples );	// release src
		list->samples = NULL;
	}

	Verbose( "%i ambient samples stored\n", g_numleaflights );
}

#endif
