#include "hud_crosshair.h"
#include "customGeometry/geometryStatics.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "gameplay/crosshairCvars.h"
#include "ui/crosshair/CrosshairRendererFactory.h"

enum CrosshairDisplayMode
{
	DisplayNoCrosshair = 0,
	DisplaySpriteCrosshair = 1,
	DisplayDynamicCrosshair = 2
};

struct ParamsTuple
{
	const WeaponAtts::CrosshairParameters* Crosshair = nullptr;
	WeaponAtts::AccuracyParameters Accuracy;
};

static ParamsTuple GetParams(WeaponId_e id, int attackMode)
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(id);

	if ( !atts || attackMode >= atts->AttackModes.Count() )
	{
		return {};
	}

	const WeaponAtts::WABaseAttack* baseAttack = atts->AttackModes[attackMode].get();

	if ( !baseAttack )
	{
		return {};
	}

	ParamsTuple outParams {};
	outParams.Crosshair = &baseAttack->Crosshair;

	const WeaponAtts::WAAmmoBasedAttack* ammoBasedAttack =
		dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(baseAttack);

	if ( ammoBasedAttack )
	{
		outParams.Accuracy = ammoBasedAttack->Accuracy;
	}

	return outParams;
}

int CHudCrosshair::Init()
{
	m_iFlags |= HUD_ACTIVE;

	m_CheatsCvar = gEngfuncs.pfnGetCvarPointer("sv_cheats");
	m_CrosshairCvar = gEngfuncs.pfnGetCvarPointer("crosshair");

	gHUD.AddHudElem(this);
	return 1;
}

int CHudCrosshair::VidInit()
{
	SCREENINFO screenInfo;

	memset(&screenInfo, 0, sizeof(screenInfo));
	screenInfo.iSize = sizeof(screenInfo);
	gEngfuncs.pfnGetScreenInfo(&screenInfo);

	m_Params.SetScreenDimensions(UIVec2(screenInfo.iWidth, screenInfo.iHeight));

	return 1;
}

void CHudCrosshair::Reset()
{
	m_CrosshairRenderer.reset();
}

int CHudCrosshair::Draw(float)
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL) || !m_CrosshairCvar )
	{
		return 1;
	}

	if ( !UpdateParameters() )
	{
		return 1;
	}

	RefreshCrosshairRenderer();

	if ( m_CrosshairRenderer )
	{
		m_CrosshairRenderer->Update(m_Params);
		m_CrosshairRenderer->Draw();
	}

	if ( CrosshairCvars::SpreadVisualisationEnabled() )
	{
		m_SpreadVisualiser.Draw(m_Params);
	}

	return 1;
}

bool CHudCrosshair::UpdateParameters()
{
	WEAPON* weapon = gHUD.m_Ammo.GetCurrentWeapon();

	if ( !weapon || weapon->iId < 1 )
	{
		return false;
	}

	m_Params.SetWeaponID(static_cast<WeaponId_e>(weapon->iId));
	m_Params.SetWeaponAttackMode(weapon->iPriAttackMode);

	const ParamsTuple paramsTuple = GetParams(m_Params.WeaponID(), m_Params.WeaponAttackMode());

	if ( !paramsTuple.Crosshair )
	{
		return false;
	}

	const WeaponAtts::CrosshairParameters* m_CrosshairParams = paramsTuple.Crosshair;

	m_Params.SetCrosshairStyle(m_CrosshairParams->RenderStyle);
	m_Params.SetWeaponInaccuracy(gHUD.m_flWeaponInaccuracy);

	float radius = CCrosshairParameters::ComputeCrosshairRadius(
		paramsTuple.Accuracy,
		m_Params.WeaponInaccuracy(),
		*m_CrosshairParams
	);

	m_Params.SetRadius(radius);

	float barLength = CCrosshairParameters::ComputeCrosshairBarLength(
		paramsTuple.Accuracy,
		m_Params.WeaponInaccuracy(),
		*m_CrosshairParams
	);

	m_Params.SetBarLength(barLength);

	UpdateParametersFromDebugCvars();

	return true;
}

void CHudCrosshair::UpdateParametersFromDebugCvars()
{
	if ( !CrosshairCvars::CrosshairOverrideEnabled() )
	{
		return;
	}

	// Make sure we always show the crosshair if the override cvars are in use.
	if ( m_Params.CrosshairStyle() == WeaponAtts::CrosshairStyle::None )
	{
		m_Params.SetCrosshairStyle(WeaponAtts::CrosshairStyle::QuadLine);
	}

	WEAPON* weapon = gHUD.m_Ammo.GetCurrentWeapon();

	if ( !weapon || weapon->iId < 1 )
	{
		return;
	}

	int mode = CrosshairCvars::AttackModeForSpreadVisualisation();

	if ( mode >= 0 )
	{
		// Attack mode was manually specified.
		m_Params.SetWeaponAttackMode(mode);
	}
	else
	{
		// Take attack mode from current weapon.
		m_Params.SetWeaponAttackMode(weapon->iPriAttackMode);
	}

	const ParamsTuple paramsTuple = GetParams(m_Params.WeaponID(), m_Params.WeaponAttackMode());

	float radius =
		CCrosshairParameters::ComputeCrosshairRadiusFromDebugCvars(paramsTuple.Accuracy, m_Params.WeaponInaccuracy());

	m_Params.SetRadius(radius);

	float barLength = CCrosshairParameters::ComputeCrosshairBarLengthFromDebugCvars(
		paramsTuple.Accuracy,
		m_Params.WeaponInaccuracy()
	);

	m_Params.SetBarLength(barLength);
}

void CHudCrosshair::RefreshCrosshairRenderer()
{
	if ( m_CrosshairCvar->value == DisplayDynamicCrosshair &&
		 m_Params.CrosshairStyle() != WeaponAtts::CrosshairStyle::None )
	{
		if ( !m_CrosshairRenderer || m_CrosshairRenderer->RenderStyle() != m_Params.CrosshairStyle() )
		{
			m_CrosshairRenderer = GetCrosshairRenderer(m_Params.CrosshairStyle());

			if ( m_CrosshairRenderer )
			{
				m_CrosshairRenderer->Initialise(m_Params);
			}
		}
	}
	else
	{
		m_CrosshairRenderer.reset();
	}
}
