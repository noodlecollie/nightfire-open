#pragma once

#include "weapon_ronin.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_projectileattack.h"
#include "weaponatts_eventattack.h"

#ifndef CLIENT_DLL
#include "npc/npc_ronin_turret.h"
#endif

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

enum VRoninAttackMode_e
{
	VRONIN_ATTACKMODE_TOSS = 0,
	VRONIN_ATTACKMODE_PLACE,
	VRONIN_ATTACKMODE_DEPLOY,
	VRONIN_ATTACKMODE_DETONATE,
};

static constexpr CAmmoDef Ammo_RoninThrow = {
	"ammo_ronin_throw",  // ClassName
	"ammodef_ronin_throw",  // AmmoName
	1,  // MaxCarry
	1  // AmmoBoxGive
};

static constexpr CAmmoDef Ammo_RoninDeploy = {
	"ammo_ronin_deploy",  // ClassName
	"ammodef_ronin_deploy",  // AmmoName
	1,  // MaxCarry
	0  // AmmoBoxGive
};

static constexpr uint8_t RONIN_HOLD_AMMO = 1;

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_ronin";
		core.Id = WeaponId_e::WeaponRonin;
		core.SwitchWeight = WeaponPref_Ronin;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_RoninThrow;
		ammo.SecondaryAmmo = &Ammo_RoninDeploy;
		ammo.MaxClip = -1;
		ammo.PrimaryAmmoOnFirstPickup = RONIN_HOLD_AMMO;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_ronin/v_ronin.mdl";
		vm.Animations.Anim_Draw = VRONIN_CASE_DRAW;
		vm.Animations.AnimList_Idle << VRONIN_CASE_IDLE1 << VRONIN_CASE_IDLE2 << VRONIN_CASE_IDLE3;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_ronin/p_ronin.mdl";
		pm.WorldModelName = "models/weapon_ronin/w_ronin.mdl";
		pm.PlayerAnimExtension = "ronin";

		obj.Prediction.SetUpPrediction<CWeaponRonin>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_ronin_bullet", &skilldata_t::plrDmgRoninBullet));
		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_ronin_explosion", &skilldata_t::plrDmgRoninExplosion));

#ifndef CLIENT_DLL
		obj.CustomCvars.AddToTail(&sv_ronin_slide_friction);
#endif

		{
			WAProjectileAttack* throwAttack = new WAProjectileAttack();
			obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(throwAttack));

			throwAttack->EventScript = "events/weapon_ronin/throw.sc";
			throwAttack->FunctionsUnderwater = true;
			throwAttack->IsContinuous = false;
			throwAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
			throwAttack->AttackRate = 1.0f;
			throwAttack->Volume = QUIET_GUN_VOLUME;
			throwAttack->MuzzleFlashBrightness = NO_GUN_FLASH;
			throwAttack->ViewPunchY = 0.0f;
			throwAttack->PlayDryFireSoundOnEmpty = false;
			throwAttack->ProjectileDelay = 0.5f;

			throwAttack->ViewModelAttackSounds.MinPitch = 98;
			throwAttack->ViewModelAttackSounds.MinPitch = 102;
			throwAttack->ViewModelAttackSounds.SoundNames << "weapons/weapon_ronin/toss.wav";

			AccuracyParameters& accuracy = throwAttack->Accuracy;
			accuracy.RestSpread = Vector2D(0.0f, 0.0f);
			accuracy.RunSpread = Vector2D(0.0f, 0.0f);

			CrosshairParameters& crosshair = throwAttack->Crosshair;
			crosshair.RenderStyle = CrosshairStyle::None;

			throwAttack->ViewModelAnimList_Attack << VRONIN_CASE_THROW;
			throwAttack->ViewModelAnimList_AttackEmpty << VRONIN_CASE_THROW;

			// This can't be make_shared() because calling that causes a very weird
			// crash on Linux *only* when starting a multiplayer game.
			// Yes, this was very annoying to track down, and no I don't know why
			// it does that.
			throwAttack->OverrideAnimations = std::shared_ptr<ViewModelAnimationSet>(new ViewModelAnimationSet());
			*throwAttack->OverrideAnimations = vm.Animations;
		}

		{
			WAEventAttack* placeAttack = new WAEventAttack();
			obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(placeAttack));

			placeAttack->EventScript = "events/weapon_ronin/place.sc";
			placeAttack->FunctionsUnderwater = true;
			placeAttack->IsContinuous = false;
			placeAttack->Volume = QUIET_GUN_VOLUME;
			placeAttack->PlayDryFireSoundOnEmpty = false;
			placeAttack->AttackRate = 3.0f;

			// deployAttack->ViewModelAttackSounds.MinPitch = 100;
			// deployAttack->ViewModelAttackSounds.MinPitch = 100;
			// deployAttack->ViewModelAttackSounds.SoundNames << "weapons/weapon_ronin/remote_activate.wav";

			CrosshairParameters& crosshair = placeAttack->Crosshair;
			crosshair.RenderStyle = CrosshairStyle::None;

			placeAttack->ViewModelAnimList_Attack << VRONIN_CASE_DEPLOY;
			placeAttack->ViewModelAnimList_AttackEmpty << VRONIN_CASE_DEPLOY;

			placeAttack->OverrideAnimations = std::shared_ptr<ViewModelAnimationSet>(new ViewModelAnimationSet());
			*placeAttack->OverrideAnimations = vm.Animations;
		}

		{
			WAEventAttack* deployAttack = new WAEventAttack();
			obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(deployAttack));

			deployAttack->EventScript = "events/weapon_ronin/deploy.sc";
			deployAttack->FunctionsUnderwater = true;
			deployAttack->IsContinuous = false;
			deployAttack->Volume = QUIET_GUN_VOLUME;
			deployAttack->PlayDryFireSoundOnEmpty = false;
			deployAttack->AttackRate = 0.5f;

			deployAttack->ViewModelAttackSounds.MinPitch = 100;
			deployAttack->ViewModelAttackSounds.MinPitch = 100;
			deployAttack->ViewModelAttackSounds.SoundNames << "weapons/weapon_ronin/remote_activate.wav";

			CrosshairParameters& crosshair = deployAttack->Crosshair;
			crosshair.RenderStyle = CrosshairStyle::None;

			deployAttack->ViewModelAnimList_Attack << VRONIN_DETONATOR_FIRE_SENTRY;
			deployAttack->ViewModelAnimList_AttackEmpty << VRONIN_DETONATOR_FIRE_SENTRY;

			ViewModelAnimationSet* overrideAnimations = new ViewModelAnimationSet();
			deployAttack->OverrideAnimations = std::shared_ptr<ViewModelAnimationSet>(overrideAnimations);

			overrideAnimations->Anim_Draw = VRONIN_DETONATOR_DRAW;
			overrideAnimations->AnimList_Idle << VRONIN_DETONATOR_IDLE1 << VRONIN_DETONATOR_IDLE2;
		}
	});
