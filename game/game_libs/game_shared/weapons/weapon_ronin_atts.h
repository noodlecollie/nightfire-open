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

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_ronin_bullet", &skilldata_t::plrDmgRoninBullet));
		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_ronin_explosion", &skilldata_t::plrDmgRoninExplosion));

		obj.CustomCvars.AddToTail(WACustomCvar::Number("ronin_search_range", "300"));

		WAProjectileAttack* priAttack = new WAProjectileAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_ronin/throw.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = 1.0f;
		priAttack->Volume = QUIET_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = NO_GUN_FLASH;
		priAttack->ViewPunchY = 0.0f;
		priAttack->PlayDryFireSoundOnEmpty = false;
		priAttack->projectileDelay = 0.5f;

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestSpread = Vector2D(0.0f, 0.0f);
		accuracy.RunSpread = Vector2D(0.0f, 0.0f);

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.RenderStyle = CrosshairStyle::None;

		priAttack->ViewModelAnimList_Attack << VRONIN_CASE_THROW;
		priAttack->ViewModelAnimList_AttackEmpty << VRONIN_CASE_THROW;
	});
