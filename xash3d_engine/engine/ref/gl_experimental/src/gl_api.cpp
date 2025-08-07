#include "EngineInternalAPI/ref_api.h"
#include "BuildPlatform/Decorators.h"

static ref_api_t g_ImportedEngineFunctions;
static ref_globals_t* g_ImportedEngineGlobals;

static const ref_interface_t g_ExportedRendererFunctions =
{
	NULL, // R_Init,
	NULL, // R_Shutdown,
	NULL, // R_GetConfigName,
	NULL, // R_SetDisplayTransform,

	NULL, // GL_SetupAttributes,
	NULL, // GL_InitExtensions,
	NULL, // GL_ClearExtensions,

	NULL, // R_BeginFrame,
	NULL, // R_RenderScene,
	NULL, // R_EndFrame,
	NULL, // R_PushScene,
	NULL, // R_PopScene,
	NULL, // GL_BackendStartFrame,
	NULL, // GL_BackendEndFrame,

	NULL, // R_ClearScreen,
	NULL, // R_AllowFog,
	NULL, // GL_SetRenderMode,

	NULL, // R_AddEntity,
	NULL, // CL_AddCustomBeam,
	NULL, // R_ProcessEntData,

	NULL, // R_ShowTextures,

	NULL, // R_GetTextureOriginalBuffer,
	NULL, // GL_LoadTextureFromBuffer,
	NULL, // GL_ProcessTexture,
	NULL, // R_SetupSky,

	NULL, // R_Set2DMode,
	NULL, // R_DrawStretchRaw,
	NULL, // R_DrawStretchPic,
	NULL, // R_DrawTileClear,
	NULL, // CL_FillRGBA,
	NULL, // CL_FillRGBABlend,
	NULL, // R_WorldToScreen,

	NULL, // VID_ScreenShot,
	NULL, // VID_CubemapShot,

	NULL, // R_LightPoint,

	NULL, // R_DecalShoot,
	NULL, // R_DecalRemoveAll,
	NULL, // R_CreateDecalList,
	NULL, // R_ClearAllDecals,

	NULL, // R_StudioEstimateFrame,
	NULL, // R_StudioLerpMovement,
	NULL, // CL_InitStudioAPI,

	NULL, // R_InitSkyClouds,
	NULL, // GL_SubdivideSurface,
	NULL, // CL_RunLightStyles,

	NULL, // R_GetSpriteParms,
	NULL, // R_GetSpriteTexture,

	NULL, // Mod_LoadMapSprite,
	NULL, // Mod_ProcessRenderData,
	NULL, // Mod_StudioLoadTextures,

	NULL, // CL_DrawParticles,
	NULL, // CL_DrawTracers,
	NULL, // CL_DrawBeams,
	NULL, // R_BeamCull,

	NULL, // GL_RefGetParm,
	NULL, // R_GetDetailScaleForTexture,
	NULL, // R_GetExtraParmsForTexture,
	NULL, // R_GetFrameTime,

	NULL, // R_SetCurrentEntity,
	NULL, // R_SetCurrentModel,

	NULL, // GL_FindTexture,
	NULL, // GL_TextureName,
	NULL, // GL_TextureData,
	NULL, // GL_LoadTexture,
	NULL, // GL_CreateTexture,
	NULL, // GL_LoadTextureArray,
	NULL, // GL_CreateTextureArray,
	NULL, // GL_FreeTexture,

	NULL, // DrawSingleDecal,
	NULL, // R_DecalSetupVerts,
	NULL, // R_EntityRemoveDecals,

	NULL, // R_UploadStretchRaw,

	NULL, // GL_Bind,
	NULL, // GL_SelectTexture,
	NULL, // GL_LoadTexMatrixExt,
	NULL, // GL_LoadIdentityTexMatrix,
	NULL, // GL_CleanUpTextureUnits,
	NULL, // GL_TexGen,
	NULL, // GL_TextureTarget,
	NULL, // GL_SetTexCoordArrayMode,
	NULL, // GL_UpdateTexSize,
	NULL, // Reserved,
	NULL, // Reserved,

	NULL, // CL_DrawParticlesExternal,
	NULL, // R_LightVec,
	NULL, // R_StudioGetTexture,

	NULL, // R_RenderFrame,
	NULL, // Mod_SetOrthoBounds,
	NULL, // R_SpeedsMessage,
	NULL, // Mod_GetCurrentVis,
	NULL, // R_NewMap,
	NULL, // R_ClearScene,
	NULL, // R_GetProcAddress,

	NULL, // TriRenderMode,
	NULL, // TriBegin,
	NULL, // TriEnd,
	NULL, // _TriColor4f,
	NULL, // _TriColor4ub,
	NULL, // TriTexCoord2f,
	NULL, // TriVertex3fv,
	NULL, // TriVertex3f,
	NULL, // TriFog,
	NULL, // R_ScreenToWorld,
	NULL, // TriGetMatrix,
	NULL, // TriFogParams,
	NULL, // TriCullFace,

	NULL, // VGUI_DrawInit,
	NULL, // VGUI_DrawShutdown,
	NULL, // VGUI_SetupDrawingText,
	NULL, // VGUI_SetupDrawingRect,
	NULL, // VGUI_SetupDrawingImage,
	NULL, // VGUI_BindTexture,
	NULL, // VGUI_EnableTexture,
	NULL, // VGUI_CreateTexture,
	NULL, // VGUI_UploadTexture,
	NULL, // VGUI_UploadTextureBlock,
	NULL, // VGUI_DrawQuad,
	NULL, // VGUI_GetTextureSizes,
	NULL, // VGUI_GenerateTexture,
};

extern "C" int EXPORT GetRefAPI(
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
