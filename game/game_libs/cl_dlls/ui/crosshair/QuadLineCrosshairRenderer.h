#pragma once

#include "ui/crosshair/BaseCrosshairRenderer.h"

class CQuadLineCrosshairRenderer : public CBaseCrosshairRenderer
{
public:
	CQuadLineCrosshairRenderer();

	void Update(const CCrosshairParameters& params) override;
	void Draw() override;

private:
	void Initialise();
};
