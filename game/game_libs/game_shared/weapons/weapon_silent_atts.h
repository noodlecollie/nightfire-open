#pragma once

#include "weapon_silent.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_hitscanattack.h"

enum SilentWeaponAnimations_e
{
	SW_IDLE1 = 0,
	SW_IDLE2,
	SW_IDLE3,
	SW_SHOOT,
	SW_SHOOT_EMPTY,
	SW_RELOAD_EMPTY,
	SW_RELOAD_NOT_EMPTY,
	SW_DRAW,
	SW_HOLSTER,
	SW_ADD_SILENCER,
	SW_REMOVE_SILENCER,
	SW_SHOOT_SILENCER,
	SW_SHOOT_SILENCER_EMPTY,
};

enum SilentWeaponAttackMode_e
{
	ATTACKMODE_NORMAL = 0
};

// Rounds per second:
static constexpr float SILENTWEAPON_FIRE_RATE = 6.0f;

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_silent";
		core.Id = WeaponId_e::WeaponSilent;
		core.SwitchWeight = WeaponPref_Debug;
		core.Flags = WeaponFlag_IsDebug;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_p99/v_p99.mdl";
		vm.Animations.Anim_Draw = SW_DRAW;
		vm.Animations.AnimList_Idle << SW_IDLE1 << SW_IDLE2 << SW_IDLE3;
		vm.Animations.AnimList_Reload << SW_RELOAD_NOT_EMPTY;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_p99/p_p99.mdl";
		pm.WorldModelName = "models/weapon_p99/w_p99.mdl";
		pm.PlayerAnimExtension = "pistol";

		obj.Prediction.SetUpPrediction<CWeaponSilent>();

		WAHitscanAttack* priAttack = new WAHitscanAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/debugweapon/fire.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::None;
		priAttack->Accuracy.RestSpread = Vector2D(0.0f, 0.0f);
		priAttack->Accuracy.RunSpread = Vector2D(0.0f, 0.0f);
		priAttack->AttackRate = SILENTWEAPON_FIRE_RATE;
		priAttack->AutoAim = AUTOAIM_10DEGREES;
		priAttack->Volume = QUIET_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = NORMAL_GUN_FLASH;
		priAttack->ViewPunchY = -2.0f;

		priAttack->ViewModelAnimList_Attack << SW_SHOOT;
	});
