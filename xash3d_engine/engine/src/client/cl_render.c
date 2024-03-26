/*
cl_render.c - RenderAPI loader & implementation
Copyright (C) 2019 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "MathLib/vec3.h"
#include "CRTLib/bitdefs.h"
#include "CRCLib/crclib.h"
#include "common/common.h"
#include "client/client.h"
#include "common/library.h"
#include "platform/platform.h"
#include "common/fscallback.h"
#include "common/engine_mempool.h"

int R_FatPVS(const vec3_t org, float radius, byte* visbuffer, qboolean merge, qboolean fullvis)
{
	return Mod_FatPVS(org, radius, visbuffer, (int)world.visbytes, merge, fullvis);
}

lightstyle_t* CL_GetLightStyle(int number)
{
	Assert(number >= 0 && number < MAX_LIGHTSTYLES);
	return &cl.lightstyles[number];
}

const ref_overview_t* GL_GetOverviewParms(void)
{
	return &clgame.overView;
}

static void* R_Mem_Alloc(size_t cb, const char* filename, const int fileline)
{
	return MemPool_Alloc(cls.mempool, cb, true, filename, fileline);
}

static void R_Mem_Free(void* mem, const char* filename, const int fileline)
{
	if ( !mem )
	{
		return;
	}

	MemPool_Free(mem, filename, fileline);
}

/*
=========
pfnGetFilesList

=========
*/
static char** pfnGetFilesList(const char* pattern, int* numFiles, int gamedironly)
{
	static search_t* t = NULL;

	if ( t )
	{
		Mem_Free(t);  // release prev search
	}

	t = FS_SearchAll(pattern, true, gamedironly);

	if ( !t )
	{
		if ( numFiles )
		{
			*numFiles = 0;
		}

		return NULL;
	}

	if ( numFiles )
	{
		*numFiles = t->numfilenames;
	}

	return t->filenames;
}

static uint pfnFileBufferCRC32(const void* buffer, const int length)
{
	uint modelCRC = 0;

	if ( !buffer || length <= 0 )
	{
		return modelCRC;
	}

	CRC32_Init(&modelCRC);
	CRC32_ProcessBuffer(&modelCRC, buffer, length);
	return CRC32_Final(modelCRC);
}

/*
=================
R_EnvShot

=================
*/
static void R_EnvShot(const float* vieworg, const char* name, qboolean skyshot, int shotsize)
{
	static vec3_t viewPoint;

	if ( !COM_CheckString(name) )
		return;

	if ( cls.scrshot_action != scrshot_inactive )
	{
		if ( cls.scrshot_action != scrshot_skyshot && cls.scrshot_action != scrshot_envshot )
			Con_Printf(S_ERROR "R_%sShot: subsystem is busy, try for next frame.\n", skyshot ? "Sky" : "Env");
		return;
	}

	cls.envshot_vieworg = NULL;  // use client view
	Q_strncpy(cls.shotname, name, sizeof(cls.shotname));

	if ( vieworg )
	{
		// make sure what viewpoint don't temporare
		VectorCopy(vieworg, viewPoint);
		cls.envshot_vieworg = viewPoint;
		cls.envshot_disable_vis = true;
	}

	// make request for envshot
	if ( skyshot )
		cls.scrshot_action = scrshot_skyshot;
	else
		cls.scrshot_action = scrshot_envshot;

	// catch negative values
	cls.envshot_viewsize = Q_max(0, shotsize);
}

/*
=============
CL_GenericHandle

=============
*/
const char* CL_GenericHandle(int fileindex)
{
	if ( fileindex < 0 || fileindex >= MAX_CUSTOM )
		return 0;
	return cl.files_precache[fileindex];
}

intptr_t CL_RenderGetParm(const int parm, const int arg, const qboolean checkRef)
{
	switch ( parm )
	{
		case PARM_BSP2_SUPPORTED:
			return 0;
		case PARM_SKY_SPHERE:
			return FBitSet(world.flags, FWORLD_SKYSPHERE) && !FBitSet(world.flags, FWORLD_CUSTOM_SKYBOX);
		case PARAM_GAMEPAUSED:
			return cl.paused;
		case PARM_CLIENT_INGAME:
			return CL_IsInGame();
		case PARM_MAX_ENTITIES:
			return clgame.maxEntities;
		case PARM_FEATURES:
			return host.features;
		case PARM_MAP_HAS_DELUXE:
			return FBitSet(world.flags, FWORLD_HAS_DELUXEMAP);
		case PARM_CLIENT_ACTIVE:
			return (cls.state == ca_active);
		case PARM_DEDICATED_SERVER:
			return (host.type == HOST_DEDICATED);
		case PARM_WATER_ALPHA:
			return FBitSet(world.flags, FWORLD_WATERALPHA);
		case PARM_DELUXEDATA:
			return (intptr_t)world.deluxedata;
		case PARM_SHADOWDATA:
			return (intptr_t)world.shadowdata;
		default:
			// indicates call from client.dll
			if ( checkRef )
			{
				return ref.dllFuncs.RefGetParm(parm, arg);
			}
			// call issued from ref_dll, check extensions here
			else
				switch ( parm )
				{
					case PARM_DEV_OVERVIEW:
						return CL_IsDevOverviewMode();
					case PARM_THIRDPERSON:
						return CL_IsThirdPerson();
					case PARM_QUAKE_COMPATIBLE:
						return Host_IsQuakeCompatible();
					case PARM_PLAYER_INDEX:
						return cl.playernum + 1;
					case PARM_VIEWENT_INDEX:
						return cl.viewentity;
					case PARM_CONNSTATE:
						return (int)cls.state;
					case PARM_PLAYING_DEMO:
						return cls.demoplayback;
					case PARM_WATER_LEVEL:
						return cl.local.waterlevel;
					case PARM_MAX_CLIENTS:
						return cl.maxclients;
					case PARM_LOCAL_HEALTH:
						return cl.local.health;
					case PARM_LOCAL_GAME:
						return Host_IsLocalGame();
					case PARM_NUMENTITIES:
						return pfnNumberOfEntities();
					case PARM_NUMMODELS:
						return cl.nummodels;
				}
	}
	return 0;
}

static intptr_t pfnRenderGetParm(int parm, int arg)
{
	return CL_RenderGetParm(parm, arg, true);
}

static int Wrapper_AVI_GetVideoInfo(void* avi, int* xres, int* yres, float* duration)
{
	return (int)AVI_GetVideoInfo((movie_state_t*)avi, xres, yres, duration);
}

static int Wrapper_AVI_IsActive(void* avi)
{
	return (int)AVI_IsActive((movie_state_t*)avi);
}

static void* Wrapper_AVI_LoadVideo(const char* filename, qboolean load_audio)
{
	return (void*)AVI_LoadVideo(filename, load_audio);
}

static int Wrapper_AVI_GetVideoFrameNumber(void* Avi, float time)
{
	return AVI_GetVideoFrameNumber((movie_state_t*)Avi, time);
}

static byte* Wrapper_AVI_GetVideoFrame(void* Avi, long frame)
{
	return AVI_GetVideoFrame((movie_state_t*)Avi, frame);
}

static void Wrapper_AVI_FreeVideo(void* Avi)
{
	AVI_FreeVideo((movie_state_t*)Avi);
}

static void* Wrapper_CL_ModelHandle(int modelindex)
{
	return (void*)CL_ModelHandle(modelindex);
}

static render_api_t gRenderAPI = {
	pfnRenderGetParm,  // GL_RenderGetParm,
	NULL,  // R_GetDetailScaleForTexture,
	NULL,  // R_GetExtraParmsForTexture,
	CL_GetLightStyle,
	CL_GetDynamicLight,
	CL_GetEntityLight,
	LightToTexGamma,
	NULL,  // R_GetFrameTime,
	NULL,  // R_SetCurrentEntity,
	NULL,  // R_SetCurrentModel,
	R_FatPVS,
	R_StoreEfrags,
	NULL,  // GL_FindTexture,
	NULL,  // GL_TextureName,
	NULL,  // GL_TextureData,
	NULL,  // GL_LoadTexture,
	NULL,  // GL_CreateTexture,
	NULL,  // GL_LoadTextureArray,
	NULL,  // GL_CreateTextureArray,
	NULL,  // GL_FreeTexture,
	NULL,  // DrawSingleDecal,
	NULL,  // R_DecalSetupVerts,
	NULL,  // R_EntityRemoveDecals,
	Wrapper_AVI_LoadVideo,
	Wrapper_AVI_GetVideoInfo,
	Wrapper_AVI_GetVideoFrameNumber,
	Wrapper_AVI_GetVideoFrame,
	NULL,  // R_UploadStretchRaw,
	Wrapper_AVI_FreeVideo,
	Wrapper_AVI_IsActive,
	S_StreamAviSamples,
	NULL,
	NULL,
	NULL,  // GL_Bind,
	NULL,  // GL_SelectTexture,
	NULL,  // GL_LoadTexMatrixExt,
	NULL,  // GL_LoadIdentityTexMatrix,
	NULL,  // GL_CleanUpTextureUnits,
	NULL,  // GL_TexGen,
	NULL,  // GL_TextureTarget,
	NULL,  // GL_SetTexCoordArrayMode,
	NULL,  // GL_GetProcAddress,
	NULL,  // GL_UpdateTexSize,
	NULL,
	NULL,
	NULL,  // CL_DrawParticlesExternal,
	R_EnvShot,
	pfnSPR_LoadExt,
	NULL,  // R_LightVec,
	NULL,  // R_StudioGetTexture,
	GL_GetOverviewParms,
	CL_GenericHandle,
	COM_SaveFile,
	NULL,
	R_Mem_Alloc,
	R_Mem_Free,
	pfnGetFilesList,
	pfnFileBufferCRC32,
	COM_CompareFileTime,
	Host_Error,
	Wrapper_CL_ModelHandle,
	pfnTime,
	Cvar_Set,
	S_FadeMusicVolume,
	COM_SetRandomSeed,
};

static void R_FillRenderAPIFromRef(render_api_t* to, const ref_interface_t* from)
{
	to->GetDetailScaleForTexture = from->GetDetailScaleForTexture;
	to->GetExtraParmsForTexture = from->GetExtraParmsForTexture;
	to->GetFrameTime = from->GetFrameTime;
	to->R_SetCurrentEntity = from->R_SetCurrentEntity;
	to->R_SetCurrentModel = from->R_SetCurrentModel;
	to->GL_FindTexture = from->GL_FindTexture;
	to->GL_TextureName = from->GL_TextureName;
	to->GL_TextureData = from->GL_TextureData;
	to->GL_LoadTexture = from->GL_LoadTexture;
	to->GL_CreateTexture = from->GL_CreateTexture;
	to->GL_LoadTextureArray = from->GL_LoadTextureArray;
	to->GL_CreateTextureArray = from->GL_CreateTextureArray;
	to->GL_FreeTexture = from->GL_FreeTexture;
	to->DrawSingleDecal = from->DrawSingleDecal;
	to->R_DecalSetupVerts = from->R_DecalSetupVerts;
	to->R_EntityRemoveDecals = from->R_EntityRemoveDecals;
	to->AVI_UploadRawFrame = from->AVI_UploadRawFrame;
	to->GL_Bind = from->GL_Bind;
	to->GL_SelectTexture = from->GL_SelectTexture;
	to->GL_LoadTextureMatrix = from->GL_LoadTextureMatrix;
	to->GL_TexMatrixIdentity = from->GL_TexMatrixIdentity;
	to->GL_CleanUpTextureUnits = from->GL_CleanUpTextureUnits;
	to->GL_TexGen = from->GL_TexGen;
	to->GL_TextureTarget = from->GL_TextureTarget;
	to->GL_TexCoordArrayMode = from->GL_TexCoordArrayMode;
	to->GL_UpdateTexSize = from->GL_UpdateTexSize;
	to->GL_DrawParticles = from->GL_DrawParticles;
	to->LightVec = from->LightVec;
	to->StudioGetTexture = from->StudioGetTexture;
	to->GL_GetProcAddress = from->R_GetProcAddress;
}

/*
===============
R_InitRenderAPI

Initialize client external rendering
===============
*/
qboolean R_InitRenderAPI(void)
{
	// make sure what render functions is cleared
	memset(&clgame.drawFuncs, 0, sizeof(clgame.drawFuncs));

	// fill missing functions from renderer
	R_FillRenderAPIFromRef(&gRenderAPI, &ref.dllFuncs);

	if ( clgame.dllFuncs.pfnGetRenderInterface )
	{
		if ( clgame.dllFuncs.pfnGetRenderInterface(CL_RENDER_INTERFACE_VERSION, &gRenderAPI, &clgame.drawFuncs) )
		{
			Con_Reportf("CL_LoadProgs: ^2initailized extended RenderAPI ^7ver. %i\n", CL_RENDER_INTERFACE_VERSION);
			return true;
		}

		// make sure what render functions is cleared
		memset(&clgame.drawFuncs, 0, sizeof(clgame.drawFuncs));

		return false;  // just tell user about problems
	}

	// render interface is missed
	return true;
}
