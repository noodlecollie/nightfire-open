#pragma once

#include <memory>
#include "ui/core/uiVec2.h"
#include "weapons/weaponids.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

namespace WeaponAtts
{
	struct AccuracyParameters;
	struct CrosshairParameters;
}  // namespace WeaponAtts

class CCrosshairParameters
{
public:
	static constexpr uint32_t DEFAULT_COLOUR = 0xFF0000FF;

	CCrosshairParameters();
	void Reset();

	const UIVec2& ScreenDimensions() const;
	void SetScreenDimensions(const UIVec2& dim);

	float WeaponInaccuracy() const;
	void SetWeaponInaccuracy(float inaccuracy);

	float Radius() const;
	void SetRadius(float radius);

	float BarLength() const;
	void SetBarLength(float length);

	float Thickness() const;
	void SetThickness(float thickness);

	WeaponAtts::CrosshairStyle CrosshairStyle() const;
	void SetCrosshairStyle(WeaponAtts::CrosshairStyle style);

	WeaponId_e WeaponID() const;
	void SetWeaponID(WeaponId_e id);

	int WeaponAttackMode() const;
	void SetWeaponAttackMode(int mode);

	UIVec2 HalfScreenDimensions() const;
	int DisplacementFromScreenCentre(float fraction) const;

	const WeaponAtts::AccuracyParameters* WeaponAccuracyParamsForAttack(size_t index) const;
	const WeaponAtts::CrosshairParameters* CrosshairParamsForAttack(size_t index) const;

	static float InterpolateBetweenRestAndRun(
		const WeaponAtts::AccuracyParameters& accuracyParams,
		float inaccuracy,
		float min,
		float max);

	static float ComputeCrosshairRadius(
		const WeaponAtts::AccuracyParameters& accuracyParams,
		float inaccuracy,
		const WeaponAtts::CrosshairParameters& crosshairParams);

	static float ComputeCrosshairBarLength(
		const WeaponAtts::AccuracyParameters& accuracyParams,
		float inaccuracy,
		const WeaponAtts::CrosshairParameters& crosshairParams);

	static float ComputeCrosshairRadiusFromDebugCvars(
		const WeaponAtts::AccuracyParameters& accuracyParams,
		float inaccuracy);

	static float ComputeCrosshairBarLengthFromDebugCvars(
		const WeaponAtts::AccuracyParameters& accuracyParams,
		float inaccuracy);

private:
	UIVec2 m_ScreenDimensions;
	float m_WeaponInaccuracy = 0.0f;
	float m_Radius = 0.0f;
	float m_BarLength = 0.1f;
	float m_Thickness = 2.0f;
	WeaponAtts::CrosshairStyle m_CrosshairStyle = WeaponAtts::CrosshairStyle::None;
	int m_AttackMode = 0;
	WeaponId_e m_WeaponID = WeaponId_e::WeaponNone;
};
