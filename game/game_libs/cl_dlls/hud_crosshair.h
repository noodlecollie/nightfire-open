#pragma once

#include <memory>
#include "hud.h"
#include "customGeometry/geometryItemPtr.h"
#include "ui/crosshair/crosshairParameters.h"
#include "weapons/weaponids.h"
#include "ui/crosshair/spreadVisualiser.h"
#include "ui/crosshair/BaseCrosshairRenderer.h"

class CHudCrosshair : public CHudBase
{
public:
	virtual ~CHudCrosshair() = default;

	int Init();
	int VidInit();
	void Reset();
	int Draw(float flTime);

private:
	bool UpdateParameters();
	void UpdateParametersFromDebugCvars();

	cvar_t* m_CheatsCvar = nullptr;
	cvar_t* m_CrosshairCvar = nullptr;

	CCrosshairParameters m_Params;
	CSpreadVisualiser m_SpreadVisualiser;
	std::shared_ptr<CBaseCrosshairRenderer> m_CrosshairRenderer;
};
