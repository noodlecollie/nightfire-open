#pragma once

#include "weapon_ronin.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_projectileattack.h"

enum VRoninAnimations_e
{
	VRONIN_CASE_IDLE1 = 0,
	VRONIN_CASE_IDLE2,
	VRONIN_CASE_IDLE3,
	VRONIN_CASE_PULL,
	VRONIN_CASE_THROW,
	VRONIN_CASE_DEPLOY,
	VRONIN_CASE_DRAW,
	VRONIN_CASE_HOLSTER,
	VRONIN_DETONATOR_IDLE1,
	VRONIN_DETONATOR_IDLE2,
	VRONIN_DETONATOR_FIRE_SENTRY,
	VRONIN_DETONATOR_FIRE_EXPLODE,
	VRONIN_DETONATOR_DRAW,
	VRONIN_DETONATOR_HOLSTER
};

static constexpr CAmmoDef Ammo_Ronin = {
	"ammo_ronin",  // ClassName
	"ammodef_ronin",  // AmmoName
	1,  // MaxCarry
	1  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_ronin";
		core.Id = WeaponId_e::WeaponRonin;
		core.SwitchWeight = WeaponPref_Ronin;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_Ronin;
		ammo.MaxClip = 1;
		ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_ronin/v_ronin.mdl";
		vm.Anim_Draw = VRONIN_CASE_DRAW;
		vm.AnimList_Idle << VRONIN_CASE_IDLE1 << VRONIN_CASE_IDLE2 << VRONIN_CASE_IDLE3;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_ronin/p_ronin.mdl";
		pm.WorldModelName = "models/weapon_ronin/w_ronin.mdl";
		pm.PlayerAnimExtension = "ronin";

		obj.Prediction.SetUpPrediction<CWeaponRonin>();

		// obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_grenadelauncher", &skilldata_t::plrDmgGrenadeLauncher));
		// obj.SkillRecords.AddToTail(
		// 	WASkillRecord("sk_plr_selfdmg_mult_grenadelauncher", &skilldata_t::plrSelfDmgMultGrenadeLauncher));
		// obj.SkillRecords.AddToTail(
		// 	WASkillRecord("sk_plr_dmg_mult_grenadelauncher_hit", &skilldata_t::plrDmgMultGrenadelauncherHit));

		// // Explode on contact
		// WAProjectileAttack* priAttack = new WAProjectileAttack();
		// obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		// priAttack->EventScript = "events/weapon_grenadelauncher/fire_impact.sc";
		// priAttack->FunctionsUnderwater = true;
		// priAttack->IsContinuous = false;
		// priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		// priAttack->AttackRate = GRENADELAUNCHER_FIRE_RATE;
		// priAttack->Volume = LOUD_GUN_VOLUME;
		// priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
		// priAttack->ViewPunchY = -4.0f;

		// AccuracyParameters& accuracy = priAttack->Accuracy;
		// accuracy.RestSpread = Vector2D(0.1f, 0.1f);
		// accuracy.RunSpread = Vector2D(0.1f, 0.1f);

		// CrosshairParameters& crosshair = priAttack->Crosshair;
		// crosshair.RenderStyle = CrosshairStyle::Circle;
		// crosshair.RadiusMin = 0.07f;
		// crosshair.RadiusMax = 0.07f;

		// priAttack->ViewModelAnimList_Attack << GRENADELAUNCHER_FIRE;

		// priAttack->AttackSounds.MinVolume = 0.9f;
		// priAttack->AttackSounds.MaxVolume = 0.9f;
		// priAttack->AttackSounds.MinPitch = 96;
		// priAttack->AttackSounds.MaxPitch = 100;
		// priAttack->AttackSounds.SoundNames << "weapons/weapon_grenadelauncher/grenade_launcher_fire.wav";

		// // Explode after delay
		// WAProjectileAttack* secAttack = new WAProjectileAttack();
		// obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

		// // Base off primary attack
		// *secAttack = *priAttack;
		// secAttack->EventScript = "events/weapon_grenadelauncher/fire_timed.sc";
	});
