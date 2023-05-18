#include <cstdlib>
#include "DebugCommands.h"
#include "cl_dll.h"
#include "screenOverlays/screenOverlayIds.h"
#include "ScreenOverlayContainer.h"

namespace ScreenOverlays
{
#define NAME_DEBUG_SCREENOVERLAY_SET "debug_screenoverlay_set"

	static void DebugScreenOverlaySet()
	{
		if ( gEngfuncs.pfnGetCvarFloat("sv_cheats") == 0.0f )
		{
			gEngfuncs.Con_Printf("Cannot set overlays when cheats are not enabled.\n");
			return;
		}

		int argc = gEngfuncs.Cmd_Argc();

		if ( argc < 2 )
		{
			gEngfuncs.Con_Printf(
				"Usage: " NAME_DEBUG_SCREENOVERLAY_SET " <value> (Value range: %d - %d)\n",
				Overlay_None,
				Overlay__Count - 1);
			return;
		}

		OverlayId id = static_cast<OverlayId>(atoi(gEngfuncs.Cmd_Argv(1)));

		if ( id < Overlay_None || id >= Overlay__Count )
		{
			gEngfuncs
				.Con_Printf("Invalid overlay value %d - expected %d to %d.\n", id, Overlay_None, Overlay__Count - 1);
			return;
		}

		CScreenOverlayContainer::StaticInstance().SetCurrentOverlay(id);
		gEngfuncs.Con_Printf("Set screen overlay to %d.\n", id);
	}

	void InitialiseDebugCommands()
	{
		gEngfuncs.pfnAddCommand(NAME_DEBUG_SCREENOVERLAY_SET, &DebugScreenOverlaySet);
	}
}  // namespace ScreenOverlays
