#ifndef WINDING_H__
#define WINDING_H__
#include "cmdlib.h" //--vluzacn

#if _MSC_VER >= 1000
#pragma once
#endif

#include "basictypes.h"
#include "mathtypes.h"
#include "win32fix.h"
#include "mathlib.h"
#include "bspfile.h"
#include "boundingbox.h"

#define MAX_POINTS_ON_WINDING 128
// TODO: FIX THIS STUPID SHIT (MAX_POINTS_ON_WINDING)

#define BASE_WINDING_DISTANCE 9000

#define	SIDE_FRONT		0
#define	SIDE_ON			2
#define	SIDE_BACK		1
#define	SIDE_CROSS		-2

#ifdef HLCSG_HLBSP_DOUBLEPLANE
#ifdef HLBSP
#ifndef DOUBLEVEC_T
#error you must add -dDOUBLEVEC_T to the project!
#endif
#define dplane_t plane_t
#define g_dplanes g_mapplanes
typedef struct
{
	vec3_t			normal;
	vec3_t			unused_origin;
	vec_t			dist;
	planetypes		type;
} dplane_t;
extern dplane_t g_dplanes[MAX_INTERNAL_MAP_PLANES];
#endif
#endif
class Winding
{
public:
    // General Functions
    void            Print() const;
    void            getPlane(dplane_t& plane) const;
    void            getPlane(vec3_t& normal, vec_t& dist) const;
    vec_t           getArea() const;
    void            getBounds(BoundingBox& bounds) const;
    void            getBounds(vec3_t& mins, vec3_t& maxs) const;
    void            getCenter(vec3_t& center) const;
    Winding*        Copy() const;
    void            Check(
#ifdef ZHLT_WINDING_EPSILON
		vec_t epsilon = ON_EPSILON
#endif
		) const;  // Developer check for validity
    bool            Valid() const;  // Runtime/user/normal check for validity
    void            addPoint(const vec3_t newpoint);
    void            insertPoint(const vec3_t newpoint, const unsigned int offset);

    // Specialized Functions
    void            RemoveColinearPoints(
#ifdef ZHLT_WINDING_EPSILON
		vec_t epsilon = ON_EPSILON
#endif
		);
    bool            Clip(const dplane_t& split, bool keepon
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		); // For hlbsp
    void            Clip(const dplane_t& split, Winding** front, Winding** back
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		);
    void            Clip(const vec3_t normal, const vec_t dist, Winding** front, Winding** back
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		);
    bool            Chop(const vec3_t normal, const vec_t dist
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		);
    void            Divide(const dplane_t& split, Winding** front, Winding** back
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		);
    int             WindingOnPlaneSide(const vec3_t normal, const vec_t dist
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		);
	void			CopyPoints(vec3_t *points, int &numpoints);

	void			initFromPoints(vec3_t *points, UINT32 numpoints);
	void			Reset(void);	// Resets the structure
protected:
    void            resize(UINT32 newsize);

public:
    // Construction
	Winding();										// Do nothing :)
	Winding(vec3_t *points, UINT32 numpoints);		// Create from raw points
    Winding(const dface_t& face
#ifdef ZHLT_WINDING_EPSILON
		, vec_t epsilon = ON_EPSILON
#endif
		);
    Winding(const dplane_t& face);
    Winding(const vec3_t normal, const vec_t dist);
    Winding(UINT32 points);
    Winding(const Winding& other);
    virtual ~Winding();
    Winding& operator=(const Winding& other);

    // Misc
private:
    void initFromPlane(const vec3_t normal, const vec_t dist);

public:
    // Data
    UINT32  m_NumPoints;
    vec3_t* m_Points;
protected:
    UINT32  m_MaxPoints;
};

#endif