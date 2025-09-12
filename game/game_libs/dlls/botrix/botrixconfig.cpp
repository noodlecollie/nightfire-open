#include <cstdlib>

#include <good/file.h>
#include <good/string_buffer.h>
#include <good/string_utils.h>

#include "botrix/botrixconfig.h"
#include "BuildPlatform/Utils.h"
#include "botrix/defines.h"
#include "botrix/botrixmod.h"
#include "botrix/type2string.h"
#include "botrix/engine_util.h"
#include "botrix/item.h"
#include "botrix/weapon.h"
#include "botrix/server_plugin.h"
#include "PlatformLib/String.h"
#include "debug_assert.h"
#include "good/defines.h"
#include "good/memory.h"
#include "skill.h"
#include "types.h"
#include "weaponatts_botinterface.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"

static constexpr const char* const TEAM_NAME_UNASSIGNED = "unassigned";
static constexpr const char* const TEAM_NAME_SPECTATOR = "spectator";
static constexpr const char* const TEAM_NAME_MI6 = "mi6";
static constexpr const char* const TEAM_NAME_PHOENIX = "phoenix";

static TWeaponType BotWeaponTypeToBotrixWeaponType(WeaponAtts::BotWeaponType weaponType)
{
	switch ( weaponType )
	{
		case WeaponAtts::BotWeaponType::Melee:
		{
			return EWeaponMelee;
		}

		case WeaponAtts::BotWeaponType::GrenadeProjectile:
		{
			return EWeaponGrenade;
		}

		case WeaponAtts::BotWeaponType::RemoteDetonation:
		{
			return EWeaponRemoteDetonation;
		}

		case WeaponAtts::BotWeaponType::HitscanSingleShot:
		{
			return EWeaponPistol;
		}

		case WeaponAtts::BotWeaponType::HitscanContinuous:
		{
			return EWeaponRifle;
		}

		case WeaponAtts::BotWeaponType::Shotgun:
		{
			return EWeaponShotgun;
		}

		case WeaponAtts::BotWeaponType::RocketProjectile:
		{
			return EWeaponRocket;
		}

		default:
		{
			ASSERT(false);
			return EWeaponIdInvalid;
		}
	}
}

static int BotWeaponPreferenceToBotrixWeaponPreference(WeaponAtts::BotWeaponPreference weaponPreference)
{
	const int ret = static_cast<int>(weaponPreference) - static_cast<int>(WeaponAtts::BotWeaponPreference::Lowest);
	ASSERTSZ(ret < EBotIntelligenceTotal, "Expected preference to be within range of bot intelligence level");
	return ret;
}

static int BotWeaponTeamRestrictionToBotrixTeamIndex(WeaponAtts::BotWeaponTeamRestriction weaponTeamRestriction)
{
	int teamIndex = -1;

	switch ( weaponTeamRestriction )
	{
		case WeaponAtts::BotWeaponTeamRestriction::MI6:
		{
			teamIndex = CBotrixMod::GetTeamIndex(TEAM_NAME_MI6);
			break;
		}

		case WeaponAtts::BotWeaponTeamRestriction::Phoenix:
		{
			teamIndex = CBotrixMod::GetTeamIndex(TEAM_NAME_PHOENIX);
			break;
		}

		default:
		{
			ASSERT(false);
			teamIndex = CBotrixMod::GetTeamIndex(TEAM_NAME_UNASSIGNED);
			break;
		}
	}

	ASSERT(teamIndex >= 0);
	return teamIndex;
}

TModId CConfiguration::LoadProgrammatic()
{
	static const int respawnableFlags = CTypeToString::EntityClassFlagsFromString("respawnable");

	CBotrixServerPlugin::UpdateLogLevel();

	// Start with HL2DM defaults.
	// TODO: Pull these values out and set them in a readable way.
	CBotrixMod::LoadDefaults(EModId_HL2DM);

	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_UNASSIGNED);
	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_SPECTATOR);
	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_MI6);
	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_PHOENIX);

	CBotrixMod::iUnassignedTeam = CBotrixMod::GetTeamIndex(TEAM_NAME_UNASSIGNED);
	CBotrixMod::iSpectatorTeam = CBotrixMod::GetTeamIndex(TEAM_NAME_SPECTATOR);

	CItems::AddItemClassFor(EItemTypePlayerSpawn, CItemClass("info_player_deathmatch", 0));
	CItems::AddItemClassFor(EItemTypePlayerSpawn, CItemClass("info_player_coop", 0));

	CItems::AddItemClassFor(EItemTypeArmor, CItemClass("item_battery", respawnableFlags));
	CItems::AddItemClassFor(EItemTypeHealth, CItemClass("item_healthkit", respawnableFlags));

	CItems::AddItemClassFor(EItemTypeLadder, CItemClass("func_ladder", FEntityNone));

	RefreshWeaponConfig();

	// Very important!
	CBotrixMod::Prepare();

	// Return this for now
	return EModId_HL2DM;
}

void CConfiguration::RefreshWeaponConfig()
{
	CWeapons::Clear();
	CWeaponRegistry::StaticInstance().ForEach(&LoadWeapon);
}

void CConfiguration::LoadWeapon(const WeaponAtts::WACollection& atts)
{
	static const int respawnableFlags = CTypeToString::EntityClassFlagsFromString("respawnable");

	const WeaponAtts::WABotInterface& botIfc = atts.BotInterface;

	if ( botIfc.Preference == WeaponAtts::BotWeaponPreference::Never )
	{
		// Ignore this weapon.
		return;
	}

	good::unique_ptr<CWeapon> pWeapon(new CWeapon());

	pWeapon->iId = CWeapons::Size();
	pWeapon->pWeaponClass = CItems::GetItemClass(EItemTypeWeapon, atts.Core.Classname);

	if ( !pWeapon->pWeaponClass )
	{
		CItemClass cEntityClass;
		cEntityClass.sClassName = atts.Core.Classname;
		pWeapon->pWeaponClass = CItems::AddItemClassFor(EItemTypeWeapon, cEntityClass);
	}

	GoodAssert(pWeapon->pWeaponClass);

	// Ignore all class restrictions since we don't support them.
	pWeapon->iClass = ~0;

	pWeapon->iType = BotWeaponTypeToBotrixWeaponType(botIfc.Type);
	pWeapon->iBotPreference = BotWeaponPreferenceToBotrixWeaponPreference(botIfc.Preference);
	pWeapon->fHolsterTime = 0.0f;  // Currently unused

	int teamIndex = -1;

	if ( botIfc.TeamRestriction != WeaponAtts::BotWeaponTeamRestriction::None )
	{
		teamIndex = BotWeaponTeamRestrictionToBotrixTeamIndex(botIfc.TeamRestriction);
	}

	if ( teamIndex >= 0 )
	{
		// Make sure the unassigned (deathmatch) team can always use the weapon.
		pWeapon->iTeam = (1 << teamIndex) | (1 << CBotrixMod::iUnassignedTeam);
	}
	else
	{
		pWeapon->iTeam = ~0;
	}

	// Deal with ammo first, so that it's present for the attack modes.
	const WeaponAtts::WAAmmoDef& ammo = atts.Ammo;

	for ( size_t ammoIndex = 0; ammoIndex < SIZE_OF_ARRAY(ammo.PickupClassnames); ++ammoIndex )
	{
		const char* ammoPickupClassName = ammo.PickupClassnames[ammoIndex];

		if ( ammoPickupClassName && *ammoPickupClassName && !CItems::GetItemClass(EItemTypeAmmo, ammoPickupClassName) )
		{
			CItems::AddItemClassFor(EItemTypeAmmo, CItemClass(ammoPickupClassName, respawnableFlags));
		}
	}

	for ( size_t modeIndex = 0; modeIndex < 2; ++modeIndex )
	{
		const WeaponAtts::WABotAttackMode* attackMode =
			modeIndex == 0 ? botIfc.PrimaryAttackMode.get() : botIfc.SecondaryAttackMode.get();

		// Mode 0 must be valid.
		ASSERT(attackMode || modeIndex != 0);

		if ( !attackMode )
		{
			continue;
		}

		pWeapon->iClipSize[modeIndex] = static_cast<unsigned char>(attackMode->ClipSize);
		pWeapon->fDamage[modeIndex] = gSkillData.*(attackMode->BaseDamagePerShot);

		// Hold time: amount of time to hold the button down for.
		// Shot time: amount of time after releasing to wait before pressing again.
		const float delayPerShot = (1.0f / attackMode->AttackRate);

		if ( attackMode->AttackButtonPressTime > 0.9f * delayPerShot )
		{
			pWeapon->fHoldTime[modeIndex] = 0.9f * delayPerShot;
			pWeapon->fShotTime[modeIndex] = 0.1f * delayPerShot;
		}
		else
		{
			pWeapon->fHoldTime[modeIndex] = attackMode->AttackButtonPressTime;
			pWeapon->fShotTime[modeIndex] = delayPerShot - attackMode->AttackButtonPressTime;
		}

		// Add some extra delay so the bot doesn't refire inhumanly fast.
		pWeapon->fShotTime[modeIndex] += attackMode->MinExtraDelayBetweenShots;

		pWeapon->iReloadBy[modeIndex] = attackMode->ReloadStyle == WeaponAtts::BotWeaponReloadStyle::PerClip
			? static_cast<unsigned char>(attackMode->ClipSize)
			: 1;

		pWeapon->fReloadTime[modeIndex] = attackMode->ReloadDuration;
		pWeapon->fReloadStartTime[modeIndex] = attackMode->ReloadStartDelay;
		pWeapon->iDefaultAmmo[modeIndex] = static_cast<unsigned char>(attackMode->DefaultAmmoOnFirstPickup);
		pWeapon->iMaxAmmo[modeIndex] = static_cast<unsigned char>(attackMode->MaxAmmo);
		pWeapon->iAttackBullets[modeIndex] = static_cast<unsigned char>(attackMode->AmmoDecrementPerAttackCycle);
		pWeapon->fMinDistanceSqr[modeIndex] = attackMode->MinEffectiveRange * attackMode->MinEffectiveRange;
		pWeapon->fMaxDistanceSqr[modeIndex] = attackMode->MaxEffectiveRange * attackMode->MaxEffectiveRange;
		pWeapon->iParabolicDistance0[modeIndex] = static_cast<int>(attackMode->ParabolicDistanceAt0Degrees);
		pWeapon->iParabolicDistance45[modeIndex] = static_cast<int>(attackMode->ParabolicDistanceAt45Degrees);

		if ( pWeapon->fMaxDistanceSqr[modeIndex] <= 0.0f )
		{
			pWeapon->fMaxDistanceSqr[modeIndex] = CBotrixEngineUtil::iMaxMapSizeSqr;
		}
		else if ( pWeapon->fMaxDistanceSqr[modeIndex] <= pWeapon->fMinDistanceSqr[modeIndex] )
		{
			pWeapon->fMaxDistanceSqr[modeIndex] = pWeapon->fMinDistanceSqr[modeIndex];
		}

		// NFTODO: Support separate ammo for secondary attack mode?
		for ( size_t index = 0; index < SIZE_OF_ARRAY(ammo.PickupClassnames); ++index )
		{
			const char* pickupClassname = ammo.PickupClassnames[index];

			if ( !pickupClassname || !(*pickupClassname) )
			{
				continue;
			}

			const CItemClass* pAmmoClass = CItems::GetItemClass(EItemTypeAmmo, pickupClassname);
			ASSERT(pAmmoClass);

			pWeapon->aAmmos[modeIndex].push_back(pAmmoClass);
			pWeapon->aAmmoBullets[modeIndex].push_back(attackMode->AmmoGivenOnPickup);
		}
	}

	if ( botIfc.PrimaryAttackMode && botIfc.SecondaryAttackMode )
	{
		pWeapon->iFlags[1] |= FWeaponSameBullets;
	}

	// Sanity:
	switch ( pWeapon->iType )
	{
		case EWeaponMelee:
		case EWeaponPhysics:
		{
			pWeapon->iAttackBullets[0] = 0;
			pWeapon->iAttackBullets[1] = 0;
			break;
		}

		case EWeaponRocket:
		case EWeaponGrenade:
		case EWeaponRemoteDetonation:
		{
			pWeapon->iClipSize[0] = 1;
			break;
		}

		default:
		{
			break;
		}
	}

	CWeapons::Add(CWeaponWithAmmo(pWeapon.release()));
}
