#include "udll_int.h"
#include <cstring>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "rmlui/RmlUiBackend.h"
#include "UIDebug.h"
#include "EnginePublicAPI/keydefs.h"

ui_enginefuncs_t gEngfuncs;
ui_extendedfuncs_t gTextfuncs;
ui_globalvars_t* gpGlobals = nullptr;
ui_gl_functions gUiGlFuncs;

static RmlUiBackend gRmlUiBackend;

static int pfnVidInit(void)
{
	return gRmlUiBackend.VidInit(gpGlobals->scrWidth, gpGlobals->scrHeight) ? 1 : 0;
}

static void pfnInit(void)
{
	gRmlUiBackend.Initialise();
}

static void pfnShutdown(void)
{
	gRmlUiBackend.ShutDown();
}

static void pfnRedraw(float /* flTime */)
{
#ifdef RENDER_DEBUG_TRIANGLE
	gRmlUiBackend.RenderDebugTriangle();
#else
	gRmlUiBackend.Update();
	gRmlUiBackend.Render();
#endif
}

static void pfnKeyEvent(int key, int down)
{
	switch ( key )
	{
		case K_MOUSE1:
		case K_MOUSE2:
		case K_MOUSE3:
		case K_MOUSE4:
		case K_MOUSE5:
		{
			gRmlUiBackend.ReceiveMouseButton(key, down != 0);
			break;
		}

		case K_MWHEELDOWN:
		case K_MWHEELUP:
		{
			// Wheel event sent as down and then up,
			// so only trigger on one of these.
			if ( down )
			{
				gRmlUiBackend.ReceiveMouseWheel(key == K_MWHEELDOWN);
			}

			break;
		}

		default:
		{
			gRmlUiBackend.ReceiveKey(key, down != 0);
			break;
		}
	}
}

static void pfnMouseMove(int x, int y)
{
	gRmlUiBackend.ReceiveMouseMove(x, y);
}

static void pfnSetActiveMenu(int active)
{
	if ( !gRmlUiBackend.IsInitialised() )
	{
		return;
	}

	gEngfuncs.pfnKeyClearStates();

	if ( active )
	{
		gEngfuncs.pfnSetKeyDest(key_menu);
		gRmlUiBackend.ReceiveShowMenu();
	}
	else
	{
		gRmlUiBackend.ReceiveHideMenu();
	}
}

static void pfnAddServerToList(struct netadr_s /* adr */, const char* /* info */)
{
	// TODO
}

static void pfnGetCursorPos(int* /* pos_x */, int* /* pos_y */)
{
	// TODO
}

static void pfnSetCursorPos(int /* pos_x */, int /* pos_y */)
{
	// TODO
}

static void pfnShowCursor(int /* show */)
{
	// TODO
}

static void pfnCharEvent(int key)
{
	gRmlUiBackend.ReceiveChar(key);
}

static int pfnMouseInRect(void)
{
	// TODO
	return 1;
}

static int pfnIsVisible(void)
{
	return gRmlUiBackend.IsVisible();
}

static int pfnCreditsActive(void)
{
	// TODO
	return 0;
}

static void pfnFinalCredits(void)
{
	// TODO
}

static void pfnAddTouchButtonToList(
	const char* /* name */,
	const char* /* texture */,
	const char* /* command */,
	unsigned char* /* color */,
	int /* flags */
)
{
	// TODO
}

static void pfnResetPing(void)
{
	// TODO
}

static void pfnShowConnectionWarning(void)
{
	// TODO
}

static void pfnShowUpdateDialog(int /* preferStore */)
{
	// TODO
}

static void pfnShowMessageBox(const char* /* text */)
{
	// TODO
}

static void pfnConnectionProgress_Disconnect(void)
{
	// TODO
}

static void pfnConnectionProgress_Download(
	const char* /* pszFileName */,
	const char* /* pszServerName */,
	int /* iCurrent */,
	int /* iTotal */,
	const char* /* comment */
)
{
	// TODO
}

static void pfnConnectionProgress_DownloadEnd(void)
{
	// TODO
}

static void pfnConnectionProgress_Precache(void)
{
	// TODO
}

static void pfnConnectionProgress_Connect(const char* /* server */)
{
	// TODO
}

static void pfnConnectionProgress_ChangeLevel(void)
{
	// TODO
}

static void pfnConnectionProgress_ParseServerInfo(const char* /* server */)
{
	// TODO
}

static void pfnStartupComplete(void)
{
	gRmlUiBackend.ReceiveStartupComplete();
}

static const UI_FUNCTIONS gFunctionTable = {
	pfnVidInit,
	pfnInit,
	pfnShutdown,
	pfnRedraw,
	pfnKeyEvent,
	pfnMouseMove,
	pfnSetActiveMenu,
	pfnAddServerToList,
	pfnGetCursorPos,
	pfnSetCursorPos,
	pfnShowCursor,
	pfnCharEvent,
	pfnMouseInRect,
	pfnIsVisible,
	pfnCreditsActive,
	pfnFinalCredits
};

extern "C" EXPORT int
GetMenuAPI(UI_FUNCTIONS* pFunctionTable, ui_enginefuncs_t* pEngfuncsFromEngine, ui_globalvars_t* pGlobals)
{
	if ( !pFunctionTable || !pEngfuncsFromEngine )
	{
		return 0;
	}

	memcpy(pFunctionTable, &gFunctionTable, sizeof(UI_FUNCTIONS));
	memcpy(&gEngfuncs, pEngfuncsFromEngine, sizeof(ui_enginefuncs_t));
	memset(&gTextfuncs, 0, sizeof(ui_extendedfuncs_t));

	gpGlobals = pGlobals;

	return 1;
}

static UI_EXTENDED_FUNCTIONS gExtendedTable = {
	pfnAddTouchButtonToList,
	pfnResetPing,
	pfnShowConnectionWarning,
	pfnShowUpdateDialog,
	pfnShowMessageBox,
	pfnConnectionProgress_Disconnect,
	pfnConnectionProgress_Download,
	pfnConnectionProgress_DownloadEnd,
	pfnConnectionProgress_Precache,
	pfnConnectionProgress_Connect,
	pfnConnectionProgress_ChangeLevel,
	pfnConnectionProgress_ParseServerInfo,
	pfnStartupComplete,
};

extern "C" EXPORT int
GetExtAPI(int version, UI_EXTENDED_FUNCTIONS* pFunctionTable, ui_extendedfuncs_t* pEngfuncsFromEngine)
{
	if ( !pFunctionTable || !pEngfuncsFromEngine )
	{
		return 0;
	}

	if ( version != MENU_EXTENDED_API_VERSION )
	{
		if ( gEngfuncs.Con_Printf )
		{
			gEngfuncs.Con_Printf(
				"Error: failed to initialize extended menu API. Expected by DLL: %d. Got from engine: %d\n",
				MENU_EXTENDED_API_VERSION,
				version
			);
		}

		return 0;
	}

	memcpy(&gTextfuncs, pEngfuncsFromEngine, sizeof(ui_extendedfuncs_t));
	memcpy(pFunctionTable, &gExtendedTable, sizeof(UI_EXTENDED_FUNCTIONS));

	return 1;
}

extern "C" EXPORT int GetUiGlAPI(int version, const ui_gl_functions* uiToEngineFuncs)
{
	if ( !uiToEngineFuncs )
	{
		return 0;
	}

	if ( version != MENU_UIGLAPI_VERSION )
	{
		if ( gEngfuncs.Con_Printf )
		{
			gEngfuncs.Con_Printf(
				"Error: failed to initialize UI GL API. Expected by DLL: %d. Got from engine: %d\n",
				MENU_UIGLAPI_VERSION,
				version
			);
		}

		return 0;
	}

	memcpy(&gUiGlFuncs, uiToEngineFuncs, sizeof(gUiGlFuncs));
	return 1;
}
