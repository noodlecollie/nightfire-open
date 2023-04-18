#include <cstddef>
#include "ui/crosshair/spreadVisualiser.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/geometryStatics.h"
#include "vector_classes.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "hud.h"
#include "cl_util.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/crosshairCvars.h"

namespace
{
	static constexpr float YOFFSET_FRAC = 0.7f;
	static constexpr size_t PADDING = 40;
	static constexpr size_t SCALE_HEIGHT = 50;
	static constexpr size_t MARKER_WIDTH = 25;
}

void CSpreadVisualiser::Draw(const CCrosshairParameters& params)
{
	ConstructGeometry(params);
	UpdateInaccuracyMarker(params);
	UpdateDynamicBars(params);

	CustomGeometry::RenderAdHocGeometry(m_Geometry);
	CustomGeometry::RenderAdHocGeometry(m_DynamicBars);
	DrawInfoText(params);
}

void CSpreadVisualiser::ConstructGeometry(const CCrosshairParameters& params)
{
	if ( m_Geometry && m_ScreenDim == params.ScreenDimensions() )
	{
		return;
	}

	m_Geometry = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());

	m_Geometry->SetColour(0xFFFFFFFF);
	m_Geometry->SetDrawType(CustomGeometry::DrawType::Lines);

	m_ScreenDim = params.ScreenDimensions();
	m_ScaleMinX = PADDING;
	m_ScaleMaxX = params.ScreenDimensions().x - PADDING;
	m_ScaleYOffset = YOFFSET_FRAC * params.ScreenDimensions().y;

	const float halfScaleHeight = static_cast<float>(SCALE_HEIGHT) / 2.0f;

	// Horizontal ruled line for scale
	m_Geometry->AddLine(Vector(m_ScaleMinX, m_ScaleYOffset, 0.0f), Vector(m_ScaleMaxX, m_ScaleYOffset, 0.0f));

	// Caps at each end
	m_Geometry->AddLine(Vector(m_ScaleMinX, m_ScaleYOffset - halfScaleHeight, 0), Vector(m_ScaleMinX, m_ScaleYOffset + halfScaleHeight, 0));
	m_Geometry->AddLine(Vector(m_ScaleMaxX, m_ScaleYOffset - halfScaleHeight, 0), Vector(m_ScaleMaxX, m_ScaleYOffset + halfScaleHeight, 0));

	const float inaccuracyX = ExtraMath::RemapLinear(params.WeaponInaccuracy(), 0, 1, m_ScaleMinX, m_ScaleMaxX);

	// Arrow marker for current inaccuracy.
	// This will be updated properly later.
	m_InaccuracyMarkerBegin = m_Geometry->GetPointCount();
	m_Geometry->AddLine(Vector(inaccuracyX, m_ScaleYOffset - SCALE_HEIGHT, 0), Vector(inaccuracyX, m_ScaleYOffset, 0));
	m_Geometry->AddLine(Vector(inaccuracyX, m_ScaleYOffset - SCALE_HEIGHT, 0), Vector(inaccuracyX, m_ScaleYOffset, 0));
}

void CSpreadVisualiser::UpdateInaccuracyMarker(const CCrosshairParameters& params)
{
	const float inaccuracyX = ExtraMath::RemapLinear(params.WeaponInaccuracy(), 0, 1, m_ScaleMinX, m_ScaleMaxX);
	const float halfMarkerWidth = static_cast<float>(MARKER_WIDTH) / 2.0f;

	uint8_t index = m_InaccuracyMarkerBegin;
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX - halfMarkerWidth, m_ScaleYOffset - SCALE_HEIGHT, 0);
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX, m_ScaleYOffset, 0);
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX + halfMarkerWidth, m_ScaleYOffset - SCALE_HEIGHT, 0);
	m_Geometry->GetPoint(index++) = Vector(inaccuracyX, m_ScaleYOffset, 0);
}

void CSpreadVisualiser::UpdateDynamicBars(const CCrosshairParameters& params)
{
	if ( !m_DynamicBars )
	{
		m_DynamicBars = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());
		m_DynamicBars->SetColour(0xFFD800FF);
		m_DynamicBars->SetDrawType(CustomGeometry::DrawType::Lines);
	}

	m_DynamicBars->ClearGeometry();

	const WeaponAtts::AccuracyParameters* accuracyParams = params.WeaponAccuracyParamsForAttack(params.WeaponAttackMode());

	if ( !accuracyParams )
	{
		return;
	}

	const float halfHeight = (2.0f * static_cast<float>(SCALE_HEIGHT)) / 6.0f;

	// Line representing rest inaccuracy.
	const float restX = ExtraMath::RemapLinear(accuracyParams->RestValue, 0, 1, m_ScaleMinX, m_ScaleMaxX);
	m_DynamicBars->AddLine(Vector(restX, m_ScaleYOffset - halfHeight, 0), Vector(restX, m_ScaleYOffset + halfHeight, 0));

	// Line representing run inaccuracy.
	const float runX = ExtraMath::RemapLinear(accuracyParams->RunValue, 0, 1, m_ScaleMinX, m_ScaleMaxX);
	m_DynamicBars->AddLine(Vector(runX, m_ScaleYOffset - halfHeight, 0), Vector(runX, m_ScaleYOffset + halfHeight, 0));
}

void CSpreadVisualiser::DrawInfoText(const CCrosshairParameters& params)
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(params.WeaponID());
	const char* weaponName = atts ? atts->Core.Classname : "UNKNOWN";

	WeaponAtts::AccuracyParameters accuracyParams;
	const bool usingDebugParams = InaccuracyModifiers::IsInaccuracyDebuggingEnabled();

	if ( usingDebugParams )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(accuracyParams);
	}
	else
	{
		const WeaponAtts::AccuracyParameters* accuracyParamsFromWeapon = params.WeaponAccuracyParamsForAttack(params.WeaponAttackMode());

		if ( accuracyParamsFromWeapon )
		{
			accuracyParams = *accuracyParamsFromWeapon;
		}
	}

	WeaponAtts::CrosshairParameters crosshairParams;
	const bool crosshairOverridden = CrosshairCvars::CrosshairOverrideEnabled();

	if ( crosshairOverridden )
	{
		// TODO: Make this a function in CrosshairCvars to fill out the struct?
		crosshairParams.RadiusMin = CrosshairCvars::RadiusMin();
		crosshairParams.RadiusMax = CrosshairCvars::RadiusMax();
		crosshairParams.BarScaleMin = CrosshairCvars::BarLengthMin();
		crosshairParams.BarScaleMax = CrosshairCvars::BarLengthMax();
	}
	else
	{
		const WeaponAtts::CrosshairParameters* crosshairParamsFromWeapon = params.CrosshairParamsForAttack(params.WeaponAttackMode());

		if ( crosshairParamsFromWeapon )
		{
			crosshairParams = *crosshairParamsFromWeapon;
		}
	}

	CUtlString text;

	text.AppendFormat("Weapon: %s (ID %u)\n", weaponName, static_cast<uint32_t>(params.WeaponID()));
	text.AppendFormat("Current inaccuracy: %f\n", params.WeaponInaccuracy());
	text.AppendFormat("Attributes for attack mode %u (source: %s):\n", params.WeaponAttackMode(), usingDebugParams ? "debug" : "weapon");

	text.AppendFormat("  Spread: Rest (%f, %f), Run (%f, %f)\n",
		accuracyParams.RestSpread.x,
		accuracyParams.RestSpread.y,
		accuracyParams.RunSpread.x,
		accuracyParams.RunSpread.y);

	text.AppendFormat("  Rest-run range: %f - %f\n", accuracyParams.RestValue, accuracyParams.RunValue);
	text.AppendFormat("  Crouch shift: %f\n", accuracyParams.CrouchShift);
	text.AppendFormat("  Air shift: %f\n", accuracyParams.AirShift);
	text.AppendFormat("  Fall shift: %f\n", accuracyParams.FallShift);
	text.AppendFormat("  Attack coefficient: %f\n", accuracyParams.AttackCoefficient);
	text.AppendFormat("  Decay coefficient: %f\n", accuracyParams.DecayCoefficient);
	text.AppendFormat("  Fire impulse: %f\n", accuracyParams.FireImpulse);
	text.AppendFormat("  Fire impulse ceiling: %f\n", accuracyParams.FireImpulseCeiling);
	text.AppendFormat("  Fire impulse hold time: %f\n", accuracyParams.FireImpulseHoldTime);
	text.AppendFormat("Attributes for crosshair (source: %s):\n", crosshairOverridden ? "debug" : "weapon");
	text.AppendFormat("  Radius range: %f - %f\n", crosshairParams.RadiusMin, crosshairParams.RadiusMax);
	text.AppendFormat("  Bar scale range: %f - %f\n", crosshairParams.BarScaleMin, crosshairParams.BarScaleMax);

	DrawConsoleString(PADDING, PADDING, text.Get());
}
