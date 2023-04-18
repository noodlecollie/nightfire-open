#pragma once

#include "ui/crosshair/crosshairParameters.h"

class CCrosshairBase
{
public:
	virtual void Init();
	virtual void VidInit();
	virtual void Draw(float flTime, const CCrosshairParameters& params);
};
