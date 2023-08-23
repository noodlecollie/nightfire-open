#pragma once

#include "ui/crosshair/BaseCrosshairRenderer.h"

class CQuadLineCrosshairRenderer : public CBaseCrosshairRenderer
{
public:
	CQuadLineCrosshairRenderer();

	void Initialise(const CCrosshairParameters& params) override;
	void Update(const CCrosshairParameters& params) override;
	void Draw() override;
};
