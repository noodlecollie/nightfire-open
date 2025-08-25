#pragma once

#include "weapon_frinesi.h"
#include "weaponatts_botinterface.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum FrinesiAnimations_e
{
	FRINESI_IDLE1 = 0,
	FRINESI_SHOOT,
	FRINESI_SHOOT_BIG,
	FRINESI_RELOAD,
	FRINESI_PUMP,
	FRINESI_START_RELOAD,
	FRINESI_DRAW,
	FRINESI_HOLSTER,
	FRINESI_IDLE2,
	FRINESI_IDLE3,
	FRINESI_IDLE4
};

enum FrinesiAttackMode_e
{
	ATTACKMODE_AUTO = 0,
	ATTACKMODE_PUMP
};

// Shots per second:
static constexpr float FRINESI_FIRE_RATE_PUMP = 1.0f;
static constexpr float FRINESI_FIRE_RATE_AUTO = 4.0f;

static constexpr uint8_t FRINESI_PELLETS_PER_SHOT = 6;
static constexpr const char* FRINESI_COCK_SOUND = "weapons/weapon_frinesi/frinesi_cock.wav";

// These used to be based on the lengths of the animations, but are
// now hard-coded so that gameplay doesn't change if the player
// changes the length of their viewmodel animations.
static constexpr float FRINESI_RELOAD_START_DELAY = 0.33f;
static constexpr float FRINESI_SHELL_RELOAD_DELAY = 0.48f;
static constexpr float FRINESI_PUMP_DELAY = 0.875f;

static constexpr float FRINESI_RELOAD_SOUND_OFFSET = 0.1f;
static constexpr float FRINESI_PUMP_SOUND_OFFSET_WHEN_RELOADING = 0.08f;
static constexpr float FRINESI_PUMP_SOUND_OFFSET_AFTER_FIRING = 0.45f;

static constexpr CAmmoDef Ammo_Frinesi = {
	"ammo_frinesi",  // ClassName
	"ammodef_frinesi",  // AmmoName
	125,  // MaxCarry
	72  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_frinesi";
		core.Id = WeaponId_e::WeaponFrinesi;
		core.SwitchWeight = WeaponPref_Frinesi;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_Frinesi;
		ammo.MaxClip = 8;
		ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;
		ammo.PickupClassnames[0] = Ammo_Frinesi.ClassName;
		ammo.PickupClassnames[1] = "ammo_shotgun";

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
		ammo.PickupClassnames[2] = "ammo_commando";
		ammo.PickupClassnames[3] = "ammo_mini";
#endif  // NFOPEN_GAMEPLAY_PLACEHOLDERS

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_frinesi/v_frinesi.mdl";
		vm.Animations.Anim_Draw = FRINESI_DRAW;
		vm.Animations.AnimList_Idle << FRINESI_IDLE1 << FRINESI_IDLE2 << FRINESI_IDLE3 << FRINESI_IDLE4;
		vm.Animations.AnimList_Reload << FRINESI_START_RELOAD;

		vm.Animations.ReloadSounds.MinVolume = 0.9f;
		vm.Animations.ReloadSounds.MaxVolume = 0.9f;
		vm.Animations.ReloadSounds.MinPitch = 98;
		vm.Animations.ReloadSounds.MaxPitch = 102;
		vm.Animations.ReloadSounds.SoundNames << "weapons/weapon_frinesi/frinesi_reload1.wav"
											  << "weapons/weapon_frinesi/frinesi_reload2.wav"
											  << "weapons/weapon_frinesi/frinesi_reload3.wav"
											  << "weapons/weapon_frinesi/frinesi_reload4.wav";

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_frinesi/p_frinesi.mdl";
		pm.WorldModelName = "models/weapon_frinesi/w_frinesi.mdl";
		pm.PlayerAnimExtension = "shotgun";

		obj.Prediction.SetUpPrediction<CWeaponFrinesi>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_frinesi_auto", &skilldata_t::plrDmgFrinesiAuto));
		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_frinesi_pump", &skilldata_t::plrDmgFrinesiPump));

		// From Nightfire:
		// - 6 bullets per shot in either mode
		// - 81 damage over all 6 shots in auto mode = 13.5 per shot
		// - 159 damage over all 6 shots in pump mode = 26.5 per shot

		// Auto
		WAHitscanAttack* priAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_frinesi/fire_auto.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = FRINESI_FIRE_RATE_AUTO;
		priAttack->BaseDamagePerShot = &skilldata_t::plrDmgFrinesiAuto;
		priAttack->BulletsPerShot = 6;
		priAttack->AutoAim = AUTOAIM_10DEGREES;
		priAttack->Volume = LOUD_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = NORMAL_GUN_FLASH;
		priAttack->ViewPunchY = -5.0f;
		priAttack->ShellModelName = "models/shotgunshell.mdl";
		priAttack->ShellModelType = ShellType::Shotgun;
		priAttack->SpecialReload = true;

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestValue = 0.2f;
		accuracy.RestSpread = Vector2D(0.04f, 0.04f);
		accuracy.RunValue = 0.4f;
		accuracy.RunSpread = Vector2D(0.1f, 0.1f);
		accuracy.CrouchShift = -0.03f;
		accuracy.FallShift = 0.15f;
		accuracy.AirShift = 0.15f;
		accuracy.AttackCoefficient = 0.3f;
		accuracy.DecayCoefficient = 0.2f;
		accuracy.FireImpulse = 0.2f;
		accuracy.FireImpulseCeiling = 0.6f;
		accuracy.FireImpulseHoldTime = 0.1f;
		accuracy.FireImpulseDecayWindow = 0.5f;
		accuracy.FireImpulseDecayMod = 0.1f;

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.RadiusMin = 0.04f;
		crosshair.RadiusMax = 0.12f;
		crosshair.BarScaleMin = 0.03f;
		crosshair.BarScaleMax = 0.03f;

		priAttack->ViewModelAnimList_Attack << FRINESI_SHOOT;

		priAttack->AttackSounds.MinVolume = 0.85f;
		priAttack->AttackSounds.MaxVolume = 0.9f;
		priAttack->AttackSounds.MinPitch = 100;
		priAttack->AttackSounds.MaxPitch = 102;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_frinesi/frinesi_fire1.wav";

		// Pump
		WAHitscanAttack* secAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

		// Base off primary attack.
		*secAttack = *priAttack;
		secAttack->EventScript = "events/weapon_frinesi/fire_pump.sc";
		secAttack->AttackRate = FRINESI_FIRE_RATE_PUMP;
		secAttack->BaseDamagePerShot = &skilldata_t::plrDmgFrinesiPump;
		secAttack->ViewPunchY = -10.0f;

		secAttack->Accuracy.RestSpread = Vector2D(0.1f, 0.1f);
		secAttack->Accuracy.RunSpread = Vector2D(0.17f, 0.17f);

		secAttack->ViewModelAnimList_Attack.Clear();
		secAttack->ViewModelAnimList_Attack << FRINESI_SHOOT_BIG;

		secAttack->AttackSounds.MinPitch = 90;
		secAttack->AttackSounds.MaxPitch = 94;

		WABotInterface& botIfc = obj.BotInterface;
		botIfc.Type = BotWeaponType::Shotgun;
		botIfc.Preference = BotWeaponPreference::High;

		WABotAttackMode* botPriAttackMode = new WABotAttackMode();
		botIfc.PrimaryAttackMode.reset(botPriAttackMode);
		botPriAttackMode->ApplyMode(priAttack);
		botPriAttackMode->ApplyAmmo(*ammo.PrimaryAmmo, ammo.PrimaryAmmoOnFirstPickup, ammo.MaxClip);
		botPriAttackMode->EnemyAimAt = BotEnemyAimAt::Body;
		botPriAttackMode->ReloadStyle = BotWeaponReloadStyle::PerShot;
		botPriAttackMode->MinEffectiveRange = 0.0f;
		botPriAttackMode->MaxEffectiveRange = 640.0f;

		WABotAttackMode* botSecAttackMode = new WABotAttackMode();
		botIfc.SecondaryAttackMode.reset(botSecAttackMode);
		botSecAttackMode->ApplyMode(secAttack);
		botSecAttackMode->ApplyAmmo(*ammo.PrimaryAmmo, ammo.PrimaryAmmoOnFirstPickup, ammo.MaxClip);
		botSecAttackMode->EnemyAimAt = BotEnemyAimAt::Body;
		botSecAttackMode->ReloadStyle = BotWeaponReloadStyle::PerShot;
		botSecAttackMode->MinEffectiveRange = 0.0f;
		botSecAttackMode->MaxEffectiveRange = 384.0f;
	}
);
