#pragma once

#include "ui/crosshair/BaseCrosshairRenderer.h"
#include "utlvector.h"

class CCircleCrosshairRenderer : public CBaseCrosshairRenderer
{
public:
	CCircleCrosshairRenderer();

	void Initialise(const CCrosshairParameters& params) override;
	void Update(const CCrosshairParameters& params) override;
	void Draw() override;

private:
	static constexpr size_t NUM_DIVISIONS = 24;

	CUtlVector<Vector> m_CirclePoints;
};
