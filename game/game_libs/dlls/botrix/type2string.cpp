#include <good/log.h>
#include <good/string_buffer.h>
#include <good/string_utils.h>

#include "botrix/botrixmod.h"
#include "botrix/server_plugin.h"
#include "botrix/type2string.h"

namespace Botrix
{
	const good::string sUnknown("unknown");
	const good::string sEmpty("");
	const good::string sNone("none");

	//================================================================================================================
	const good::string&
	EnumToString(int iEnum, int iEnumCount, const good::string aStrings[], const good::string& sDefault)
	{
		GoodAssert(0 <= iEnumCount);
		return (0 <= iEnum && iEnum < iEnumCount) ? aStrings[iEnum] : sDefault;
	}

	int EnumFromString(const good::string& s, int iEnumCount, const good::string aStrings[])
	{
		GoodAssert(0 <= iEnumCount);
		for ( int i = 0; i < iEnumCount; ++i )
			if ( s == aStrings[i] )
				return i;
		return -1;
	}

	//================================================================================================================
	inline const good::string& EnumToString(int iEnum, const StringVector& aStrings, const good::string& sDefault)
	{
		return EnumToString(iEnum, aStrings.size(), aStrings.data(), sDefault);
	}

	inline int EnumFromString(const good::string& s, const StringVector& aStrings)
	{
		return EnumFromString(s, aStrings.size(), aStrings.data());
	}

	//================================================================================================================
	const good::string& FlagsToString(int iFlags, int iFlagsCount, const good::string aStrings[], bool bUseNone)
	{
		static constexpr size_t BUFFER_SIZE = 1024;
		static char s_Buffer[BUFFER_SIZE];

		GoodAssert(0 <= iFlagsCount);

		if ( iFlags == 0 )
		{
			return bUseNone ? sNone : sEmpty;
		}

		static good::string_buffer sbBuffer(s_Buffer, BUFFER_SIZE, false);
		sbBuffer.erase();

		for ( int i = 0; i < iFlagsCount; ++i )
		{
			if ( FLAG_ALL_SET_OR_0(1 << i, iFlags) )
			{
				sbBuffer << aStrings[i] << ' ';
			}
		}

		if ( sbBuffer.size() )
		{
			sbBuffer.erase(sbBuffer.size() - 1, 1);  // Erase last space.
		}

		return sbBuffer;
	}

	const good::string& FlagsToString(int iFlags, const StringVector& aStrings, bool bUseNone)
	{
		return FlagsToString(iFlags, aStrings.size(), aStrings.data(), bUseNone);
	}

	int FlagsFromString(const good::string& s, int iFlagsCount, const good::string aStrings[])
	{
		GoodAssert(0 <= iFlagsCount);

		int iResult = 0;
		StringVector aFlags;
		good::split(s, aFlags, ' ', true);

		for ( StringVector::size_type i = 0; i < aFlags.size(); ++i )
		{
			int j;
			for ( j = 0; j < iFlagsCount; ++j )
			{
				if ( aFlags[i] == aStrings[j] )
				{
					FLAG_SET(1 << j, iResult);
					break;
				}
			}
			if ( j == iFlagsCount )  // Couldn't find flag in array of strings, return an error.
				return -1;
		}

		return iResult;
	}

	//****************************************************************************************************************
	const good::string& CTypeToString::StringArrayToString(const good::string aStrings[], int iSize)
	{
		static constexpr size_t BUFFER_SIZE = 1024;
		static char s_Buffer[BUFFER_SIZE];

		GoodAssert(0 <= iSize);

		static good::string_buffer sbBuffer(s_Buffer, BUFFER_SIZE, false);
		sbBuffer.erase();

		for ( int i = 0; i < iSize; ++i )
		{
			sbBuffer << aStrings[i] << ' ';
		}

		if ( sbBuffer.size() )
		{
			sbBuffer.erase(sbBuffer.size() - 1, 1);  // Erase last space.
		}

		return sbBuffer;
	}

	//----------------------------------------------------------------------------------------------------------------
	const int iYesNoSynonims = 4;
	good::string aBools[2 * iYesNoSynonims] = {
		"false",
		"no",
		"off",
		"disable",
		"true",
		"yes",
		"on",
		"enable",
	};

	int CTypeToString::BoolFromString(const good::string& sBool)
	{
		int iResult = EnumFromString(sBool, 2 * iYesNoSynonims, aBools);
		return (iResult == -1) ? -1 : (iResult / iYesNoSynonims);
	}

	const good::string& CTypeToString::BoolToString(bool b, int which)
	{
		return aBools[b * iYesNoSynonims + which];
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TCommandAccessFlags.
	//----------------------------------------------------------------------------------------------------------------
	good::string aAccessFlags[ECommandAccessFlagTotal] = {
		"waypoint",
		"bot",
		"config",
	};

	int CTypeToString::AccessFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, ECommandAccessFlagTotal, aAccessFlags);
	}

	const good::string& CTypeToString::AccessFlagsToString(TCommandAccessFlags iFlags, bool bUseNone)
	{
		return FlagsToString(iFlags, ECommandAccessFlagTotal, aAccessFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TWaypointFlag.
	//----------------------------------------------------------------------------------------------------------------
	good::string aWaypointFlags[EWaypointFlagTotal] = {
		"stop",
		"camper",
		"sniper",
		"weapon",
		"ammo",
		"health",
		"armor",
		"health-charger",
		"armor-charger",
		"button",
		"see-button",
		"use",
		"elevator",
		"ladder",
	};

	int CTypeToString::WaypointFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, EWaypointFlagTotal, aWaypointFlags);
	}

	const good::string& CTypeToString::WaypointFlagsToString(TWaypointFlags iFlags, bool bUseNone)
	{
		return FlagsToString(iFlags, EWaypointFlagTotal, aWaypointFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TPathFlag.
	//----------------------------------------------------------------------------------------------------------------
	good::string aPathFlags[EPathFlagTotal] = {
		"crouch",
		"jump",
		"break",
		"sprint",
		"ladder",
		"stop",
		"damage",
		"flashlight",
		"door",
		"elevator",
		"totem",
		"unused",
		"unused",
		"unused",
		"untested",
		"demo",
	};

	int CTypeToString::PathFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, EPathFlagTotal, aPathFlags);
	}

	const good::string& CTypeToString::PathFlagsToString(TPathFlags iFlags, bool bUseNone)
	{
		return FlagsToString(iFlags, EPathFlagTotal, aPathFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TWaypointDrawFlags.
	//----------------------------------------------------------------------------------------------------------------
	good::string aDrawTypeFlags[EWaypointDrawFlagTotal] = {
		"beam",
		"line",
		"box",
		"text",
	};

	int CTypeToString::WaypointDrawFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, EWaypointDrawFlagTotal, aDrawTypeFlags);
	}

	const good::string& CTypeToString::WaypointDrawFlagsToString(TWaypointDrawFlags iFlags, bool bUseNone)
	{
		return FlagsToString(iFlags, EWaypointDrawFlagTotal, aDrawTypeFlags, bUseNone);
	}

	int CTypeToString::PathDrawFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, EPathDrawFlagTotal, aDrawTypeFlags);
	}

	const good::string& CTypeToString::PathDrawFlagsToString(TPathDrawFlags iFlags, bool bUseNone)
	{
		return FlagsToString(iFlags, EPathDrawFlagTotal, aDrawTypeFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TItemType.
	//----------------------------------------------------------------------------------------------------------------
	good::string aItemTypes[EItemTypeAll] = {
		"health",
		"armor",
		"weapon",
		"ammo",
		"button",
		"door",
		"ladder",
		"object",
		"spawn",
		"other",
	};

	int CTypeToString::EntityTypeFromString(const good::string& sType)
	{
		return EnumFromString(sType, EItemTypeKnownTotal + 1, aItemTypes);
	}

	const good::string& CTypeToString::EntityTypeToString(TItemType iType)
	{
		return EnumToString(iType, EItemTypeKnownTotal + 1, aItemTypes, sUnknown);
	}

	int CTypeToString::EntityTypeFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, EItemTypeKnownTotal + 1, aItemTypes);
	}

	const good::string& CTypeToString::EntityTypeFlagsToString(TItemTypeFlags iItemTypeFlags, bool bUseNone)
	{
		return FlagsToString(iItemTypeFlags, EItemTypeKnownTotal + 1, aItemTypes, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TItemFlags.
	//----------------------------------------------------------------------------------------------------------------
	good::string aEntityClassFlags[EItemFlagsTotal] = {
		"use",
		"respawnable",
		"explosive",
		"heavy",
		"box",
		"taken",
	};

	int CTypeToString::EntityClassFlagsFromString(const good::string& sFlags)
	{
		if ( sFlags == sNone )
			return 0;
		else
			return FlagsFromString(sFlags, EItemFlagsTotal, aEntityClassFlags);
	}

	const good::string& CTypeToString::EntityClassFlagsToString(TItemFlags iItemFlags, bool bUseNone)
	{
		return FlagsToString(iItemFlags, EItemFlagsTotal, aEntityClassFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TItemDrawFlags.
	//----------------------------------------------------------------------------------------------------------------
	good::string aItemDrawFlags[EItemDrawFlagTotal] = {
		"name",
		"box",
		"waypoint",
	};

	int CTypeToString::ItemDrawFlagsFromString(const good::string& sFlags)
	{
		return FlagsFromString(sFlags, EItemDrawFlagTotal, aItemDrawFlags);
	}

	const good::string& CTypeToString::ItemDrawFlagsToString(TItemDrawFlags iFlags, bool bUseNone)
	{
		return FlagsToString(iFlags, EItemDrawFlagTotal, aItemDrawFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TWeaponType.
	//----------------------------------------------------------------------------------------------------------------
	good::string aWeaponTypes[EWeaponTotal] = {
		"melee",
		"grenade",
		"physics",
		"remote",
		"pistol",
		"rifle",
		"shotgun",
		"rocket",
	};

	int CTypeToString::WeaponTypeFromString(const good::string& sType)
	{
		return EnumFromString(sType, EWeaponTotal, aWeaponTypes);
	}

	const good::string& CTypeToString::WeaponTypeToString(TWeaponType iType)
	{
		return EnumToString(iType, EWeaponTotal, aWeaponTypes, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TWeaponFlags.
	//----------------------------------------------------------------------------------------------------------------
	good::string aWeaponFlags[EWeaponFlagsTotal] = {
		"zoom",
		"trigger",
		"cure",
		"push_away",
		"deflect",
		"extinguish",
		"prepare",
		"slowing",

		"force_range",
		"force_aim",
		"same_ammo",
		"dont_add_clip",
		"default_empty",
		"bg_reload",
	};

	TWeaponFlags CTypeToString::WeaponFlagsFromString(const good::string& sType)
	{
		return FlagsFromString(sType, EWeaponFlagsTotal, aWeaponFlags);
	}

	const good::string& CTypeToString::WeaponFlagsToString(TWeaponFlags iType, bool bUseNone)
	{
		return FlagsToString(iType, EWeaponFlagsTotal, aWeaponFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TWeaponAim.
	//----------------------------------------------------------------------------------------------------------------
	good::string aWeaponAim[EWeaponAimTotal] = {"body", "head", "foot"};

	TWeaponAim CTypeToString::WeaponAimFromString(const good::string& sWeaponAim)
	{
		return EnumFromString(sWeaponAim, EWeaponAimTotal, aWeaponAim);
	}

	const good::string& CTypeToString::WeaponAimToString(TWeaponAim iWeaponAim)
	{
		return EnumToString(iWeaponAim, EWeaponAimTotal, aWeaponAim, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TPreference.
	//----------------------------------------------------------------------------------------------------------------
	good::string aPreferences[EBotIntelligenceTotal] = {
		"lowest",
		"low",
		"normal",
		"high",
		"highest",
	};

	int CTypeToString::PreferenceFromString(const good::string& sPreference)
	{
		return EnumFromString(sPreference, EBotIntelligenceTotal, aPreferences);
	}

	const good::string& CTypeToString::PreferenceToString(TBotIntelligence iPreference)
	{
		return EnumToString(iPreference, EBotIntelligenceTotal, aPreferences, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TPreference.
	//----------------------------------------------------------------------------------------------------------------
	good::string aIntelligences[EBotIntelligenceTotal] = {
		"fool",
		"stupid",
		"normal",
		"smart",
		"pro",
	};

	const good::string& CTypeToString::IntelligenceToString(TBotIntelligence iIntelligence)
	{
		return EnumToString(iIntelligence, EBotIntelligenceTotal, aIntelligences, sUnknown);
	}

	int CTypeToString::IntelligenceFromString(const good::string& sIntelligence)
	{
		return EnumFromString(sIntelligence, EBotIntelligenceTotal, aIntelligences);
	}

	//----------------------------------------------------------------------------------------------------------------
	const good::string& CTypeToString::TeamToString(int iTeam)
	{
		return EnumToString(iTeam, CBotrixMod::aTeamsNames, sUnknown);
	}

	const good::string& CTypeToString::TeamFlagsToString(int iTeams, bool bUseNone)
	{
		return FlagsToString(iTeams, CBotrixMod::aTeamsNames, bUseNone);
	}

	int CTypeToString::TeamFromString(const good::string& sTeam)
	{
		return EnumFromString(sTeam, CBotrixMod::aTeamsNames);
	}

	//----------------------------------------------------------------------------------------------------------------
	const good::string& CTypeToString::ClassToString(int iClass)
	{
		return EnumToString(iClass, CBotrixMod::aClassNames, sUnknown);
	}

	const good::string& CTypeToString::ClassFlagsToString(int iClasses, bool bUseNone)
	{
		return FlagsToString(iClasses, CBotrixMod::aClassNames, bUseNone);
	}

	int CTypeToString::ClassFromString(const good::string& sClass)
	{
		return EnumFromString(sClass, CBotrixMod::aClassNames);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TBotTasks.
	//----------------------------------------------------------------------------------------------------------------
	good::string aBotTasks[EBotTasksTotal] = {
		"find health",
		"find armor",
		"find weapon",
		"find ammo",
		"engage enemy",
		"find enemy",
	};

	const good::string& CTypeToString::BotTaskToString(TBotTask iBotTask)
	{
		return EnumToString(iBotTask, EBotTasksTotal, aBotTasks, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Ordered by TBotChat. Should be in lowercase.
	//----------------------------------------------------------------------------------------------------------------
	good::string aBotCommands[EBotChatTotal] = {
		"error",
		"greeting",
		"bye",
		"busy",
		"affirmative",
		"negative",
		"affirm",
		"negate",
		"call",
		"call response",
		"help",
		"stop",
		"come",
		"follow",
		"attack",
		"no kill",
		"sit down",
		"stand up",
		"jump",
		"leave",
		"dont hurt",

		"ok",
		"done",
		"wait",
		"no moves",
		"think",
		"explore",
		"explore new",
		"finish explore",

		"new area",
		"change area",

		"weapon found",

		"door found",
		"door change",
		"door no change",

		"button see",
		"button can push",
		"button cant push",

		"box found",
		"box lost",
		"gravity gun have",
		"gravity gun need",

		"wall found",
		"box need",

		"box try",
		"box i take",
		"box you take",
		"box i drop",
		"box you drop",

		"button weapon",
		"button no weapon",

		"door try",
		"button try",
		"button try go",
		"button door",
		"button toggles",
		"button no toggles",

		"button i push",
		"button you push",
		"button i shoot",
		"button you shoot",

		"area go",
		"area cant go",
		"door go",
		"button go",

		"cancel task",
		"better idea",

		"found plan",
	};

	int CTypeToString::BotCommandFromString(const good::string& sCommand)
	{
		return EnumFromString(sCommand, EBotChatTotal, aBotCommands);
	}

	const good::string& CTypeToString::BotCommandToString(TBotChat iCommand)
	{
		return EnumToString(iCommand, EBotChatTotal, aBotCommands, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	good::string aCmdResults[good::ELogLevelTotal] = {
		"Command not found.",
		"Command not implemented",
		"Sorry, you don't have access to this command.",
		"Command error.",
	};

	const good::string& CTypeToString::ConsoleCommandResultToString(TCommandResult iCmdResult)
	{
		return EnumToString(iCmdResult - 1, ECommandTotal - 1, aCmdResults, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	good::string aLogLevels[good::ELogLevelTotal] = {
		"trace",
		"debug",
		"info",
		"warning",
		"error",
		"none",
	};

	int CTypeToString::LogLevelFromString(const good::string& sLevel)
	{
		return EnumFromString(sLevel, good::ELogLevelTotal, aLogLevels);
	}

	const good::string& CTypeToString::LogLevelToString(int iLevel)
	{
		return EnumToString(iLevel, good::ELogLevelTotal, aLogLevels, sUnknown);
	}

	//----------------------------------------------------------------------------------------------------------------
	good::string aStrategyFlags[EFightStrategyFlagTotal] = {
		"run-away-if-near",
		"come-closer-if-far",
		//    "force-stay-far",
		//    "melee-if-close",
	};

	int CTypeToString::StrategyFlagsFromString(const good::string& sArg)
	{
		return FlagsFromString(sArg, EFightStrategyFlagTotal, aStrategyFlags);
	}

	const good::string& CTypeToString::StrategyFlagsToString(int iArg, bool bUseNone)
	{
		return FlagsToString(iArg, EFightStrategyFlagTotal, aStrategyFlags, bUseNone);
	}

	//----------------------------------------------------------------------------------------------------------------
	good::string aStrategyArgs[EFightStrategyArgTotal] = {
		"near-distance",
		"far-distance",
	};

	int CTypeToString::StrategyArgFromString(const good::string& sArg)
	{
		return EnumFromString(sArg, EFightStrategyArgTotal, aStrategyArgs);
	}

	const good::string& CTypeToString::StrategyArgToString(int iArg)
	{
		return EnumToString(iArg, EFightStrategyArgTotal, aStrategyArgs, sUnknown);
	}

	const good::string& CTypeToString::StrategyArgs()
	{
		return StringArrayToString(aStrategyArgs, EFightStrategyArgTotal);
	}
}  // namespace Botrix
