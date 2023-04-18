#pragma once

#include "weapon_grenadelauncher.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_projectileattack.h"

enum GrenadeLauncherAnimations_e
{
	GRENADELAUNCHER_IDLE1 = 0,
	GRENADELAUNCHER_IDLE2,
	GRENADELAUNCHER_FIRE,
	GRENADELAUNCHER_RELOAD,
	GRENADELAUNCHER_DRAW,
	GRENADELAUNCHER_HOLSTER
};

enum GrenadeLauncherAttackMode_e
{
	ATTACKMODE_CONTACT,
	ATTACKMODE_TIMED
};

// Grenades per second:
static constexpr float GRENADELAUNCHER_FIRE_RATE = 1.0f / 0.8f;

// Dynamic attributes for tuning purposes
#define DEV_CVAR(name, value) static cvar_t name = { #name, value, FCVAR_SPONLY }
DEV_CVAR(grenadelauncher_explosion_radius, "250");
DEV_CVAR(grenadelauncher_fuse_time, "4");
DEV_CVAR(grenadelauncher_launch_speed, "1000");
DEV_CVAR(grenadelauncher_launch_pitch_adjust, "5");
#undef DEV_CVAR

static constexpr CAmmoDef Ammo_GrenadeLauncher =
{
	"ammo_grenadelauncher",	// ClassName
	"ammodef_grenadelauncher",	// AmmoName
	12,	// MaxCarry
	6	// AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes([](WeaponAtts::WACollection& obj)
{
	using namespace WeaponAtts;

	WACore& core = obj.Core;
	core.Classname = "weapon_grenadelauncher";
	core.Id = WeaponId_e::WeaponGrenadeLauncher;
	core.SwitchWeight = WeaponPref_GrenadeLauncher;

	WAAmmoDef& ammo = obj.Ammo;
	ammo.PrimaryAmmo = &Ammo_GrenadeLauncher;
	ammo.MaxClip = 6;
	ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;

	WAViewModel& vm = obj.ViewModel;
	vm.ModelName = "models/weapon_grenadelauncher/v_grenadelauncher.mdl";
	vm.Anim_Draw = GRENADELAUNCHER_DRAW;
	vm.AnimList_Idle << GRENADELAUNCHER_IDLE1 << GRENADELAUNCHER_IDLE2;
	vm.AnimList_Reload << GRENADELAUNCHER_RELOAD;

	WAPlayerModel& pm = obj.PlayerModel;
	pm.PlayerModelName = "models/weapon_grenadelauncher/p_grenadelauncher.mdl";
	pm.WorldModelName = "models/weapon_grenadelauncher/w_grenadelauncher.mdl";
	pm.PlayerAnimExtension = "gauss";

	obj.Prediction.SetUpPrediction<CWeaponGrenadeLauncher>();

	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_grenadelauncher", &skilldata_t::plrDmgGrenadeLauncher));
	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_selfdmg_mult_grenadelauncher", &skilldata_t::plrSelfDmgMultGrenadeLauncher));
	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_mult_grenadelauncher_hit", &skilldata_t::plrDmgMultGrenadelauncherHit));

	obj.CustomCvars.AddToTail(&grenadelauncher_explosion_radius);
	obj.CustomCvars.AddToTail(&grenadelauncher_fuse_time);
	obj.CustomCvars.AddToTail(&grenadelauncher_launch_speed);
	obj.CustomCvars.AddToTail(&grenadelauncher_launch_pitch_adjust);

	// Explode on contact
	WAProjectileAttack* priAttack = new WAProjectileAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

	priAttack->EventScript = "events/weapon_grenadelauncher/fire_impact.sc";
	priAttack->FunctionsUnderwater = true;
	priAttack->IsContinuous = false;
	priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
	priAttack->AttackRate = GRENADELAUNCHER_FIRE_RATE;
	priAttack->Volume = LOUD_GUN_VOLUME;
	priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
	priAttack->ViewPunchY = -4.0f;

	AccuracyParameters& accuracy = priAttack->Accuracy;
	accuracy.RestSpread = Vector2D(0.1f, 0.1f);
	accuracy.RunSpread = Vector2D(0.1f, 0.1f);

	priAttack->ViewModelAnimList_Attack << GRENADELAUNCHER_FIRE;

	priAttack->AttackSounds.MinVolume = 0.9f;
	priAttack->AttackSounds.MaxVolume = 0.9f;
	priAttack->AttackSounds.MinPitch = 96;
	priAttack->AttackSounds.MaxPitch = 100;
	priAttack->AttackSounds.SoundNames << "weapons/weapon_grenadelauncher/grenade_launcher_fire.wav";

	CrosshairParameters& crosshair = priAttack->Crosshair;
	crosshair.BarScaleMin = 0.07f;
	crosshair.BarScaleMax = 0.07f;
	crosshair.RadiusMin = 0.14f;
	crosshair.RadiusMax = 0.14f;

	// Explode after delay
	WAProjectileAttack* secAttack = new WAProjectileAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

	// Base off primary attack
	*secAttack = *priAttack;
	secAttack->EventScript = "events/weapon_grenadelauncher/fire_timed.sc";
});
