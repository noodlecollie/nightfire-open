#include "MathLib/plane.h"

// fast box on planeside test
int SignbitsForPlane(const vec3_t normal)
{
	int bits, i;

	for ( bits = i = 0; i < 3; i++ )
	{
		if ( normal[i] < 0.0f )
		{
			bits |= 1 << i;
		}
	}

	return bits;
}

int PlaneTypeForNormal(const vec3_t normal)
{
	if ( normal[0] == 1.0f )
	{
		return PLANE_X;
	}

	if ( normal[1] == 1.0f )
	{
		return PLANE_Y;
	}

	if ( normal[2] == 1.0f )
	{
		return PLANE_Z;
	}

	return PLANE_NONAXIAL;
}

void PlaneIntersect(const mplane_t* plane, const vec3_t p0, const vec3_t p1, vec3_t out)
{
	if ( !plane )
	{
		return;
	}

	const float distToPlane = PlaneDiff(p0, plane);
	const float planeDotRay = DotProduct(plane->normal, p1);
	const float sect = -(distToPlane) / planeDotRay;

	VectorMA(p0, sect, p1, out);
}

static int BoxOnPlaneSideNonAxial(const vec3_t emins, const vec3_t emaxs, const mplane_t* p)
{
	float dist1 = 0.0f;
	float dist2 = 0.0f;
	int sides = 0;

	// general case
	switch ( p->signbits )
	{
		case 0:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			break;
		case 1:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			break;
		case 2:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			break;
		case 3:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			break;
		case 4:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			break;
		case 5:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			break;
		case 6:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			break;
		case 7:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			break;
		default:
			break;
	}

	if ( dist1 >= p->dist )
	{
		sides = 1;
	}

	if ( dist2 < p->dist )
	{
		sides |= 2;
	}

	return sides;
}

int BoxOnPlaneSide(const vec3_t emins, const vec3_t emaxs, const mplane_t* p)
{
	if ( !p )
	{
		return 0;
	}

	if ( p->type >= PLANE_NONAXIAL )
	{
		return BoxOnPlaneSideNonAxial(emins, emaxs, p);
	}

	if ( p->dist <= emins[p->type] )
	{
		return 1;
	}
	else if ( p->dist >= emaxs[p->type] )
	{
		return 2;
	}
	else
	{
		return 3;
	}
}
