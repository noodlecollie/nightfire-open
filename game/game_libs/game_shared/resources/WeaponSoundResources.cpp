#include "WeaponSoundResources.h"

static constexpr const char* const BulletRicochetSounds[] = {
	"weapons/ric1.wav",
	"weapons/ric2.wav",
	"weapons/ric3.wav",
	"weapons/ric4.wav",
	"weapons/ric5.wav",
};

static constexpr const char* const ExplosionSounds[] = {
	"weapons/explosion1.wav",
	"weapons/explosion2.wav",
	"weapons/explosion3.wav",
	"weapons/explosion4.wav",
};

CWeaponSoundResources::CWeaponSoundResources() :
	CBaseResourceCollection<WeaponSoundId, static_cast<size_t>(WeaponSoundId::Count)>()
{
	InitialiseResources(WeaponSoundId::BulletRicochet, BulletRicochetSounds);
	InitialiseResource(WeaponSoundId::GrenadeBounce, "weapons/grenade_hit1.wav");
	InitialiseResources(WeaponSoundId::Explosion, ExplosionSounds);
}
