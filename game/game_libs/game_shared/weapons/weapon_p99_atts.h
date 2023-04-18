#pragma once

#include "weapon_p99.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum P99Animations_e
{
	P99_IDLE1 = 0,
	P99_IDLE2,
	P99_IDLE3,
	P99_SHOOT,
	P99_SHOOT_EMPTY,
	P99_RELOAD_EMPTY,
	P99_RELOAD_NOT_EMPTY,
	P99_DRAW,
	P99_HOLSTER,
	P99_ADD_SILENCER,
	P99_REMOVE_SILENCER,
	P99_SHOOT_SILENCER,
	P99_SHOOT_SILENCER_EMPTY,
};

enum P99Body_e
{
	P99BODY_UNSILENCED = 0,
	P99BODY_SILENCED
};

enum P99AttackMode_e
{
	ATTACKMODE_UNSILENCED = 0,
	ATTACKMODE_SILENCED
};

// Rounds per second:
static constexpr float P99_FIRE_RATE = 6.0f;

static constexpr CAmmoDef Ammo_P99 =
{
	"ammo_p99",	// ClassName
	"ammodef_p99",	// AmmoName
	200,	// MaxCarry
	32	// AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes([](WeaponAtts::WACollection& obj)
{
	using namespace WeaponAtts;

	WACore& core = obj.Core;
	core.Classname = "weapon_p99";
	core.Id = WeaponId_e::WeaponP99;
	core.SwitchWeight = WeaponPref_P99;

	WAAmmoDef& ammo = obj.Ammo;
	ammo.PrimaryAmmo = &Ammo_P99;
	ammo.MaxClip = 16;
	ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;

	WAViewModel& vm = obj.ViewModel;
	vm.ModelName = "models/weapon_p99/v_p99.mdl";
	vm.Anim_Draw = P99_DRAW;
	vm.AnimList_Idle << P99_IDLE1 << P99_IDLE2 << P99_IDLE3;
	vm.AnimList_Reload << P99_RELOAD_NOT_EMPTY;
	vm.AnimList_ReloadEmpty << P99_RELOAD_EMPTY;

	WAPlayerModel& pm = obj.PlayerModel;
	pm.PlayerModelName = "models/weapon_p99/p_p99.mdl";
	pm.WorldModelName = "models/weapon_p99/w_p99.mdl";
	pm.PlayerAnimExtension = "onehanded";

	obj.Prediction.SetUpPrediction<CWeaponP99>();

	obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_p99", &skilldata_t::plrDmgP99));

	// Unsilenced
	WAHitscanAttack* priAttack = new WAHitscanAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

	priAttack->EventScript = "events/weapon_p99/fire_unsilenced.sc";
	priAttack->FunctionsUnderwater = true;
	priAttack->IsContinuous = false;
	priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
	priAttack->AttackRate = P99_FIRE_RATE;
	priAttack->BaseDamagePerShot = &skilldata_t::plrDmgP99;
	priAttack->AutoAim = AUTOAIM_10DEGREES;
	priAttack->Volume = NORMAL_GUN_VOLUME;
	priAttack->MuzzleFlashBrightness = NORMAL_GUN_FLASH;
	priAttack->ViewPunchY = -2.0f;
	priAttack->ShellModelName = "models/shell.mdl";

	AccuracyParameters& accuracy = priAttack->Accuracy;
	accuracy.RestValue = 0.1f;
	accuracy.RestSpread = Vector2D(0.01f, 0.01f);
	accuracy.RunValue = 0.5f;
	accuracy.RunSpread = Vector2D(0.03f, 0.03f);
	accuracy.CrouchShift = -0.08f;
	accuracy.AirShift = 0.2f;
	accuracy.FallShift = 0.1f;
	accuracy.AttackCoefficient = 0.3f;
	accuracy.DecayCoefficient = 0.3f;
	accuracy.FireImpulse = 0.3f;
	accuracy.FireImpulseCeiling = 0.5f;
	accuracy.FireImpulseHoldTime = 0.05f;

	priAttack->ViewModelAnimList_Attack << P99_SHOOT;
	priAttack->ViewModelAnimList_AttackEmpty << P99_SHOOT_EMPTY;
	priAttack->ViewModelBodyOverride = P99BODY_UNSILENCED;

	priAttack->AttackSounds.MinVolume = 0.82f;
	priAttack->AttackSounds.MaxVolume = 0.9f;
	priAttack->AttackSounds.MinPitch = 98;
	priAttack->AttackSounds.MaxPitch = 104;
	priAttack->AttackSounds.SoundNames << "weapons/weapon_p99/p99_fire1.wav";

	CrosshairParameters& crosshair = priAttack->Crosshair;
	crosshair.BarScaleMin = 0.03f;
	crosshair.BarScaleMax = 0.03f;
	crosshair.RadiusMin = 0.015f;
	crosshair.RadiusMax = 0.04f;

	// Silenced
	WAHitscanAttack* secAttack = new WAHitscanAttack();
	obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

	// Base off primary attack.
	*secAttack = *priAttack;
	secAttack->EventScript = "events/weapon_p99/fire_silenced.sc";
	secAttack->Volume = QUIET_GUN_VOLUME;
	secAttack->MuzzleFlashBrightness = DIM_GUN_FLASH;

	secAttack->ViewModelBodyOverride = P99BODY_SILENCED;

	secAttack->AttackSounds.MinVolume = 0.76;
	secAttack->AttackSounds.MaxVolume = 0.8;
	secAttack->AttackSounds.SoundNames.Clear();
	secAttack->AttackSounds.SoundNames << "weapons/weapon_p99/p99_fire_sil1.wav";
});
