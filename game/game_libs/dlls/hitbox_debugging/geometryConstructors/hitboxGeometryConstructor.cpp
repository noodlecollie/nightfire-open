#include "hitboxGeometryConstructor.h"

void CHitboxGeometryConstructor::SetEntity(CBaseAnimating* entity)
{
	m_pEntity = entity;
}

void CHitboxGeometryConstructor::SetHitboxIndex(uint32_t index)
{
	m_iHitboxIndex = index;
}

bool CHitboxGeometryConstructor::AddGeometry(const CustomGeometry::GeometryItemPtr_t& geom) const
{
	if ( !m_pEntity )
	{
		return false;
	}

	edict_t* edict = m_pEntity->edict();
	uint32_t hitboxCount = g_engfuncs.pfnGetHitboxCount(edict);

	if ( m_iHitboxIndex >= hitboxCount )
	{
		return false;
	}

	HitboxFloats floats;

	if ( !g_engfuncs.pfnGetTransformedHitboxPoints(edict, m_iHitboxIndex, floats.floats) )
	{
		return false;
	}

	geom->SetDrawType(CustomGeometry::DrawType::Lines);

	HitboxPoints points;
	HitboxFloatsToPoints(floats, points);
	CreateGeometryFromPoints(geom, points);

	return true;
}

void CHitboxGeometryConstructor::CreateGeometryFromPoints(
	const CustomGeometry::GeometryItemPtr_t& geom,
	const HitboxPoints& points) const
{
	// Bottom rect
	CreateRectFromPoints(geom, points, 0);

	// Top rect
	CreateRectFromPoints(geom, points, 4);

	// Connect up the corners
	geom->AddLineIndices(0, 4);
	geom->AddLineIndices(1, 5);
	geom->AddLineIndices(2, 6);
	geom->AddLineIndices(3, 7);
}

void CHitboxGeometryConstructor::CreateRectFromPoints(
	const CustomGeometry::GeometryItemPtr_t& geom,
	const HitboxPoints& points,
	uint32_t offset) const
{
	ASSERT(HitboxPoints::NUM_ELEMENTS - offset >= 4);

	size_t baseIndex = geom->GetPointCount();

	// Points are specified as (min, min), (max, min), (min, max), (max, max).
	// Reorder these slightly so that only one component changes on each step.
	geom->AddPoint(points.points[offset]);
	geom->AddPoint(points.points[offset + 1]);
	geom->AddPoint(points.points[offset + 3]);
	geom->AddPoint(points.points[offset + 2]);

	for ( uint32_t index = 0; index < 4; ++index )
	{
		geom->AddLineIndices(baseIndex + index, baseIndex + ((index + 1) % 4));
	}
}

void CHitboxGeometryConstructor::HitboxFloatsToPoints(const HitboxFloats& floats, HitboxPoints& points)
{
	for ( uint32_t pointIndex = 0; pointIndex < HitboxPoints::NUM_ELEMENTS; ++pointIndex )
	{
		points.points[pointIndex] = Vector(
			floats.floats[(3 * pointIndex) + 0],
			floats.floats[(3 * pointIndex) + 1],
			floats.floats[(3 * pointIndex) + 2]);
	}
}
