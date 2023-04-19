#include "hud_crosshair.h"
#include "customGeometry/geometryStatics.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "gameplay/crosshairCvars.h"

namespace
{
	// Crosshair bars are added in this order.
	// The first two points of each bar are inner, and the second two are outer.
	enum CrosshairBar
	{
		TopBar = 0,
		BottomBar,
		LeftBar,
		RightBar
	};

	static constexpr size_t BAR_HALF_WIDTH = 1;

	inline constexpr uint8_t PointOffset(CrosshairBar bar)
	{
		return 4 * bar;
	}
}

int CHudCrosshair::Init()
{
	m_CrosshairGeometry = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());
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
	m_CrosshairGeometry->SetColour(0xFF0000FF);
	m_CrosshairGeometry->SetDrawType(CustomGeometry::DrawType::Triangles);
	InitialiseGeometry();

	return 1;
}

void CHudCrosshair::Reset()
{
}

int CHudCrosshair::Draw(float flTime)
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL) || !m_CrosshairCvar )
	{
		return 1;
	}

	if ( !UpdateParameters() )
	{
		return 1;
	}

	// Value of 1 is default crosshair sprite,
	// value of 2 is dynamic crosshair.
	if ( m_CrosshairCvar->value == 2 && m_Params.ShowCrosshair() )
	{
		UpdateGeometry();
		CustomGeometry::RenderAdHocGeometry(m_CrosshairGeometry);
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
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = GetAttackMode(*weapon);

	if ( !ammoAttack )
	{
		return false;
	}

	const WeaponAtts::CrosshairParameters* m_CrosshairParams = &ammoAttack->Crosshair;

	m_Params.SetShowCrosshair(m_CrosshairParams->HasCrosshair);
	m_Params.SetWeaponInaccuracy(gHUD.m_flWeaponInaccuracy);
	m_Params.SetWeaponAttackMode(weapon->iPriAttackMode);

	float radius = ExtraMath::RemapLinear(m_Params.WeaponInaccuracy(),
										  ammoAttack->Accuracy.RestValue,
										  ammoAttack->Accuracy.RunValue,
										  m_CrosshairParams->RadiusMin,
										  m_CrosshairParams->RadiusMax,
										  false);
	m_Params.SetRadius(radius);

	float barLength = ExtraMath::RemapLinear(m_Params.WeaponInaccuracy(),
											 ammoAttack->Accuracy.RestValue,
											 ammoAttack->Accuracy.RunValue,
											 m_CrosshairParams->BarScaleMin,
											 m_CrosshairParams->BarScaleMin,
											 false);
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
	m_Params.SetShowCrosshair(true);

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

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = GetAttackMode(*weapon);

	if ( !ammoAttack )
	{
		return;
	}

	float radius = ExtraMath::RemapLinear(m_Params.WeaponInaccuracy(),
										  ammoAttack->Accuracy.RestValue,
										  ammoAttack->Accuracy.RunValue,
										  CrosshairCvars::RadiusMin(),
										  CrosshairCvars::RadiusMax(),
										  false);

	m_Params.SetRadius(radius);

	float barLength = ExtraMath::RemapLinear(m_Params.WeaponInaccuracy(),
											 ammoAttack->Accuracy.RestValue,
											 ammoAttack->Accuracy.RunValue,
											 CrosshairCvars::BarLengthMin(),
											 CrosshairCvars::BarLengthMax(),
											 false);

	m_Params.SetBarLength(barLength);
}

void CHudCrosshair::InitialiseGeometry()
{
	if ( !m_CrosshairGeometry )
	{
		return;
	}

	m_CrosshairGeometry->ClearGeometry();
	const UIVec2 screenCentre = m_Params.HalfScreenDimensions();

	// Compute crosshair spokes. The positions will be manually modified later
	// according to the parameters we have.
	// The first two points of each bar are inner, and the second two are outer.

	// Top
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y - 1, 0),
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y - 1, 0),
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y - 2, 0),
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y - 2, 0)
	);

	// Bottom
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y + 1, 0),
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y + 1, 0),
		Vector(screenCentre.x - BAR_HALF_WIDTH, screenCentre.y + 2, 0),
		Vector(screenCentre.x + BAR_HALF_WIDTH, screenCentre.y + 2, 0)
	);

	// Left
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x - 1, screenCentre.y + BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x - 1, screenCentre.y - BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x - 2, screenCentre.y - BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x - 2, screenCentre.y + BAR_HALF_WIDTH, 0)
	);

	// Right
	m_CrosshairGeometry->AddTriangleQuad(
		Vector(screenCentre.x + 1, screenCentre.y - BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x + 1, screenCentre.y + BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x + 2, screenCentre.y + BAR_HALF_WIDTH, 0),
		Vector(screenCentre.x + 2, screenCentre.y - BAR_HALF_WIDTH, 0)
	);
}

void CHudCrosshair::UpdateGeometry()
{
	if ( !m_CrosshairGeometry || m_CrosshairGeometry->GetPointCount() != 4 * 4 )
	{
		return;
	}

	const UIVec2 screenCentre = m_Params.HalfScreenDimensions();
	const int innerDisp = m_Params.DisplacementFromScreenCentre(m_Params.Radius());
	const float barLength = Max(m_Params.BarLength(), 0.01f);
	const int outerDisp = m_Params.DisplacementFromScreenCentre(m_Params.Radius() + barLength);

	for ( uint8_t bar = 0; bar < 4; ++bar )
	{
		const uint8_t base = PointOffset(static_cast<CrosshairBar>(bar));

		switch ( bar )
		{
			case TopBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).y = screenCentre.y - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).y = screenCentre.y - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).y = screenCentre.y - outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).y = screenCentre.y - outerDisp;
				break;
			}

			case BottomBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).y = screenCentre.y + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).y = screenCentre.y + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).y = screenCentre.y + outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).y = screenCentre.y + outerDisp;
				break;
			}

			case LeftBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).x = screenCentre.x - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).x = screenCentre.x - innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).x = screenCentre.x - outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).x = screenCentre.x - outerDisp;
				break;
			}

			case RightBar:
			{
				m_CrosshairGeometry->GetPoint(base + 0).x = screenCentre.x + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 1).x = screenCentre.x + innerDisp;
				m_CrosshairGeometry->GetPoint(base + 2).x = screenCentre.x + outerDisp;
				m_CrosshairGeometry->GetPoint(base + 3).x = screenCentre.x + outerDisp;
				break;
			}
		}
	}
}

const WeaponAtts::WAAmmoBasedAttack* CHudCrosshair::GetAttackMode(const WEAPON& weapon) const
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(m_Params.WeaponID());

	if ( !atts || weapon.iPriAttackMode >= atts->AttackModes.Count() )
	{
		return nullptr;
	}

	const WeaponAtts::WABaseAttack* baseAttack = atts->AttackModes[weapon.iPriAttackMode].get();
	return dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(baseAttack);
}
