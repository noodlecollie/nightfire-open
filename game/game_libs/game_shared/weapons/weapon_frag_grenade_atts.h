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

enum FragGrenadeAttackMode_e
{
	ATTACKMODE_THROW,
};

// Delay between throwing one grenade and being able
// to pull the pin on the next one.
static constexpr float FRAGGRENADE_REFIRE_DELAY_SECS = 1.1f;

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
		ammo.PickupClassnames[0] = Ammo_FragGrenade.ClassName;

		// Different spelling, for Nightfire compatibility.
		ammo.PickupClassnames[1] = "ammo_fraggrenade";

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_frag_grenade/v_frag_grenade.mdl";
		vm.Animations.Anim_Draw = FRAGGRENADE_DRAW;
		vm.Animations.AnimList_Idle << FRAGGRENADE_IDLE1 << FRAGGRENADE_IDLE2 << FRAGGRENADE_IDLE3;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_frag_grenade/p_frag_grenade.mdl";
		pm.WorldModelName = "models/weapon_frag_grenade/w_frag_grenade.mdl";
		pm.PlayerAnimExtension = "grenade";

		obj.Prediction.SetUpPrediction<CWeaponFragGrenade>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_fraggrenade", &skilldata_t::plrDmgFragGrenade));
		obj.SkillRecords.AddToTail(
			WASkillRecord("sk_plr_selfdmg_mult_fraggrenade", &skilldata_t::plrSelfDmgMultFragGrenade));

		WAProjectileAttack* priAttack = new WAProjectileAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_frag_grenade/throw.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = 1.0f / FRAGGRENADE_REFIRE_DELAY_SECS;
		priAttack->Volume = QUIET_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = NO_GUN_FLASH;
		priAttack->ViewPunchY = 0.0f;
		priAttack->PlayDryFireSoundOnEmpty = false;
		priAttack->ProjectileDelay = 0.5f;
		priAttack->ProjectileModelName = "models/weapon_frag_grenade/w_frag_grenade.mdl";

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestSpread = Vector2D(0.1f, 0.1f);
		accuracy.RunSpread = Vector2D(0.1f, 0.1f);

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.RenderStyle = CrosshairStyle::None;

		priAttack->ViewModelAnimList_Attack << FRAGGRENADE_THROW;
		priAttack->ViewModelAnimList_AttackEmpty << FRAGGRENADE_THROW_LAST;
	});
