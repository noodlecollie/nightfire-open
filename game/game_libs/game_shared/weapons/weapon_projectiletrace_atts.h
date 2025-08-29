#pragma once

#include "weapon_projectiletrace.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_eventattack.h"

enum ProjectileTraceWeaponAnimations_e
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

enum ProjectileTraceWeaponAttackMode_e
{
	ATTACKMODE_NORMAL = 0
};

// Rounds per second:
static constexpr float PROJECTILETRACEWEAPON_FIRE_RATE = 6.0f;

static cvar_t debug_projectiletrace_launch_speed =
	CONSTRUCT_CVAR_T("debug_projectiletrace_launch_speed", 500, FCVAR_CHEAT);
static cvar_t debug_projectiletrace_gravity = CONSTRUCT_CVAR_T("debug_projectiletrace_gravity", 800, FCVAR_CHEAT);
static cvar_t debug_projectiletrace_max_iterations =
	CONSTRUCT_CVAR_T("debug_projectiletrace_max_iterations", 32, FCVAR_CHEAT);
static cvar_t debug_projectiletrace_time_step = CONSTRUCT_CVAR_T("debug_projectiletrace_time_step", 0.05, FCVAR_CHEAT);

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_projectiletrace";
		core.Id = WeaponId_e::WeaponProjectileTrace;
		core.SwitchWeight = WeaponPref_Debug;
		core.Flags = WeaponFlag_IsDebug;

		obj.CustomCvars.AddToTail(&debug_projectiletrace_launch_speed);
		obj.CustomCvars.AddToTail(&debug_projectiletrace_gravity);
		obj.CustomCvars.AddToTail(&debug_projectiletrace_max_iterations);
		obj.CustomCvars.AddToTail(&debug_projectiletrace_time_step);

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_p99/v_p99.mdl";
		vm.Animations.Anim_Draw = HTW_DRAW;
		vm.Animations.AnimList_Idle << HTW_IDLE1 << HTW_IDLE2 << HTW_IDLE3;
		vm.Animations.AnimList_Reload << HTW_RELOAD_NOT_EMPTY;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_p99/p_p99.mdl";
		pm.WorldModelName = "models/weapon_p99/w_p99.mdl";
		pm.PlayerAnimExtension = "pistol";

		obj.Prediction.SetUpPrediction<CWeaponProjectileTrace>();

		WAEventAttack* priAttack = new WAEventAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/debugweapon/fire.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = true;
		priAttack->AttackRate = PROJECTILETRACEWEAPON_FIRE_RATE;
		priAttack->Volume = QUIET_GUN_VOLUME;

		priAttack->ViewModelAnimList_Attack << HTW_SHOOT;
	});
