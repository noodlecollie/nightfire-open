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

		if ( !item )
		{
			return false;
		}

		CMessageWriter writer(Category::DebugPlayerBounds);
		return writer.WriteClearMessage() && writer.WriteMessage(*item);
	}

	static bool TransmitTextInWorld(const Vector& pos, float lifetimeSecs, const char* text)
	{
		CGeometryItem item;

		item.SetColour(0xFFFFFFFF);
		item.SetLifetimeSecs(lifetimeSecs);
		item.SetDrawType(DrawType::Text);
		item.SetText(pos, CUtlString(text));

		CMessageWriter writer(Category::General);
		return writer.WriteClearMessage() && writer.WriteMessage(item);
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

	static void DisplayTextInWorld()
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "Command requires sv_cheats to be 1.\n");
			return;
		}

		int argc = g_engfuncs.pfnCmd_Argc();

		if ( argc != 6 )
		{
			ALERT(at_console, "Usage: debug_display_text_in_world <x> <y> <z> <lifetime> <text>\n");
			return;
		}

		float x = static_cast<float>(atof(g_engfuncs.pfnCmd_Argv(1)));
		float y = static_cast<float>(atof(g_engfuncs.pfnCmd_Argv(2)));
		float z = static_cast<float>(atof(g_engfuncs.pfnCmd_Argv(3)));
		float lifetime = static_cast<float>(atof(g_engfuncs.pfnCmd_Argv(4)));
		const char* text = g_engfuncs.pfnCmd_Argv(5);

		if ( TransmitTextInWorld(Vector(x, y, z), lifetime, text) )
		{
			ALERT(at_console, "Sent message for world text: '%s'.\n", text);
		}
		else
		{
			ALERT(at_error, "Failed to send message for world text: '%s'.\n", text);
		}
	}

	void InitialiseCommands()
	{
		g_engfuncs.pfnAddServerCommand("debug_snapshot_player_bounds", &SnapshotPlayerBounds);
		g_engfuncs.pfnAddServerCommand("debug_display_text_in_world", &DisplayTextInWorld);
	}
}  // namespace CustomGeometry
