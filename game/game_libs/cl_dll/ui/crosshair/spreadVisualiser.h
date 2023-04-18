#pragma once

#include <cstdint>
#include "ui/crosshair/crosshairParameters.h"
#include "customGeometry/geometryItemPtr.h"

class CSpreadVisualiser
{
public:
	void Draw(const CCrosshairParameters& params);

private:
	void ConstructGeometry(const CCrosshairParameters& params);
	void UpdateInaccuracyMarker(const CCrosshairParameters& params);
	void UpdateDynamicBars(const CCrosshairParameters& params);
	void DrawInfoText(const CCrosshairParameters& params);

	CustomGeometry::GeometryItemPtr_t m_Geometry;
	CustomGeometry::GeometryItemPtr_t m_DynamicBars;
	UIVec2 m_ScreenDim;
	float m_ScaleMinX = 0.0f;
	float m_ScaleMaxX = 1.0f;
	float m_ScaleYOffset = 0.0f;

	uint8_t m_InaccuracyMarkerBegin = 0;
};
