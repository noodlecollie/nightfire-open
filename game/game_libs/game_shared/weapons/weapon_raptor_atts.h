#pragma once

#include "weapon_raptor.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum RaptorAnimations_e
{
	RAPTOR_IDLE1 = 0,
	RAPTOR_IDLE2,
	RAPTOR_IDLE3,
	RAPTOR_IDLE4,
	RAPTOR_SHOOT,
	RAPTOR_RELOAD_NOT_EMPTY,
	RAPTOR_SHOOT_LAST,
	RAPTOR_RELOAD_EMPTY,
	RAPTOR_IDLE1_EMPTY,
	RAPTOR_IDLE2_EMPTY,
	RAPTOR_IDLE3_EMPTY,
	RAPTOR_IDLE4_EMPTY,
	RAPTOR_DRAW,
	RAPTOR_HOLSTER
};

enum RaptorAttackMode_e
{
	ATTACKMODE_NORMAL = 0
};

// Rounds per second:
static constexpr float RAPTOR_FIRE_RATE = 4.0f;

static constexpr CAmmoDef Ammo_Raptor = {
	"ammo_raptor",  // ClassName
	"ammodef_raptor",  // AmmoName
	72,  // MaxCarry
	9  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_raptor";
		core.Id = WeaponId_e::WeaponRaptor;
		core.SwitchWeight = WeaponPref_Raptor;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_Raptor;
		ammo.MaxClip = 9;
		ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;
		ammo.PickupClassnames[0] = Ammo_Raptor.ClassName;

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
		ammo.PickupClassnames[1] = "ammo_pdw90";
#endif  // NFOPEN_GAMEPLAY_PLACEHOLDERS

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_raptor/v_raptor.mdl";
		vm.Animations.Anim_Draw = RAPTOR_DRAW;
		vm.Animations.AnimList_Idle << RAPTOR_IDLE1 << RAPTOR_IDLE2 << RAPTOR_IDLE3 << RAPTOR_IDLE4;
		vm.Animations.AnimList_Reload << RAPTOR_RELOAD_NOT_EMPTY;
		vm.Animations.AnimList_ReloadEmpty << RAPTOR_RELOAD_EMPTY;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_raptor/p_raptor.mdl";
		pm.WorldModelName = "models/weapon_raptor/w_raptor.mdl";
		pm.PlayerAnimExtension = "pistol";

		obj.Prediction.SetUpPrediction<CWeaponRaptor>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_raptor", &skilldata_t::plrDmgRaptor));

		WAHitscanAttack* priAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_raptor/fire.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = RAPTOR_FIRE_RATE;
		priAttack->BaseDamagePerShot = &skilldata_t::plrDmgRaptor;
		priAttack->AutoAim = AUTOAIM_10DEGREES;
		priAttack->Volume = LOUD_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
		priAttack->ViewPunchY = -3.0f;
		priAttack->ShellModelName = "models/shell.mdl";

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestValue = 0.2f;
		accuracy.RestSpread = Vector2D(0.003f, 0.003f);
		accuracy.RunValue = 0.3f;
		accuracy.RunSpread = Vector2D(0.015f, 0.015f);
		accuracy.CrouchShift = -0.03f;
		accuracy.AirShift = 0.05f;
		accuracy.FallShift = 0.025f;
		accuracy.AttackCoefficient = 0.3f;
		accuracy.DecayCoefficient = 0.4f;
		accuracy.FireImpulse = 0.2f;
		accuracy.FireImpulseCeiling = 0.3f;
		accuracy.FireImpulseHoldTime = 0.1f;
		accuracy.FireImpulseDecayWindow = 0.5f;
		accuracy.FireImpulseDecayMod = 0.05f;

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.BarScaleMin = 0.02f;
		crosshair.BarScaleMax = 0.02f;
		crosshair.RadiusMin = 0.01f;
		crosshair.RadiusMax = 0.025f;

		priAttack->ViewModelAnimList_Attack << RAPTOR_SHOOT;
		priAttack->ViewModelAnimList_AttackEmpty << RAPTOR_SHOOT_LAST;

		priAttack->AttackSounds.MinVolume = 0.87f;
		priAttack->AttackSounds.MaxVolume = 0.9f;
		priAttack->AttackSounds.MinPitch = 95;
		priAttack->AttackSounds.MaxPitch = 100;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_raptor/raptor_fire1.wav";

		WABotInterface& botIfc = obj.BotInterface;
		botIfc.Type = BotWeaponType::HitscanSingleShot;
		botIfc.Preference = BotWeaponPreference::High;

		WABotAttackMode* botPrimaryAttackMode = new WABotAttackMode();
		botIfc.PrimaryAttackMode.reset(botPrimaryAttackMode);
		botPrimaryAttackMode->ApplyMode(priAttack);
		botPrimaryAttackMode->ApplyAmmo(*ammo.PrimaryAmmo, ammo.PrimaryAmmoOnFirstPickup, ammo.MaxClip);
		botPrimaryAttackMode->EnemyAimAt = BotEnemyAimAt::Body;
		botPrimaryAttackMode->MinEffectiveRange = 0.0f;
		botPrimaryAttackMode->MaxEffectiveRange = 1280.0f;
		botPrimaryAttackMode->MinExtraDelayBetweenShots = 0.25f;
	}
);
