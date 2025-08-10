#pragma once

#include "weapon_silent.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_eventattack.h"

enum HullTraceWeaponAnimations_e
{
	HTW_IDLE1 = 0,
	HTW_IDLE2,
	HTW_IDLE3,
	HTW_SHOOT,
	HTW_SHOOT_EMPTY,
	HTW_RELOAD_EMPTY,
	HTW_RELOAD_NOT_EMPTY,
	HTW_DRAW,
	HTW_HOLSTER,
	HTW_ADD_SILENCER,
	HTW_REMOVE_SILENCER,
	HTW_SHOOT_SILENCER,
	HTW_SHOOT_SILENCER_EMPTY,
};

enum HullTraceWeaponAttackMode_e
{
	ATTACKMODE_NORMAL = 0
};

// Rounds per second:
static constexpr float HULLTRACEWEAPON_FIRE_RATE = 6.0f;

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_hulltrace";
		core.Id = WeaponId_e::WeaponHullTrace;
		core.SwitchWeight = WeaponPref_Debug;
		core.Flags = WeaponFlag_IsDebug;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_p99/v_p99.mdl";
		vm.Animations.Anim_Draw = HTW_DRAW;
		vm.Animations.AnimList_Idle << HTW_IDLE1 << HTW_IDLE2 << HTW_IDLE3;
		vm.Animations.AnimList_Reload << HTW_RELOAD_NOT_EMPTY;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_p99/p_p99.mdl";
		pm.WorldModelName = "models/weapon_p99/w_p99.mdl";
		pm.PlayerAnimExtension = "pistol";

		obj.Prediction.SetUpPrediction<CWeaponSilent>();

		WAEventAttack* priAttack = new WAEventAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/debugweapon/fire.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = true;
		priAttack->AttackRate = HULLTRACEWEAPON_FIRE_RATE;
		priAttack->Volume = QUIET_GUN_VOLUME;

		priAttack->ViewModelAnimList_Attack << HTW_SHOOT;
	});
