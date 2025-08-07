#pragma once

#include "customGeometry/geometryItemPtr.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

class CCrosshairParameters;

class CBaseCrosshairRenderer
{
public:
	WeaponAtts::CrosshairStyle RenderStyle() const;

	virtual void Initialise(const CCrosshairParameters& params) = 0;
	virtual void Update(const CCrosshairParameters& params) = 0;
	virtual void Draw() = 0;

protected:
	CBaseCrosshairRenderer(WeaponAtts::CrosshairStyle renderStyle);

	CustomGeometry::GeometryItemPtr_t m_Geometry;

private:
	WeaponAtts::CrosshairStyle m_RenderStyle;
};
