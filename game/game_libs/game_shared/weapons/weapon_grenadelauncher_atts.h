#pragma once

#include "weapon_grenadelauncher.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_projectileattack.h"

enum GrenadeLauncherAnimations_e
{
	GRENADELAUNCHER_IDLE1 = 0,
	GRENADELAUNCHER_IDLE2,
	GRENADELAUNCHER_FIRE,
	GRENADELAUNCHER_RELOAD,
	GRENADELAUNCHER_DRAW,
	GRENADELAUNCHER_HOLSTER
};

enum GrenadeLauncherAttackMode_e
{
	ATTACKMODE_CONTACT,
	ATTACKMODE_TIMED
};

// Grenades per second:
static constexpr float GRENADELAUNCHER_FIRE_RATE = 1.0f / 0.8f;

static constexpr CAmmoDef Ammo_GrenadeLauncher = {
	"ammo_grenadelauncher",  // ClassName
	"ammodef_grenadelauncher",  // AmmoName
	12,  // MaxCarry
	6  // AmmoBoxGive
};

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_grenadelauncher";
		core.Id = WeaponId_e::WeaponGrenadeLauncher;
		core.SwitchWeight = WeaponPref_GrenadeLauncher;

		WAAmmoDef& ammo = obj.Ammo;
		ammo.PrimaryAmmo = &Ammo_GrenadeLauncher;
		ammo.MaxClip = 6;
		ammo.PrimaryAmmoOnFirstPickup = ammo.MaxClip;
		ammo.PickupClassnames[0] = Ammo_GrenadeLauncher.ClassName;

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
		ammo.PickupClassnames[1] = "ammo_rocketlauncher";
#endif  // NFOPEN_GAMEPLAY_PLACEHOLDERS

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_grenadelauncher/v_grenadelauncher.mdl";
		vm.Animations.Anim_Draw = GRENADELAUNCHER_DRAW;
		vm.Animations.AnimList_Idle << GRENADELAUNCHER_IDLE1 << GRENADELAUNCHER_IDLE2;
		vm.Animations.AnimList_Reload << GRENADELAUNCHER_RELOAD;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerModelName = "models/weapon_grenadelauncher/p_grenadelauncher.mdl";
		pm.WorldModelName = "models/weapon_grenadelauncher/w_grenadelauncher.mdl";
		pm.PlayerAnimExtension = "grenadelauncher";

		obj.Prediction.SetUpPrediction<CWeaponGrenadeLauncher>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_grenadelauncher", &skilldata_t::plrDmgGrenadeLauncher));
		obj.SkillRecords.AddToTail(
			WASkillRecord("sk_plr_selfdmg_mult_grenadelauncher", &skilldata_t::plrSelfDmgMultGrenadeLauncher)
		);
		obj.SkillRecords.AddToTail(
			WASkillRecord("sk_plr_dmg_mult_grenadelauncher_hit", &skilldata_t::plrDmgMultGrenadelauncherHit)
		);

		// Explode on contact
		WAProjectileAttack* priAttack = new WAProjectileAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_grenadelauncher/fire_impact.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = false;
		priAttack->UsesAmmoPool = WAAmmoBasedAttack::AmmoPool::Primary;
		priAttack->AttackRate = GRENADELAUNCHER_FIRE_RATE;
		priAttack->Volume = LOUD_GUN_VOLUME;
		priAttack->MuzzleFlashBrightness = BRIGHT_GUN_FLASH;
		priAttack->ViewPunchY = -4.0f;
		priAttack->ProjectileModelName = "models/weapon_grenadelauncher/w_grenade_projectile.mdl";
		priAttack->LaunchSpeed = 1000.0f;

		AccuracyParameters& accuracy = priAttack->Accuracy;
		accuracy.RestSpread = Vector2D(0.1f, 0.1f);
		accuracy.RunSpread = Vector2D(0.1f, 0.1f);

		CrosshairParameters& crosshair = priAttack->Crosshair;
		crosshair.RenderStyle = CrosshairStyle::Circle;
		crosshair.RadiusMin = 0.07f;
		crosshair.RadiusMax = 0.07f;

		priAttack->ViewModelAnimList_Attack << GRENADELAUNCHER_FIRE;

		priAttack->AttackSounds.MinVolume = 0.9f;
		priAttack->AttackSounds.MaxVolume = 0.9f;
		priAttack->AttackSounds.MinPitch = 96;
		priAttack->AttackSounds.MaxPitch = 100;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_grenadelauncher/grenade_launcher_fire.wav";

		// Explode after delay
		WAProjectileAttack* secAttack = new WAProjectileAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

		// Base off primary attack
		*secAttack = *priAttack;
		secAttack->EventScript = "events/weapon_grenadelauncher/fire_timed.sc";

		WABotInterface& botIfc = obj.BotInterface;
		botIfc.Type = BotWeaponType::GrenadeProjectile;
		botIfc.Preference = BotWeaponPreference::Highest;

		WABotAttackMode* botPriAttackMode = new WABotAttackMode();
		botIfc.PrimaryAttackMode.reset(botPriAttackMode);
		botPriAttackMode->ApplyMode(priAttack);
		botPriAttackMode->ApplyAmmo(*ammo.PrimaryAmmo, ammo.PrimaryAmmoOnFirstPickup, ammo.MaxClip);
		botPriAttackMode->EnemyAimAt = BotEnemyAimAt::Foot;
		botPriAttackMode->MinEffectiveRange = 384.0f;
		botPriAttackMode->MaxEffectiveRange = 896.0f;
	}
);
