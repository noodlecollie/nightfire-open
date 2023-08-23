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
	void UpdateDynamicBars(const CCrosshairParameters& params);
	void UpdateInaccuracyBar(const CCrosshairParameters& params);
	void DrawInfoText(const CCrosshairParameters& params);
	void DrawLabel(float x, float y, const char* text);
	void DrawScaleLabels();

	CustomGeometry::GeometryItemPtr_t m_Geometry;
	CustomGeometry::GeometryItemPtr_t m_DynamicBars;
	CustomGeometry::GeometryItemPtr_t m_InaccuracyBar;
	UIVec2 m_ScreenDim;
	float m_ScaleMinX = 0.0f;
	float m_ScaleMaxX = 1.0f;
	float m_ScaleYOffset = 0.0f;

	float m_ScaleNumY = 0.0f;
	float m_LabelY = 0.0f;
	float m_RestX = 0.0f;
	float m_RunX = 0.0f;
};
