#pragma once

#include "weapon_fists.h"
#include "weaponatts_collection.h"
#include "weapon_pref_weights.h"
#include "weaponatts_meleeattack.h"

enum FistsAnimations_e
{
	FISTS_IDLE1 = 0,
	FISTS_JAB,
	FISTS_COMBO,
	FISTS_KARATE_CHOP
};

enum FistsAttackMode_e
{
	ATTACKMODE_PUNCH = 0,
	ATTACKMODE_PUNCH_COMBO,
	ATTACKMODE_KARATE_CHOP
};

// Cycles per second:
static constexpr float FISTS_PUNCH_RATE_SINGLE = 2.0f;
static constexpr float FISTS_PUNCH_RATE_DOUBLE = 1.0f;
static constexpr float FISTS_KARATE_CHOP_RATE = 1.0f / 1.2f;

static const WeaponAtts::WACollection StaticWeaponAttributes(
	[](WeaponAtts::WACollection& obj)
	{
		using namespace WeaponAtts;

		WACore& core = obj.Core;
		core.Classname = "weapon_fists";
		core.Id = WeaponId_e::WeaponFists;
		core.SwitchWeight = WeaponPref_Fists;

		WAViewModel& vm = obj.ViewModel;
		vm.ModelName = "models/weapon_fists/v_fists.mdl";
		vm.Animations.Anim_Draw = FISTS_IDLE1;
		vm.Animations.AnimList_Idle << FISTS_IDLE1;

		WAPlayerModel& pm = obj.PlayerModel;
		pm.PlayerAnimExtension = "dukes";

		obj.Prediction.SetUpPrediction<CWeaponFists>();

		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_fists", &skilldata_t::plrDmgFists));
		obj.SkillRecords.AddToTail(WASkillRecord("sk_plr_dmg_fists_alt", &skilldata_t::plrDmgFistsAlt));

		WAMeleeAttack* priAttack = new WAMeleeAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priAttack));

		priAttack->EventScript = "events/weapon_fists/punch.sc";
		priAttack->FunctionsUnderwater = true;
		priAttack->IsContinuous = true;
		priAttack->AttackRate = FISTS_PUNCH_RATE_SINGLE;
		priAttack->BaseDamagePerHit = &skilldata_t::plrDmgFists;
		priAttack->DecalOnImpact = false;
		priAttack->Strikes.AddToTail(7.0f / 40.0f);
		priAttack->Volume = 128;
		priAttack->ViewModelAnimList_Attack << FISTS_JAB;
		priAttack->Crosshair.RenderStyle = CrosshairStyle::None;

		priAttack->WorldHitSounds.MinPitch = 97;
		priAttack->WorldHitSounds.MaxPitch = 103;
		priAttack->WorldHitSounds.SoundNames << "weapons/weapon_fists/impact1.wav"
											 << "weapons/weapon_fists/impact2.wav"
											 << "weapons/weapon_fists/impact3.wav";

		priAttack->AttackSounds.MinPitch = 97;
		priAttack->AttackSounds.MaxPitch = 103;
		priAttack->AttackSounds.SoundNames << "weapons/weapon_fists/swing1.wav"
										   << "weapons/weapon_fists/swing2.wav"
										   << "weapons/weapon_fists/swing3.wav";

		WAMeleeAttack* priDoubleAttack = new WAMeleeAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(priDoubleAttack));

		*priDoubleAttack = *priAttack;
		priDoubleAttack->EventScript = "events/weapon_fists/punch_double.sc";
		priDoubleAttack->AttackRate = FISTS_PUNCH_RATE_DOUBLE;

		priDoubleAttack->Strikes.Purge();
		priDoubleAttack->Strikes.AddToTail(0.1f);
		priDoubleAttack->Strikes.AddToTail(0.4f);

		priDoubleAttack->ViewModelAnimList_Attack.Clear();
		priDoubleAttack->ViewModelAnimList_Attack << FISTS_COMBO;

		priDoubleAttack->AttackSounds.SoundNames.Clear();
		priDoubleAttack->AttackSounds.SoundNames << "weapons/weapon_fists/swing_double.wav";

		WAMeleeAttack* secAttack = new WAMeleeAttack();
		obj.AttackModes.AddToTail(std::shared_ptr<WABaseAttack>(secAttack));

		*secAttack = *priAttack;
		secAttack->EventScript = "events/weapon_fists/karate_chop.sc";
		secAttack->AttackRate = FISTS_KARATE_CHOP_RATE;
		secAttack->BaseDamagePerHit = &skilldata_t::plrDmgFistsAlt;

		secAttack->Strikes.Purge();
		secAttack->Strikes.AddToTail(0.35f);

		secAttack->ViewModelAnimList_Attack.Clear();
		secAttack->ViewModelAnimList_Attack << FISTS_KARATE_CHOP;

		WABotInterface& botIfc = obj.BotInterface;
		botIfc.Type = BotWeaponType::Melee;
		botIfc.Preference = BotWeaponPreference::Lowest;

		WABotAttackMode* botPriAttackMode = new WABotAttackMode();
		botIfc.PrimaryAttackMode.reset(botPriAttackMode);
		botPriAttackMode->ApplyMode(priAttack);
		botPriAttackMode->EnemyAimAt = BotEnemyAimAt::Body;
		botPriAttackMode->MinEffectiveRange = 0.0f;
		botPriAttackMode->MaxEffectiveRange = 32.0f;
	}
);
