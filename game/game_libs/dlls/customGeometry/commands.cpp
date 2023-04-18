#include "customGeometry/commands.h"
#include "standard_includes.h"
#include "utils/mp_utils.h"
#include "gamerules.h"
#include "customGeometry/constructors/aabboxConstructor.h"
#include "customGeometry/messageWriter.h"

namespace CustomGeometry
{
	static bool TransmitPlayerBounds(const CBasePlayer& player)
	{
		CAABBoxConstructor constructor;
		constructor.SetBounds(player.pev->absmin, player.pev->absmax);

		GeometryItemPtr_t item = constructor.Construct();

		CMessageWriter writer(Category::DebugPlayerBounds);
		return writer.WriteClearMessage() && writer.WriteMessage(*item);
	}

	static void SnapshotPlayerBounds(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "Command requires sv_cheats to be 1.\n");
			return;
		}

		if ( !g_pGameRules || !g_pGameRules->IsMultiplayer() )
		{
			ALERT(at_console, "Command can only be executed in multiplayer.\n");
			return;
		}

		int argc = g_engfuncs.pfnCmd_Argc();

		if ( argc != 2 )
		{
			ALERT(at_console, "Usage: debug_snapshot_player_bounds <#playerId|playerName>\n");
			return;
		}

		const char* playerString = g_engfuncs.pfnCmd_Argv(1);
		CBasePlayer* player = MPUtils::CBasePlayerFromStringLookup(playerString);

		if ( !player )
		{
			ALERT(at_error, "Could not find player: '%s'\n", playerString);
			return;
		}

		if ( TransmitPlayerBounds(*player) )
		{
			ALERT(at_console, "Sent bounds information for player '%s'.\n", playerString);
		}
		else
		{
			ALERT(at_error, "Failed to send bounds information for player '%s'.\n", playerString);
		}
	}

	void InitialiseCommands()
	{
		g_engfuncs.pfnAddServerCommand("debug_snapshot_player_bounds", &SnapshotPlayerBounds);
	}
}
