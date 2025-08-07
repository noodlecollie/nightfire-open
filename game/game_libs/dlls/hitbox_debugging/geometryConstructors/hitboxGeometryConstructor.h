#pragma once

#include "customGeometry/geometryItem.h"
#include "standard_includes.h"

class CBaseAnimating;

class CHitboxGeometryConstructor
{
public:
	void SetEntity(CBaseAnimating* entity);
	void SetHitboxIndex(uint32_t index);

	bool AddGeometry(const CustomGeometry::GeometryItemPtr_t& geom) const;

private:
	struct HitboxPoints
	{
		static constexpr size_t NUM_ELEMENTS = 8;
		Vector points[NUM_ELEMENTS];
	};

	struct HitboxFloats
	{
		static constexpr size_t NUM_ELEMENTS = HitboxPoints::NUM_ELEMENTS * 3;
		float floats[NUM_ELEMENTS];
	};

	static void HitboxFloatsToPoints(const HitboxFloats& floats, HitboxPoints& points);

	void CreateGeometryFromPoints(const CustomGeometry::GeometryItemPtr_t& geom, const HitboxPoints& points) const;

	void CreateRectFromPoints(
		const CustomGeometry::GeometryItemPtr_t& geom,
		const HitboxPoints& points,
		uint32_t offset) const;

	CBaseAnimating* m_pEntity = nullptr;
	uint32_t m_iHitboxIndex = 0;
};
