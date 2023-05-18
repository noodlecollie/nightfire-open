#include <memory>
#include "hitbox_commands.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "mp_utils.h"
#include "hitbox_debugData.h"
#include "gameplay/gameplaySystems.h"
#include "gameplay/gameplaySystemsBase.h"

namespace HitboxDebugging
{
	static CHitboxDebugData* GetData()
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "Hitbox debugging requires sv_cheats to be 1.\n");
			return nullptr;
		}

		return &(GameplaySystems::GetBase()->HitboxDebugData());
	}

	static void HitboxDebugSetMultiplayer(CHitboxDebugData& debugData)
	{
		int argc = g_engfuncs.pfnCmd_Argc();

		if ( argc != 2 && argc != 3 )
		{
			ALERT(
				at_console,
				"Multiplayer usage: hitbox_debug_set <#attackerId|attackerName> [#targetId|targetName]\n");
			ALERT(at_console, "In multiplayer, only players are supported as targets.\n");
			ALERT(at_console, "Target can be left blank to show only weapon shot traces.\n");
			return;
		}

		debugData.Clear();

		for ( int index = 1; index < argc; ++index )
		{
			const char* input = g_engfuncs.pfnCmd_Argv(index);

			if ( !input || !input[0] )
			{
				ALERT(at_error, "Invalid argument for %s\n", index == 1 ? "attacker" : "target");
				break;
			}

			CBasePlayer* player = MPUtils::CBasePlayerFromStringLookup(input);

			if ( !player )
			{
				ALERT(at_error, "Could not find %s player '%s'.\n", index == 1 ? "attacker" : "target", input);
				break;
			}

			if ( index == 1 )
			{
				debugData.SetAttackerPlayer(player);
			}
			else
			{
				debugData.SetTargetPlayer(player);
			}
		}

		if ( debugData.IsValid() )
		{
			if ( debugData.TargetPlayer() )
			{
				ALERT(
					at_console,
					"Set hitbox debugging attacker '%s' and target '%s'.\n",
					MPUtils::PlayerNetName(debugData.AttackerPlayer()),
					MPUtils::PlayerNetName(debugData.TargetPlayer()));
			}
			else
			{
				ALERT(
					at_console,
					"Set hitbox debugging attacker '%s' with no target player.\n",
					MPUtils::PlayerNetName(debugData.AttackerPlayer()));
			}
		}
		else
		{
			debugData.Clear();
			ALERT(at_console, "Hitbox debugging turned off.\n");
		}
	}

	static void HitboxDebugSetSingleplayer(CHitboxDebugData& debugData)
	{
		int argc = g_engfuncs.pfnCmd_Argc();

		if ( argc != 1 && argc != 2 )
		{
			ALERT(at_console, "Single player usage: hitbox_debug_set [#entindex|targetname]\n");
			ALERT(at_console, "In singleplayer, any entity with a model is supported as a target.\n");
			ALERT(at_console, "Target can be left blank to show only weapon shot traces.\n");
			return;
		}

		debugData.Clear();

		CBasePlayer* player = MPUtils::CBasePlayerFromIndex(1);

		if ( !player )
		{
			ALERT(at_error, "Could not get local player! Hitbox debugging turned off.\n");
			return;
		}

		CBaseAnimating* targetEntity = nullptr;
		const char* targetName = nullptr;

		if ( argc == 2 )
		{
			targetName = g_engfuncs.pfnCmd_Argv(1);
			edict_t* edict = FIND_ENTITY_BY_TARGETNAME(nullptr, targetName);

			if ( !edict )
			{
				ALERT(
					at_error,
					"Could not find entity with targetname '%s'. Hitbox debugging turned off.\n",
					targetName);
				return;
			}

			targetEntity = GetClassPtrFromEdict<CBaseAnimating>(edict);

			if ( !targetEntity )
			{
				ALERT(
					at_error,
					"Entity '%s' does not have a suported model. Hitbox debugging turned off.\n",
					targetName);
				return;
			}
		}

		debugData.SetAttackerPlayer(player);
		debugData.SetTargetEnt(targetEntity);

		ALERT(at_console, "Set hitbox debugging target entity '%s'.\n", targetName ? targetName : "worldspawn");
	}

	static void HitboxDebugClear(void)
	{
		CHitboxDebugData* debugData = GetData();

		if ( !debugData )
		{
			return;
		}

		debugData->Clear();

		ALERT(at_console, "Hitbox debugging turned off.\n");
	}

	static void HitboxDebugSet(void)
	{
		CHitboxDebugData* debugData = GetData();

		if ( !debugData )
		{
			return;
		}

		if ( GameplaySystems::IsMultiplayer() )
		{
			HitboxDebugSetMultiplayer(*debugData);
		}
		else
		{
			HitboxDebugSetSingleplayer(*debugData);
		}
	}

	void Initialise()
	{
		g_engfuncs.pfnAddServerCommand("hitbox_debug_set", &HitboxDebugSet);
		g_engfuncs.pfnAddServerCommand("hitbox_debug_clear", &HitboxDebugClear);
	}

	bool Enabled()
	{
		CHitboxDebugData* debugData = GetData();
		return debugData ? debugData->IsValid() : false;
	}
}  // namespace HitboxDebugging
