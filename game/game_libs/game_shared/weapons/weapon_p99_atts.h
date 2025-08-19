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
	P99_BODY_UNSILENCED = 0,
	P99_BODY_SILENCED
};

enum P99AttackMode_e
{
	P99_ATTACKMODE_UNSILENCED = 0,
	P99_ATTACKMODE_SILENCED
};

// Rounds per second:
static constexpr float P99_FIRE_RATE = 6.0f;

static constexpr CAmmoDef Ammo_P99 = {
	"ammo_p99",  // ClassName
	AMMOPOOL_9MM_NAME,  // AmmoName
	AMMOPOOL_9MM_MAXCARRY,  // MaxCarry
	AMMOPOOL_9MM_AMMOBOXGIVE  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
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
		ammo.PickupClassnames[0] = Ammo_P99.ClassName;

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
		ammo.PickupClassnames[1] = "ammo_kowloon";
#endif  // NFOPEN_GAMEPLAY_PLACEHOLDERS

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_p99/v_p99.mdl";
		vm.Animations.Anim_Draw = P99_DRAW;
		vm.Animations.AnimList_Idle << P99_IDLE1 << P99_IDLE2 << P99_IDLE3;
		vm.Animations.AnimList_Reload << P99_RELOAD_NOT_EMPTY;
		vm.Animations.AnimList_ReloadEmpty << P99_RELOAD_EMPTY;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_p99/p_p99.mdl";
		pm.WorldModelName = "models/weapon_p99/w_p99.mdl";
		pm.PlayerAnimExtension = "pistol";

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
		priAttack->ReloadDuration = 2.0f;

		// Weapon play style: accurate, reliable sidearm which doesn't do
		// a ton of damage.
		// - Spread is generally very low, affected little by speed and jumping.
		// - Each shot applies a small penalty, so shooting lots in quick
		//   successsion decreases accuracy.

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestValue = 0.12f;
		accuracy.RestSpread = Vector2D(0.007f, 0.007f);
		accuracy.RunValue = 0.4f;
		accuracy.RunSpread = Vector2D(0.03f, 0.03f);
		accuracy.CrouchShift = -0.08f;
		accuracy.AirShift = 0.06f;
		accuracy.FallShift = 0.06f;
		accuracy.AttackCoefficient = 0.3f;
		accuracy.DecayCoefficient = 0.3f;
		accuracy.FireImpulse = 0.02f;
		accuracy.FireImpulseCeiling = 0.2f;
		accuracy.FireImpulseHoldTime = 0.2f;
		accuracy.FireImpulseDecayWindow = 0.0f;
		accuracy.FireImpulseDecayMod = 1.0f;

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.BarScaleMin = 0.02f;
		crosshair.BarScaleMax = 0.02f;
		crosshair.RadiusMin = 0.01f;
		crosshair.RadiusMax = 0.02f;

		priAttack->ViewModelAnimList_Attack << P99_SHOOT;
		priAttack->ViewModelAnimList_AttackEmpty << P99_SHOOT_EMPTY;
		priAttack->ViewModelBodyOverride = P99_BODY_UNSILENCED;

		priAttack->AttackSounds.MinVolume = 0.82f;
		priAttack->AttackSounds.MaxVolume = 0.9f;
		priAttack->AttackSounds.MinPitch = 98;
		priAttack->AttackSounds.MaxPitch = 104;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_p99/p99_fire1.wav";

		// Silenced
		WAHitscanAttack* secAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

		// Base off primary attack.
		*secAttack = *priAttack;
		secAttack->EventScript = "events/weapon_p99/fire_silenced.sc";
		secAttack->Volume = QUIET_GUN_VOLUME;
		secAttack->MuzzleFlashBrightness = DIM_GUN_FLASH;

		secAttack->ViewModelBodyOverride = P99_BODY_SILENCED;

		secAttack->AttackSounds.MinVolume = 0.76f;
		secAttack->AttackSounds.MaxVolume = 0.8f;
		secAttack->AttackSounds.SoundNames.Clear();
		secAttack->AttackSounds.SoundNames << "weapons/weapon_p99/p99_fire_sil1.wav";

		WABotInterface& botIfc = obj.BotInterface;
		botIfc.Type = BotWeaponType::HitscanSingleShot;
		botIfc.Preference = BotWeaponPreference::Low;

		WABotAttackMode* botAttackMode = new WABotAttackMode();
		botIfc.PrimaryAttackMode.reset(botAttackMode);
		botAttackMode->ApplyMode(priAttack);
		botAttackMode->ApplyAmmo(*ammo.PrimaryAmmo, ammo.PrimaryAmmoOnFirstPickup, ammo.MaxClip);
		botAttackMode->EnemyAimAt = BotEnemyAimAt::Body;
	});
