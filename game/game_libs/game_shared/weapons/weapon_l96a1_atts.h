#pragma once

#include "weapon_l96a1.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"
#include "coreutil/coreutil.h"

enum L96A1Animations_e
{
	L96A1_IDLE1,
	L96A1_IDLE2,
	L96A1_FIRE,
	L96A1_FIRE_LAST,
	L96A1_RELOAD,
	L96A1_RELOAD_CHARGE,
	L96A1_HOLSTER,
	L96A1_DRAW,
};

enum L9A61AttackMode_e
{
	ATTACKMODE_UNSCOPED = 0,
	ATTACKMODE_SCOPED
};

// Rounds per second:
static constexpr float L96A1_FIRE_RATE = 1.0f / 1.5f;

static constexpr float L96A1_ZOOM_TOGGLE_DELAY = 0.2f;
static constexpr const char* L96A1_ZOOM_IN_SOUND = "weapons/weapon_l96a1/l96a1_zoom_in.wav";
static constexpr const char* L96A1_ZOOM_OUT_SOUND = "weapons/weapon_l96a1/l96a1_zoom_out.wav";

static constexpr float L96A1_ZOOM_LEVELS[] = {
	0.0f,  // Default
	40.0f,
	15.0f,
};

static constexpr size_t L96A1_ZOOM_LEVEL_COUNT = CoreUtil::ArraySize(L96A1_ZOOM_LEVELS);

static constexpr CAmmoDef Ammo_L96A1 = {
	"ammo_l96a1",  // ClassName
	"ammodef_l96a1",  // AmmoName
	50,  // MaxCarry
	10  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_l96a1";
		core.Id = WeaponId_e::WeaponL96A1;
		core.SwitchWeight = WeaponPref_L96A1;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_L96A1;
		ammo.MaxClip = 10;
		ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;
		ammo.PickupClassnames[0] = Ammo_L96A1.ClassName;
		ammo.PickupClassnames[1] = "ammo_sniper";

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_l96a1/v_l96a1.mdl";
		vm.Animations.Anim_Draw = L96A1_DRAW;
		vm.Animations.AnimList_Idle << L96A1_IDLE1 << L96A1_IDLE2;
		vm.Animations.AnimList_Reload << L96A1_RELOAD;
		vm.Animations.AnimList_ReloadEmpty << L96A1_RELOAD_CHARGE;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_l96a1/p_l96a1.mdl";
		pm.WorldModelName = "models/weapon_l96a1/w_l96a1.mdl";
		pm.PlayerAnimExtension = "sniper_rifle";

		obj.Prediction.SetUpPrediction<CWeaponL96A1>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_l96a1", &skilldata_t::plrDmgL96A1));

		WAHitscanAttack* priAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_l96a1/fire_unscoped.sc";
		priAttack->FunctionsUnderwater = false;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = L96A1_FIRE_RATE;
		priAttack->BaseDamagePerShot = &skilldata_t::plrDmgL96A1;
		priAttack->AutoAim = AUTOAIM_10DEGREES;
		priAttack->Volume = LOUD_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
		priAttack->ViewPunchY = -7.0f;
		priAttack->ShellModelName = "models/shell.mdl";

		priAttack->ViewModelAnimList_Attack << L96A1_FIRE;
		priAttack->ViewModelAnimList_AttackEmpty << L96A1_FIRE_LAST;

		priAttack->AttackSounds.MinVolume = 0.87f;
		priAttack->AttackSounds.MaxVolume = 0.9f;
		priAttack->AttackSounds.MinPitch = 95;
		priAttack->AttackSounds.MaxPitch = 100;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_l96a1/l96a1_fire_distant.wav";

		priAttack->ViewModelAttackSounds.MinVolume = 0.87f;
		priAttack->ViewModelAttackSounds.MaxVolume = 0.9f;
		priAttack->ViewModelAttackSounds.MinPitch = 95;
		priAttack->ViewModelAttackSounds.MaxPitch = 100;
		priAttack->ViewModelAttackSounds.SoundNames << "weapons/weapon_l96a1/l96a1_fire.wav";

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestValue = 0.1f;
		accuracy.RestSpread = Vector2D(0.01f, 0.01f);
		accuracy.RunValue = 0.5f;
		accuracy.RunSpread = Vector2D(0.035f, 0.035f);
		accuracy.CrouchShift = -0.08f;
		accuracy.AirShift = 0.2f;
		accuracy.FallShift = 0.1f;
		accuracy.AttackCoefficient = 0.3f;
		accuracy.DecayCoefficient = 0.3f;
		accuracy.FireImpulse = 0.1f;
		accuracy.FireImpulseCeiling = 0.3f;
		accuracy.FireImpulseHoldTime = 0.05f;
		accuracy.FireImpulseDecayWindow = 0.0f;
		accuracy.FireImpulseDecayMod = 1.0f;

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.RenderStyle = CrosshairStyle::None;

		WAHitscanAttack* scopedAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(scopedAttack));

		// Base off primary attack
		*scopedAttack = *priAttack;

		scopedAttack->EventScript = "events/weapon_l96a1/fire_scoped.sc";
		scopedAttack->Accuracy.RestSpread = Vector2D(0.0f, 0.0f);
		scopedAttack->Accuracy.RunSpread = Vector2D(0.02f, 0.02f);
		scopedAttack->ViewPunchY = -3.0f;

		// TODO: Need to support multiple zoom levels before we can allow bot use.
		// NB. Max range for deepest zoom is probably around 3000.
	}
);
