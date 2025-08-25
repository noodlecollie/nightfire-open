#pragma once

#include "weapon_mp9.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum MP9Animations_e
{
	MP9_2HANDED_IDLE1 = 0,
	MP9_2HANDED_IDLE2,
	MP9_2HANDED_IDLE3,
	MP9_2HANDED_FIRE,
	MP9_2HANDED_ALT_FIRE,
	MP9_2HANDED_RELOAD_NOT_EMPTY,
	MP9_2HANDED_RELOAD_EMPTY,
	MP9_2HANDED_DRAW,
	MP9_2HANDED_HOLSTER,
	MP9_IDLE1,
	MP9_IDLE2,
	MP9_IDLE3,
	MP9_FIRE,
	MP9_ALT_FIRE,
	MP9_RELOAD_NOT_EMPTY,
	MP9_RELOAD_EMPTY,
	MP9_DRAW,
	MP9_HOLSTER,
};

enum MP9AttackMode_e
{
	MP9_ATTACKMODE_NORMAL = 0
};

// Rounds per second:
static constexpr float MP9_FIRE_RATE = 13.0f;

static constexpr CAmmoDef Ammo_MP9 = {
	"ammo_mp9",  // ClassName
	AMMOPOOL_9MM_NAME,  // AmmoName
	AMMOPOOL_9MM_MAXCARRY,  // MaxCarry
	AMMOPOOL_9MM_AMMOBOXGIVE  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_mp9";
		core.Id = WeaponId_e::WeaponMP9;
		core.SwitchWeight = WeaponPref_MP9;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_MP9;
		ammo.MaxClip = 32;
		ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;
		ammo.PickupClassnames[0] = Ammo_MP9.ClassName;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_mp9/v_mp9.mdl";
		vm.Animations.Anim_Draw = MP9_2HANDED_DRAW;
		vm.Animations.AnimList_Idle << MP9_2HANDED_IDLE1 << MP9_2HANDED_IDLE2 << MP9_2HANDED_IDLE3;
		vm.Animations.AnimList_Reload << MP9_2HANDED_RELOAD_NOT_EMPTY;
		vm.Animations.AnimList_ReloadEmpty << MP9_2HANDED_RELOAD_EMPTY;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_mp9/p_mp9.mdl";
		pm.WorldModelName = "models/weapon_mp9/w_mp9.mdl";
		pm.PlayerAnimExtension = "mp9";

		obj.Prediction.SetUpPrediction<CWeaponMP9>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_mp9", &skilldata_t::plrDmgMP9));

		// Unsilenced
		WAHitscanAttack* priAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_mp9/fire.sc";
		priAttack->FunctionsUnderwater = false;
		priAttack->IsContinuous = true;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = MP9_FIRE_RATE;
		priAttack->BaseDamagePerShot = &skilldata_t::plrDmgMP9;
		priAttack->AutoAim = AUTOAIM_5DEGREES;
		priAttack->Volume = NORMAL_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = NORMAL_GUN_FLASH;
		priAttack->ViewPunchY = -2.0f;
		priAttack->ShellModelName = "models/shell.mdl";

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestValue = 0.12f;
		accuracy.RestSpread = Vector2D(0.007f, 0.007f);
		accuracy.RunValue = 0.4f;
		accuracy.RunSpread = Vector2D(0.04f, 0.04f);
		accuracy.CrouchShift = -0.08f;
		accuracy.AirShift = 0.06f;
		accuracy.FallShift = 0.06f;
		accuracy.AttackCoefficient = 0.2f;
		accuracy.DecayCoefficient = 0.2f;
		accuracy.FireImpulse = 0.02f;
		accuracy.FireImpulseCeiling = 0.3f;
		accuracy.FireImpulseHoldTime = 0.1f;
		accuracy.FireImpulseDecayWindow = 0.0f;
		accuracy.FireImpulseDecayMod = 1.0f;

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.BarScaleMin = 0.02f;
		crosshair.BarScaleMax = 0.02f;
		crosshair.RadiusMin = 0.01f;
		crosshair.RadiusMax = 0.025f;

		priAttack->ViewModelAnimList_Attack << MP9_2HANDED_FIRE;

		priAttack->AttackSounds.MinVolume = 0.82f;
		priAttack->AttackSounds.MaxVolume = 0.9f;
		priAttack->AttackSounds.MinPitch = 98;
		priAttack->AttackSounds.MaxPitch = 104;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_mp9/mp9_fire1.wav";

		WABotInterface& botIfc = obj.BotInterface;
		botIfc.Type = BotWeaponType::HitscanContinuous;
		botIfc.Preference = BotWeaponPreference::Normal;

		WABotAttackMode* botPrimaryAttackMode = new WABotAttackMode();
		botIfc.PrimaryAttackMode.reset(botPrimaryAttackMode);
		botPrimaryAttackMode->ApplyMode(priAttack);
		botPrimaryAttackMode->ApplyAmmo(*ammo.PrimaryAmmo, ammo.PrimaryAmmoOnFirstPickup, ammo.MaxClip);
		botPrimaryAttackMode->EnemyAimAt = BotEnemyAimAt::Body;
		botPrimaryAttackMode->MinEffectiveRange = 0.0f;
		botPrimaryAttackMode->MaxEffectiveRange = 1024.0f;
	}
);
