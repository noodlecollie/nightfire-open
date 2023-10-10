#pragma once

#include "weapon_frag_grenade.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_projectileattack.h"

enum FragGrenadeAnimations_e
{
	FRAGGRENADE_IDLE1 = 0,
	FRAGGRENADE_IDLE2,
	FRAGGRENADE_IDLE3,
	FRAGGRENADE_PULL,
	FRAGGRENADE_THROW,
	FRAGGRENADE_THROW_LAST,
	FRAGGRENADE_DRAW,
	FRAGGRENADE_HOLSTER
};

enum FragGrenadeBody_e
{
	FRAGGRENADE_THROWN = 0,
	FRAGGRENADE_HELD
};

enum FragGrenadeAttackMode_e
{
	ATTACKMODE_THROW,
};

// Grenades per second:
static constexpr float FRAGGRENADE_FIRE_RATE = 1.0f / 2.0f;

static constexpr CAmmoDef Ammo_FragGrenade = {
	"ammo_frag_grenade",  // ClassName
	"ammodef_frag_grenade",  // AmmoName
	10,  // MaxCarry
	2  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_frag_grenade";
		core.Id = WeaponId_e::WeaponFragGrenade;
		core.SwitchWeight = WeaponPref_FragGrenade;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_FragGrenade;
		ammo.PrimaryAmmoOnFirstPickup = ammo.PrimaryAmmo->AmmoBoxGive;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_frag_grenade/v_frag_grenade.mdl";
		vm.Anim_Draw = FRAGGRENADE_DRAW;
		vm.AnimList_Idle << FRAGGRENADE_IDLE1 << FRAGGRENADE_IDLE2 << FRAGGRENADE_IDLE3;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_frag_grenade/p_frag_grenade.mdl";
		pm.WorldModelName = "models/weapon_frag_grenade/w_frag_grenade.mdl";
		pm.PlayerAnimExtension = "crowbar";

		obj.Prediction.SetUpPrediction<CWeaponFragGrenade>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_fraggrenade", &skilldata_t::plrDmgFragGrenade));
		obj.SkillRecords.AddToTail(
			WASkillRecord("sk_plr_selfdmg_mult_fraggrenade", &skilldata_t::plrSelfDmgMultFragGrenade));

		// Explode on contact
		WAProjectileAttack* priAttack = new WAProjectileAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_frag_grenade/throw.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = FRAGGRENADE_FIRE_RATE;
		priAttack->Volume = LOUD_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
		priAttack->ViewPunchY = -4.0f;

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestSpread = Vector2D(0.1f, 0.1f);
		accuracy.RunSpread = Vector2D(0.1f, 0.1f);

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.RenderStyle = CrosshairStyle::None;

		priAttack->ViewModelAnimList_Attack << FRAGGRENADE_THROW;
		priAttack->ViewModelAnimList_AttackEmpty << FRAGGRENADE_THROW_LAST;
	});
