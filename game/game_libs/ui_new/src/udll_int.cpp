#include <cstring>
#include "EngineInternalAPI/menu_int.h"

ui_enginefuncs_t gEngfuncs;
ui_extendedfuncs_t gTextfuncs;
ui_globalvars_t* gpGlobals = nullptr;

static const UI_FUNCTIONS gFunctionTable = {
	nullptr,  // pfnVidInit
	nullptr,  // pfnInit
	nullptr,  // pfnShutdown
	nullptr,  // pfnRedraw
	nullptr,  // pfnKeyEvent
	nullptr,  // pfnMouseMove
	nullptr,  // pfnSetActiveMenu
	nullptr,  // pfnAddServerToList
	nullptr,  // pfnGetCursorPos
	nullptr,  // pfnSetCursorPos
	nullptr,  // pfnShowCursor
	nullptr,  // pfnCharEvent
	nullptr,  // pfnMouseInRect
	nullptr,  // pfnIsVisible
	nullptr,  // pfnCreditsActive
	nullptr,  // pfnFinalCredits
};

static UI_EXTENDED_FUNCTIONS gExtendedTable = {
	nullptr,  // pfnAddTouchButtonToList
	nullptr,  // pfnResetPing
	nullptr,  // pfnShowConnectionWarning
	nullptr,  // pfnShowUpdateDialog
	nullptr,  // pfnShowMessageBox
	nullptr,  // pfnConnectionProgress_Disconnect
	nullptr,  // pfnConnectionProgress_Download
	nullptr,  // pfnConnectionProgress_DownloadEnd
	nullptr,  // pfnConnectionProgress_Precache
	nullptr,  // pfnConnectionProgress_Connect
	nullptr,  // pfnConnectionProgress_ChangeLevel
	nullptr,  // pfnConnectionProgress_ParseServerInfo
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
