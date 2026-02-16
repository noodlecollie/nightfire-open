#include "udll_int.h"
#include <cstring>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "rmlui/RmlUiBackend.h"
#include "UIDebug.h"

ui_enginefuncs_t gEngfuncs;
ui_extendedfuncs_t gTextfuncs;
ui_globalvars_t* gpGlobals = nullptr;
ui_gl_functions gUiGlFuncs;

static RmlUiBackend gRmlUiBackend;

static int pfnVidInit(void)
{
	if ( gRmlUiBackend.IsInitialised() )
	{
		gRmlUiBackend.ShutDown();
	}

	if ( !gRmlUiBackend.Initialise(gpGlobals->scrWidth, gpGlobals->scrHeight) )
	{
		return 0;
	}

	// REMOVE ME: Example document to display
	Rml::Context* context = gRmlUiBackend.GetRmlContext();
	Rml::ElementDocument* doc = context->LoadDocumentFromMemory(
		"<rml>\n"
		"<head>\n"
		"<title>Test Document</title>\n"
		"<style>\n"
		"body\n"
		"{\n"
		"font-family: rmlui-debugger-font;\n"
		"background-color: #FFFFFF;\n"
		"color: #000000;\n"
		"width: 100%;\n"
		"height: 100%\n"
		"}\n"
		"h1, p\n"
		"{\n"
		"display: block;"
		"}\n"
		"</style>\n"
		"</head>\n"
		"<body>\n"
		"<div style=\"border: 1px #0000FF;\">\n"
		"<h1>Testing RmlUi</h1>\n"
		"<p>This is a paragraph</p>\n"
		"<p>This is another paragraph</p>\n"
		"<p>Some more paragraphs</p>\n"
		"</div>\n"
		"</body>\n"
		"</rml>\n"
	);

	doc->Show();

	return 1;
}

static void pfnInit(void)
{
	// TODO
}

static void pfnShutdown(void)
{
	Rml::Context* context = gRmlUiBackend.GetRmlContext();
	ASSERT(context);

	context->UnloadAllDocuments();
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

static void pfnKeyEvent(int /* key */, int /* down */)
{
	// TODO
}

static void pfnMouseMove(int /* x */, int /* y */)
{
	// TODO
}

static void pfnSetActiveMenu(int /* active */)
{
	// TODO
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

static void pfnCharEvent(int /* key */)
{
	// TODO
}

static int pfnMouseInRect(void)
{
	// TODO
	return 0;
}

static int pfnIsVisible(void)
{
	// TODO
	return 0;
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
	pfnConnectionProgress_ParseServerInfo
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
