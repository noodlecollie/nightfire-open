#include "ui/crosshair/BaseCrosshairRenderer.h"
#include "customGeometry/geometryItem.h"

CBaseCrosshairRenderer::CBaseCrosshairRenderer(WeaponAtts::CrosshairStyle renderStyle) :
	m_Geometry(new CustomGeometry::CGeometryItem()),
	m_RenderStyle(renderStyle)
{
}

WeaponAtts::CrosshairStyle CBaseCrosshairRenderer::RenderStyle() const
{
	return m_RenderStyle;
}
