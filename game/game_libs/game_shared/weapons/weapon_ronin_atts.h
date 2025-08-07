#pragma once

#include "weapon_ronin.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_projectileattack.h"
#include "weaponatts_eventattack.h"
#include "weapons/ronin_common_defs.h"

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

static constexpr CAmmoDef Ammo_Ronin = {
	"ammo_ronin",  // ClassName
	"ammodef_ronin",  // AmmoName
	1,  // MaxCarry
	1  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_ronin";
		core.Id = WeaponId_e::WeaponRonin;
		core.SwitchWeight = WeaponPref_Ronin;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_Ronin;
		ammo.MaxClip = -1;
		ammo.PrimaryAmmoOnFirstPickup = 1;
		ammo.PickupClassnames[0] = Ammo_Ronin.AmmoName;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_ronin/v_ronin.mdl";
		vm.Animations.Anim_Draw = VRONIN_CASE_DRAW;
		vm.Animations.AnimList_Idle << VRONIN_CASE_IDLE1 << VRONIN_CASE_IDLE2 << VRONIN_CASE_IDLE3;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_ronin/p_ronin.mdl";
		pm.WorldModelName = "models/weapon_ronin/w_ronin.mdl";
		pm.WorldModelSkin = NPCRONIN_SKIN_NO_LIGHTS;
		pm.WorldModelBody = NPCRONIN_BODY_NO_LIGHTS;
		pm.PlayerAnimExtension = "ronin";

		obj.Precache.Classes.AddToTail("npc_ronin_turret");

		obj.Prediction.SetUpPrediction<CWeaponRonin>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_ronin_bullet", &skilldata_t::plrDmgRoninBullet));
		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_ronin_explosion", &skilldata_t::plrDmgRoninExplosion));
		obj.SkillRecords.AddToTail(WASkillRecord("sk_ronin_base_health", &skilldata_t::roninBaseHealth));

#ifndef CLIENT_DLL
		obj.CustomCvars.AddToTail(&sv_ronin_slide_friction);
		obj.CustomCvars.AddToTail(&sv_ronin_death_delay);
		obj.CustomCvars.AddToTail(&sv_ronin_place_fov);
		obj.CustomCvars.AddToTail(&debug_ronin_placement);
#endif

		{
			WAProjectileAttack* throwAttack = new WAProjectileAttack();
			obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(throwAttack));

			throwAttack->EventScript = "events/weapon_ronin/throw.sc";
			throwAttack->FunctionsUnderwater = true;
			throwAttack->IsContinuous = false;
			// This is None because we manage the ammo manually.
			throwAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::None;
			throwAttack->AttackRate = 1.0f / 2.5f;
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

			placeAttack->ViewModelAttackSounds.MinPitch = 100;
			placeAttack->ViewModelAttackSounds.MinPitch = 100;
			placeAttack->ViewModelAttackSounds.MinVolume = 0.6f;
			placeAttack->ViewModelAttackSounds.MaxVolume = 0.6f;
			placeAttack->ViewModelAttackSounds.SoundNames << "weapons/weapon_ronin/holster.wav";

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

			// 1 / time this attack takes to execute
			deployAttack->AttackRate = 1.0f / 2.0f;

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

		{
			WAEventAttack* detonateAttack = new WAEventAttack();
			obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(detonateAttack));

			detonateAttack->EventScript = "events/weapon_ronin/detonate.sc";
			detonateAttack->FunctionsUnderwater = true;
			detonateAttack->IsContinuous = false;
			detonateAttack->Volume = QUIET_GUN_VOLUME;
			detonateAttack->PlayDryFireSoundOnEmpty = false;

			// 1 / time this attack takes to execute
			detonateAttack->AttackRate = 1.0f / 3.96f;

			detonateAttack->ViewModelAttackSounds.MinPitch = 100;
			detonateAttack->ViewModelAttackSounds.MinPitch = 100;
			detonateAttack->ViewModelAttackSounds.SoundNames << "weapons/weapon_ronin/remote_detonate.wav";

			CrosshairParameters& crosshair = detonateAttack->Crosshair;
			crosshair.RenderStyle = CrosshairStyle::None;

			detonateAttack->ViewModelAnimList_Attack << VRONIN_DETONATOR_FIRE_EXPLODE;
			detonateAttack->ViewModelAnimList_AttackEmpty << VRONIN_DETONATOR_FIRE_EXPLODE;

			ViewModelAnimationSet* overrideAnimations = new ViewModelAnimationSet();
			detonateAttack->OverrideAnimations = std::shared_ptr<ViewModelAnimationSet>(overrideAnimations);

			overrideAnimations->Anim_Draw = VRONIN_DETONATOR_DRAW;
			overrideAnimations->AnimList_Idle << VRONIN_DETONATOR_IDLE1 << VRONIN_DETONATOR_IDLE2;
		}
	});
