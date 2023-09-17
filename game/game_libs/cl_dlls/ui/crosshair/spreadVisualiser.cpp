#include <cstddef>
#include "ui/crosshair/spreadVisualiser.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/geometryStatics.h"
#include "MathLib/vec3.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "hud.h"
#include "cl_util.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/crosshairCvars.h"

static constexpr float YOFFSET_FRAC = 0.7f;
static constexpr size_t PADDING = 40;
static constexpr float SCALE_HEIGHT = 50.0f;
static constexpr float HALF_DYNAMIC_BAR_HEIGHT = (1.0f/3.0f) * SCALE_HEIGHT;
static constexpr float ACCURACY_MARKER_HEIGHT = 0.75f * SCALE_HEIGHT;
static constexpr float HALF_ACCURACY_MARKER_DEV = 10.0f;
static constexpr float LABEL_Y_OFFSET = 48.0f;

// Returns if the params were overridden.
static bool PopulateAccuracyParams(const CCrosshairParameters& params, WeaponAtts::AccuracyParameters& accuracyParams)
{
	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(accuracyParams);
		return true;
	}

	const WeaponAtts::AccuracyParameters* accuracyParamsFromWeapon =
		params.WeaponAccuracyParamsForAttack(params.WeaponAttackMode());

	if ( accuracyParamsFromWeapon )
	{
		accuracyParams = *accuracyParamsFromWeapon;
	}

	return false;
}

void CSpreadVisualiser::Draw(const CCrosshairParameters& params)
{
	ConstructGeometry(params);
	UpdateDynamicBars(params);
	UpdateInaccuracyBar(params);

	CustomGeometry::RenderAdHocGeometry(m_Geometry);
	CustomGeometry::RenderAdHocGeometry(m_DynamicBars);
	CustomGeometry::RenderAdHocGeometry(m_InaccuracyBar);

	DrawScaleLabels();
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
	m_ScaleMaxX = static_cast<float>(params.ScreenDimensions().x - PADDING);
	m_ScaleYOffset = YOFFSET_FRAC * params.ScreenDimensions().y;

	const float halfScaleHeight = SCALE_HEIGHT / 2.0f;
	const float smallScaleHeight = SCALE_HEIGHT / 6.0f;
	const float decimalSectionWidth = (m_ScaleMaxX - m_ScaleMinX) / 10.0f;

	// Horizontal ruled line for scale
	m_Geometry->AddLine(Vector(m_ScaleMinX, m_ScaleYOffset, 0.0f), Vector(m_ScaleMaxX, m_ScaleYOffset, 0.0f));

	// Caps at each end
	m_Geometry->AddLine(
		Vector(m_ScaleMinX, m_ScaleYOffset - halfScaleHeight, 0),
		Vector(m_ScaleMinX, m_ScaleYOffset + halfScaleHeight, 0));
	m_Geometry->AddLine(
		Vector(m_ScaleMaxX, m_ScaleYOffset - halfScaleHeight, 0),
		Vector(m_ScaleMaxX, m_ScaleYOffset + halfScaleHeight, 0));

	// Decimal lines
	for ( int section = 1; section <= 9; ++section )
	{
		const float offset = m_ScaleMinX + (static_cast<float>(section) * decimalSectionWidth);

		m_Geometry->AddLine(
			Vector(offset, m_ScaleYOffset - smallScaleHeight, 0),
			Vector(offset, m_ScaleYOffset + smallScaleHeight, 0));
	}
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

	WeaponAtts::AccuracyParameters accuracyParams {};
	PopulateAccuracyParams(params, accuracyParams);

	m_ScaleNumY = m_ScaleYOffset + (SCALE_HEIGHT / 2.0f) + 4.0f;
	m_LabelY = m_ScaleYOffset + LABEL_Y_OFFSET;

	// Line representing rest inaccuracy.
	m_RestX = ExtraMath::RemapLinear(accuracyParams.RestValue, 0, 1, m_ScaleMinX, m_ScaleMaxX);
	m_DynamicBars->AddLine(
		Vector(m_RestX, m_ScaleYOffset - HALF_DYNAMIC_BAR_HEIGHT, 0),
		Vector(m_RestX, m_ScaleYOffset + LABEL_Y_OFFSET - 4.0f, 0));

	// Line representing run inaccuracy.
	m_RunX = ExtraMath::RemapLinear(accuracyParams.RunValue, 0, 1, m_ScaleMinX, m_ScaleMaxX);
	m_DynamicBars->AddLine(
		Vector(m_RunX, m_ScaleYOffset - HALF_DYNAMIC_BAR_HEIGHT, 0),
		Vector(m_RunX, m_ScaleYOffset + LABEL_Y_OFFSET - 4.0f, 0));
}

void CSpreadVisualiser::UpdateInaccuracyBar(const CCrosshairParameters& params)
{
	if ( !m_InaccuracyBar )
	{
		m_InaccuracyBar = CustomGeometry::GeometryItemPtr_t(new CustomGeometry::CGeometryItem());
		m_InaccuracyBar->SetColour(0xFF0000FF);
		m_InaccuracyBar->SetDrawType(CustomGeometry::DrawType::Triangles);
	}

	m_InaccuracyBar->ClearGeometry();

	const float inaccuracyX = ExtraMath::RemapLinear(params.WeaponInaccuracy(), 0, 1, m_ScaleMinX, m_ScaleMaxX);

	m_InaccuracyBar->AddTriangle(
		Vector(inaccuracyX, m_ScaleYOffset, 0),
		Vector(inaccuracyX - HALF_ACCURACY_MARKER_DEV, m_ScaleYOffset - ACCURACY_MARKER_HEIGHT, 0),
		Vector(inaccuracyX + HALF_ACCURACY_MARKER_DEV, m_ScaleYOffset - ACCURACY_MARKER_HEIGHT, 0));
}

void CSpreadVisualiser::DrawScaleLabels()
{
	const float decimalSectionWidth = (m_ScaleMaxX - m_ScaleMinX) / 10.0f;

	for ( int section = 0; section <= 10; ++section )
	{
		char buffer[8];
		const float offset = m_ScaleMinX + (static_cast<float>(section) * decimalSectionWidth);

		PlatformLib_SNPrintF(buffer, sizeof(buffer), "%d.%d", section / 10, section % 10);
		DrawLabel(offset, m_ScaleNumY, buffer);
	}
}

void CSpreadVisualiser::DrawInfoText(const CCrosshairParameters& params)
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(params.WeaponID());
	const char* weaponName = atts ? atts->Core.Classname : "UNKNOWN";

	WeaponAtts::AccuracyParameters accuracyParams {};
	const bool usingDebugParams = PopulateAccuracyParams(params, accuracyParams);

	WeaponAtts::CrosshairParameters crosshairParams;
	const bool crosshairOverridden = CrosshairCvars::CrosshairOverrideEnabled();

	if ( crosshairOverridden )
	{
		CrosshairCvars::PopulateCrosshairParametersFromDebugCvars(crosshairParams);
	}
	else
	{
		const WeaponAtts::CrosshairParameters* crosshairParamsFromWeapon =
			params.CrosshairParamsForAttack(params.WeaponAttackMode());

		if ( crosshairParamsFromWeapon )
		{
			crosshairParams = *crosshairParamsFromWeapon;
		}
	}

	const float radius = CrosshairCvars::CrosshairOverrideEnabled()
		? CCrosshairParameters::ComputeCrosshairRadiusFromDebugCvars(accuracyParams, params.WeaponInaccuracy())
		: CCrosshairParameters::ComputeCrosshairRadius(accuracyParams, params.WeaponInaccuracy(), crosshairParams);

	const float barLength = CrosshairCvars::CrosshairOverrideEnabled()
		? CCrosshairParameters::ComputeCrosshairBarLengthFromDebugCvars(accuracyParams, params.WeaponInaccuracy())
		: CCrosshairParameters::ComputeCrosshairBarLength(accuracyParams, params.WeaponInaccuracy(), crosshairParams);

	const Vector2D spread = InaccuracyModifiers::GetInterpolatedSpread(accuracyParams, params.WeaponInaccuracy());

	CUtlString text;

	text.AppendFormat("Weapon: %s (ID %u)\n", weaponName, static_cast<uint32_t>(params.WeaponID()));
	text.AppendFormat("Current inaccuracy: %f\n", params.WeaponInaccuracy());
	text.AppendFormat("Spread: (%f, %f)\n", spread.x, spread.y);
	text.AppendFormat("Crosshair radius: %f\n", radius);
	text.AppendFormat("Crosshair bar length: %f\n", barLength);
	text.AppendFormat(
		"Attributes for attack mode %u (source: %s):\n",
		params.WeaponAttackMode(),
		usingDebugParams ? "debug" : "weapon");

	text.AppendFormat(
		"  Spread: Rest (%f, %f), Run (%f, %f)\n",
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
	text.AppendFormat("  Fire impulse decay window: %fs\n", accuracyParams.FireImpulseDecayWindow);
	text.AppendFormat("  Fire impulse decay mod: %f\n", accuracyParams.FireImpulseDecayMod);
	text.AppendFormat("Attributes for crosshair (source: %s):\n", crosshairOverridden ? "debug" : "weapon");
	text.AppendFormat("  Radius range: %f - %f\n", crosshairParams.RadiusMin, crosshairParams.RadiusMax);
	text.AppendFormat("  Bar scale range: %f - %f\n", crosshairParams.BarScaleMin, crosshairParams.BarScaleMax);

	DrawConsoleString(PADDING, PADDING, text.Get());

	DrawLabel(m_RestX, m_LabelY, "Rest");
	DrawLabel(m_RunX, m_LabelY, "Run");
}

void CSpreadVisualiser::DrawLabel(float x, float y, const char* text)
{
	if ( !text || !(*text) )
	{
		return;
	}

	int stringWidth = 0;
	GetConsoleStringSize(text, &stringWidth, nullptr);

	int posX = static_cast<int>(x - (static_cast<float>(stringWidth) / 2.0f));
	DrawConsoleString(posX, static_cast<int>(y), text);
}
