#pragma once

#include "hud.h"
#include "customGeometry/geometryItemPtr.h"
#include "ui/crosshair/crosshairParameters.h"
#include "weapons/weaponids.h"
#include "ui/crosshair/spreadVisualiser.h"

namespace WeaponAtts
{
	struct WAAmmoBasedAttack;
}

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
	void InitialiseGeometry();
	void UpdateGeometry();
	const WeaponAtts::WAAmmoBasedAttack* GetAttackMode(const WEAPON& weapon) const;

	cvar_t* m_CheatsCvar = nullptr;
	cvar_t* m_CrosshairCvar = nullptr;

	CustomGeometry::GeometryItemPtr_t m_CrosshairGeometry;
	CCrosshairParameters m_Params;
	CSpreadVisualiser m_SpreadVisualiser;
};
