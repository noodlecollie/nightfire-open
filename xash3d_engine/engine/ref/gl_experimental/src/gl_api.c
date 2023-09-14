#include "EngineInternalAPI/ref_api.h"
#include "PlatformDefs/decorators.h"

static ref_interface_t g_ExportedRendererFunctions;
static ref_api_t g_ImportedEngineFunctions;
static ref_globals_t* g_ImportedEngineGlobals;

int EXPORT GetRefAPI(
	int version,
	ref_interface_t* outRendererFuncs,
	const ref_api_t* inEngineFuncs,
	ref_globals_t* inEngineGlobals)
{
	if ( version != REF_API_VERSION || !outRendererFuncs || !inEngineFuncs || !inEngineGlobals )
	{
		return 0;
	}

	memcpy(outRendererFuncs, &g_ExportedRendererFunctions, sizeof(*outRendererFuncs));
	memcpy(&g_ImportedEngineFunctions, inEngineFuncs, sizeof(g_ImportedEngineFunctions));
	g_ImportedEngineGlobals = inEngineGlobals;

	return REF_API_VERSION;
}
