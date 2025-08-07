#include <cstdlib>
#include "eventCommands.h"
#include "EnginePublicAPI/cvardef.h"
#include "cl_dll.h"
#include "projectInterface/IProjectInterface.h"
#include "debugging/hitscanweapondebugging.h"
#include "PlatformLib/String.h"

namespace EventCommands
{
	static bool DebugEventHitscan = false;

	static inline ILogInterface& Log()
	{
		return IProjectInterface::ProjectInterfaceImpl()->LogInterface();
	}

	static inline void PrintStatus(const char* eventType, bool state)
	{
		Log().LogF(
			ILogInterface::Level::Message,
			"%s event debugging is %s.\n",
			eventType,
			state ? "enabled" : "disabled");
	}

	static bool CheatsEnabled(bool logIfNotEnabled = false)
	{
		if ( gEngfuncs.pfnGetCvarFloat("sv_cheats") == 0.0f )
		{
			if ( logIfNotEnabled )
			{
				Log().LogF(ILogInterface::Level::Error, "sv_cheats must be enabled in order to use event debugging.\n");
			}

			return false;
		}

		return true;
	}

	static bool ParseEnabledState(const char* input)
	{
		if ( !input || !(*input) )
		{
			return false;
		}

		if ( PlatformLib_StrCaseCmp(input, "true") == 0 || PlatformLib_StrCaseCmp(input, "on") == 0 )
		{
			return true;
		}
		else if ( PlatformLib_StrCaseCmp(input, "false") == 0 || PlatformLib_StrCaseCmp(input, "off") == 0 )
		{
			return false;
		}
		else
		{
			return atoi(input) != 0;
		}
	}

	static void SetHitscanEventDebug()
	{
		if ( !CheatsEnabled(true) )
		{
			DebugEventHitscan = false;
			return;
		}

		const int argc = gEngfuncs.Cmd_Argc();

		if ( argc >= 2 )
		{
			DebugEventHitscan = ParseEnabledState(gEngfuncs.Cmd_Argv(1));
		}

		if ( !DebugEventHitscan )
		{
			HitscanWeaponDebugging::Clear();
		}

		PrintStatus("Hitscan", DebugEventHitscan);
	}

	void Initialise()
	{
		gEngfuncs.pfnAddCommand("event_hitscan_debug", &SetHitscanEventDebug);
	}

	bool HitscanEventDebuggingEnabled()
	{
		return DebugEventHitscan && CheatsEnabled();
	}
}  // namespace EventCommands
