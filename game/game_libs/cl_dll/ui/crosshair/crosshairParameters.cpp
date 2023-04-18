#include "ui/crosshair/crosshairParameters.h"
#include "miniutl.h"
#include "util/extramath.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

CCrosshairParameters::CCrosshairParameters()
{
	Reset();
}

void CCrosshairParameters::Reset()
{
	m_ScreenDimensions.x = 0;
	m_ScreenDimensions.y = 0;
	m_WeaponInaccuracy = 0.0f;
	m_Radius = 0.0f;
	m_BarLength = 0.1f;
	m_WeaponID = WeaponId_e::WeaponNone;
}

const UIVec2& CCrosshairParameters::ScreenDimensions() const
{
	return m_ScreenDimensions;
}

void CCrosshairParameters::SetScreenDimensions(const UIVec2& dim)
{
	m_ScreenDimensions = dim;
}

float CCrosshairParameters::WeaponInaccuracy() const
{
	return m_WeaponInaccuracy;
}

void CCrosshairParameters::SetWeaponInaccuracy(float inaccuracy)
{
	m_WeaponInaccuracy = ExtraMath::Clamp(0.0f, inaccuracy, 1.0f);
}

float CCrosshairParameters::Radius() const
{
	return m_Radius;
}

void CCrosshairParameters::SetRadius(float radius)
{
	m_Radius = Max(radius, 0.0f);
}

float CCrosshairParameters::BarLength() const
{
	return m_BarLength;
}

void CCrosshairParameters::SetBarLength(float length)
{
	m_BarLength = length;
}

bool CCrosshairParameters::ShowCrosshair() const
{
	return m_ShowCrosshair;
}

void CCrosshairParameters::SetShowCrosshair(bool show)
{
	m_ShowCrosshair = show;
}

WeaponId_e CCrosshairParameters::WeaponID() const
{
	return m_WeaponID;
}

void CCrosshairParameters::SetWeaponID(WeaponId_e id)
{
	m_WeaponID = id;
}

int CCrosshairParameters::WeaponAttackMode() const
{
	return m_AttackMode;
}

void CCrosshairParameters::SetWeaponAttackMode(int mode)
{
	m_AttackMode = mode;
}

UIVec2 CCrosshairParameters::HalfScreenDimensions() const
{
	return m_ScreenDimensions / 2;
}

int CCrosshairParameters::DisplacementFromScreenCentre(float fraction) const
{
	const UIVec2 centre = HalfScreenDimensions();
	const int smallerDim = Min(centre.x, centre.y);

	return static_cast<int>(static_cast<float>(smallerDim) * fraction);
}

const WeaponAtts::AccuracyParameters* CCrosshairParameters::WeaponAccuracyParamsForAttack(size_t index) const
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(m_WeaponID);

	if ( !atts || index >= atts->AttackModes.Count() )
	{
		return nullptr;
	}

	std::shared_ptr<WeaponAtts::WABaseAttack> baseAttackMode = atts->AttackModes[index];

	if ( !baseAttackMode )
	{
		return nullptr;
	}

	std::shared_ptr<WeaponAtts::WAAmmoBasedAttack> ammoBasedAttack = std::dynamic_pointer_cast<WeaponAtts::WAAmmoBasedAttack>(baseAttackMode);

	// I know this is technically returning a raw pointer from shared pointer contents,
	// but the shared object's lifetime is static so it's OK.
	return ammoBasedAttack ? &ammoBasedAttack->Accuracy : nullptr;
}

const WeaponAtts::CrosshairParameters* CCrosshairParameters::CrosshairParamsForAttack(size_t index) const
{
	CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
	const WeaponAtts::WACollection* atts = registry.Get(m_WeaponID);

	if ( !atts || index >= atts->AttackModes.Count() )
	{
		return nullptr;
	}

	std::shared_ptr<WeaponAtts::WABaseAttack> baseAttackMode = atts->AttackModes[index];

	if ( !baseAttackMode )
	{
		return nullptr;
	}

	std::shared_ptr<WeaponAtts::WAAmmoBasedAttack> ammoBasedAttack = std::dynamic_pointer_cast<WeaponAtts::WAAmmoBasedAttack>(baseAttackMode);

	// I know this is technically returning a raw pointer from shared pointer contents,
	// but the shared object's lifetime is static so it's OK.
	return ammoBasedAttack ? &ammoBasedAttack->Crosshair : nullptr;
}
