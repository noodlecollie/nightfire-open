/*
cl_securedstub.c - secured client dll stub
Copyright (C) 2022 FWGS

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common/common.h"
#include "client/client.h"

typedef struct cldll_func_src_s
{
	int (*pfnInitialize)(cl_enginefunc_t* pEnginefuncs, int iVersion);
	void (*pfnInit)(void);
	int (*pfnVidInit)(void);
	int (*pfnRedraw)(float flTime, int intermission);
	int (*pfnUpdateClientData)(client_data_t* cdata, float flTime);
	void (*pfnReset)(void);
	void (*pfnPlayerMove)(struct playermove_s* ppmove, int server);
	void (*pfnPlayerMoveInit)(struct playermove_s* ppmove);
	char (*pfnPlayerMoveTexture)(char* name);
	void (*IN_ActivateMouse)(void);
	void (*IN_DeactivateMouse)(void);
	void (*IN_MouseEvent)(int mstate);
	void (*IN_ClearStates)(void);
	void (*IN_Accumulate)(void);
	void (*CL_CreateMove)(float frametime, struct usercmd_s* cmd, int active);
	int (*CL_IsThirdPerson)(void);
	void (*CL_CameraOffset)(float* ofs);  // unused
	void* (*KB_Find)(const char* name);
	void (*CAM_Think)(void);  // camera stuff
	void (*pfnCalcRefdef)(ref_params_t* pparams);
	int (*pfnAddEntity)(int type, cl_entity_t* ent, const char* modelname);
	void (*pfnCreateEntities)(void);
	void (*pfnDrawNormalTriangles)(void);
	void (*pfnDrawTransparentTriangles)(void);
	void (*pfnStudioEvent)(const struct mstudioevent_s* event, const cl_entity_t* entity);
	void (*pfnPostRunCmd)(
		struct local_state_s* from,
		struct local_state_s* to,
		usercmd_t* cmd,
		int runfuncs,
		double time,
		unsigned int random_seed);
	void (*pfnShutdown)(void);
	void (*pfnTxferLocalOverrides)(entity_state_t* state, const clientdata_t* client);
	void (*pfnProcessPlayerState)(entity_state_t* dst, const entity_state_t* src);
	void (*pfnTxferPredictionData)(
		entity_state_t* ps,
		const entity_state_t* pps,
		clientdata_t* pcd,
		const clientdata_t* ppcd,
		weapon_data_t* wd,
		const weapon_data_t* pwd);
	void (*pfnDemo_ReadBuffer)(int size, byte* buffer);
	int (*pfnConnectionlessPacket)(const struct netadr_s* net_from, const char* args, char* buffer, int* size);
	int (*pfnGetHullBounds)(int hullnumber, float* mins, float* maxs);
	void (*pfnFrame)(double time);
	int (*pfnKey_Event)(int eventcode, int keynum, const char* pszCurrentBinding);
	// NOODLECOLLIE: Removed extra function pointer at the end of this that is currently unused, to eliminate warnings.
	void (*pfnTempEntUpdate)(
		double frametime,
		double client_time,
		double cl_gravity,
		struct tempent_s** ppTempEntFree,
		struct tempent_s** ppTempEntActive,
		int (*Callback_AddVisibleEntity)(
			cl_entity_t* pEntity) /*, void ( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp )*/);
	cl_entity_t* (*pfnGetUserEntity)(int index);
	void (*pfnVoiceStatus)(int entindex, qboolean bTalking);
	void (*pfnDirectorMessage)(int iSize, void* pbuf);
	int (*pfnGetStudioModelInterface)(
		int version,
		struct r_studio_interface_s** ppinterface,
		struct engine_studio_api_s* pstudio);
	void (*pfnChatInputPosition)(int* x, int* y);
	int (*pfnGetPlayerTeam)(int iPlayer);
	void* (*pfnClientFactory)(void);
} cldll_func_src_t;

typedef struct cldll_func_dst_s
{
	void (*pfnInitialize)(cl_enginefunc_t** pEnginefuncs, int* iVersion);
	void (*pfnInit)(void);
	void (*pfnVidInit)(void);
	void (*pfnRedraw)(float* flTime, int* intermission);
	void (*pfnUpdateClientData)(client_data_t** cdata, float* flTime);
	void (*pfnReset)(void);
	void (*pfnPlayerMove)(struct playermove_s** ppmove, int* server);
	void (*pfnPlayerMoveInit)(struct playermove_s** ppmove);
	void (*pfnPlayerMoveTexture)(char** name);
	void (*IN_ActivateMouse)(void);
	void (*IN_DeactivateMouse)(void);
	void (*IN_MouseEvent)(int* mstate);
	void (*IN_ClearStates)(void);
	void (*IN_Accumulate)(void);
	void (*CL_CreateMove)(float* frametime, struct usercmd_s** cmd, int* active);
	void (*CL_IsThirdPerson)(void);
	void (*CL_CameraOffset)(float** ofs);
	void (*KB_Find)(const char** name);
	void (*CAM_Think)(void);
	void (*pfnCalcRefdef)(ref_params_t** pparams);
	void (*pfnAddEntity)(int* type, cl_entity_t** ent, const char** modelname);
	void (*pfnCreateEntities)(void);
	void (*pfnDrawNormalTriangles)(void);
	void (*pfnDrawTransparentTriangles)(void);
	void (*pfnStudioEvent)(const struct mstudioevent_s** event, const cl_entity_t** entity);
	void (*pfnPostRunCmd)(
		struct local_state_s** from,
		struct local_state_s** to,
		usercmd_t** cmd,
		int* runfuncs,
		double* time,
		unsigned int* random_seed);
	void (*pfnShutdown)(void);
	void (*pfnTxferLocalOverrides)(entity_state_t** state, const clientdata_t** client);
	void (*pfnProcessPlayerState)(entity_state_t** dst, const entity_state_t** src);
	void (*pfnTxferPredictionData)(
		entity_state_t** ps,
		const entity_state_t** pps,
		clientdata_t** pcd,
		const clientdata_t** ppcd,
		weapon_data_t** wd,
		const weapon_data_t** pwd);
	void (*pfnDemo_ReadBuffer)(int* size, byte** buffer);
	void (*pfnConnectionlessPacket)(const struct netadr_s** net_from, const char** args, char** buffer, int** size);
	void (*pfnGetHullBounds)(int* hullnumber, float** mins, float** maxs);
	void (*pfnFrame)(double* time);
	void (*pfnKey_Event)(int* eventcode, int* keynum, const char** pszCurrentBinding);
	// NOODLECOLLIE: Removed extra function pointer at the end of this that is currently unused, to eliminate warnings.
	void (*pfnTempEntUpdate)(
		double* frametime,
		double* client_time,
		double* cl_gravity,
		struct tempent_s*** ppTempEntFree,
		struct tempent_s*** ppTempEntActive,
		int (**Callback_AddVisibleEntity)(
			cl_entity_t* pEntity) /*, void ( **Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp )*/);
	void (*pfnGetUserEntity)(int* index);
	void (*pfnVoiceStatus)(int* entindex, qboolean* bTalking);
	void (*pfnDirectorMessage)(int* iSize, void** pbuf);
	void (*pfnGetStudioModelInterface)(
		int* version,
		struct r_studio_interface_s*** ppinterface,
		struct engine_studio_api_s** pstudio);
	void (*pfnChatInputPosition)(int** x, int** y);
	void (*pfnGetPlayerTeam)(int* iPlayer);
} cldll_func_dst_t;

struct cl_enginefunc_dst_s;
struct modshelpers_s;
struct modchelpers_s;
struct engdata_s;

typedef struct modfuncs_s
{
	void (*m_pfnLoadMod)(char* pchModule);
	void (*m_pfnCloseMod)(void);
	int (*m_pfnNCall)(int ijump, int cnArg, ...);
	void (*m_pfnGetClDstAddrs)(cldll_func_dst_t* pcldstAddrs);
	void (*m_pfnGetEngDstAddrs)(struct cl_enginefunc_dst_s* pengdstAddrs);
	void (*m_pfnModuleLoaded)(void);
	void (*m_pfnProcessOutgoingNet)(struct netchan_s* pchan, struct sizebuf_s* psizebuf);
	qboolean (*m_pfnProcessIncomingNet)(struct netchan_s* pchan, struct sizebuf_s* psizebuf);
	void (*m_pfnTextureLoad)(char* pszName, int dxWidth, int dyHeight, char* pbData);
	void (*m_pfnModelLoad)(struct model_s* pmodel, void* pvBuf);
	void (*m_pfnFrameBegin)(void);
	void (*m_pfnFrameRender1)(void);
	void (*m_pfnFrameRender2)(void);
	void (*m_pfnSetModSHelpers)(struct modshelpers_s* pmodshelpers);
	void (*m_pfnSetModCHelpers)(struct modchelpers_s* pmodchelpers);
	void (*m_pfnSetEngData)(struct engdata_s* pengdata);
	int m_nVersion;
	void (*m_pfnConnectClient)(int iPlayer);
	void (*m_pfnRecordIP)(unsigned int pnIP);
	void (*m_pfnPlayerStatus)(unsigned char* pbData, int cbData);
	void (*m_pfnSetEngineVersion)(int nVersion);
	int m_nVoid2;
	int m_nVoid3;
	int m_nVoid4;
	int m_nVoid5;
	int m_nVoid6;
	int m_nVoid7;
	int m_nVoid8;
	int m_nVoid9;
} modfuncs_t;

static void DstInitialize(cl_enginefunc_t** pEnginefuncs, int* iVersion)
{
	(void)pEnginefuncs;
	(void)iVersion;
}

static void DstInit(void)
{
}

static void DstVidInit(void)
{
}

static void DstRedraw(float* flTime, int* intermission)
{
	(void)flTime;
	(void)intermission;
}

static void DstUpdateClientData(client_data_t** cdata, float* flTime)
{
	(void)cdata;
	(void)flTime;
}

static void DstReset(void)
{

}

static void DstPlayerMove(struct playermove_s** ppmove, int* server)
{
	(void)ppmove;
	(void)server;
}

static void DstPlayerMoveInit(struct playermove_s** ppmove)
{
	(void)ppmove;
}

static void DstPlayerMoveTexture(char** name)
{
	(void)name;
}

static void DstIN_ActivateMouse(void)
{

}

static void DstIN_DeactivateMouse(void)
{

}

static void DstIN_MouseEvent(int* mstate)
{
	(void)mstate;
}

static void DstIN_ClearStates(void)
{

}

static void DstIN_Accumulate(void)
{

}

static void DstCL_CreateMove(float* frametime, struct usercmd_s** cmd, int* active)
{
	(void)frametime;
	(void)cmd;
	(void)active;
}

static void DstCL_IsThirdPerson(void)
{

}

static void DstCL_CameraOffset(float** ofs)
{
	(void)ofs;
}

static void DstKB_Find(const char** name)
{
	(void)name;
}

static void DstCAM_Think(void)
{

}

static void DstCalcRefdef(ref_params_t** pparams)
{
	(void)pparams;
}

static void DstAddEntity(int* type, cl_entity_t** ent, const char** modelname)
{
	(void)type;
	(void)ent;
	(void)modelname;
}

static void DstCreateEntities(void)
{

}

static void DstDrawNormalTriangles(void)
{

}

static void DstDrawTransparentTriangles(void)
{

}

static void DstStudioEvent(const struct mstudioevent_s** event, const cl_entity_t** entity)
{
	(void)event;
	(void)entity;
}

static void DstPostRunCmd(
	struct local_state_s** from,
	struct local_state_s** to,
	usercmd_t** cmd,
	int* runfuncs,
	double* time,
	unsigned int* random_seed)
{
	(void)from;
	(void)to;
	(void)cmd;
	(void)runfuncs;
	(void)time;
	(void)random_seed;
}

static void DstShutdown(void)
{
}

static void DstTxferLocalOverrides(entity_state_t** state, const clientdata_t** client)
{
	(void)state;
	(void)client;
}

static void DstProcessPlayerState(entity_state_t** dst, const entity_state_t** src)
{
	(void)dst;
	(void)src;
}

static void DstTxferPredictionData(
	entity_state_t** ps,
	const entity_state_t** pps,
	clientdata_t** pcd,
	const clientdata_t** ppcd,
	weapon_data_t** wd,
	const weapon_data_t** pwd)
{
	(void)ps;
	(void)pps;
	(void)pcd;
	(void)ppcd;
	(void)wd;
	(void)pwd;
}

static void DstDemo_ReadBuffer(int* size, byte** buffer)
{
	(void)size;
	(void)buffer;
}

static void DstConnectionlessPacket(const struct netadr_s** net, const char** args, char** buffer, int** size)
{
	(void)net;
	(void)args;
	(void)buffer;
	(void)size;
}

static void DstGetHullBounds(int* hullnumber, float** mins, float** maxs)
{
	(void)hullnumber;
	(void)mins;
	(void)maxs;
}

static void DstFrame(double* time)
{
	(void)time;
}

static void DstKey_Event(int* eventcode, int* keynum, const char** pszCurrentBinding)
{
	(void)eventcode;
	(void)keynum;
	(void)pszCurrentBinding;
}

// NOODLECOLLIE: Removed extra function pointer at the end of this that is currently unused, to eliminate warnings.
static void DstTempEntUpdate(
	double* frametime,
	double* client_time,
	double* cl_gravity,
	struct tempent_s*** ppTempEntFree,
	struct tempent_s*** ppTempEntActive,
	int (**Callback_AddVisibleEntity)(
		cl_entity_t* pEntity) /*, void ( **Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp )*/)
{
	(void)frametime;
	(void)client_time;
	(void)cl_gravity;
	(void)ppTempEntFree;
	(void)ppTempEntActive;
	(void)Callback_AddVisibleEntity;
}

static void DstGetUserEntity(int* index)
{
	(void)index;
}

static void DstVoiceStatus(int* entindex, qboolean* bTalking)
{
	(void)entindex;
	(void)bTalking;
}

static void DstDirectorMessage(int* iSize, void** pbuf)
{
	(void)iSize;
	(void)pbuf;
}

static void DstGetStudioModelInterface(
	int* version,
	struct r_studio_interface_s*** ppinterface,
	struct engine_studio_api_s** pstudio)
{
	(void)version;
	(void)ppinterface;
	(void)pstudio;
}

static void DstChatInputPosition(int** x, int** y)
{
	(void)x;
	(void)y;
}

static void DstGetPlayerTeam(int* iPlayer)
{
	(void)iPlayer;
}

static cldll_func_dst_t cldllFuncDst = {
	DstInitialize,
	DstInit,
	DstVidInit,
	DstRedraw,
	DstUpdateClientData,
	DstReset,
	DstPlayerMove,
	DstPlayerMoveInit,
	DstPlayerMoveTexture,
	DstIN_ActivateMouse,
	DstIN_DeactivateMouse,
	DstIN_MouseEvent,
	DstIN_ClearStates,
	DstIN_Accumulate,
	DstCL_CreateMove,
	DstCL_IsThirdPerson,
	DstCL_CameraOffset,
	DstKB_Find,
	DstCAM_Think,
	DstCalcRefdef,
	DstAddEntity,
	DstCreateEntities,
	DstDrawNormalTriangles,
	DstDrawTransparentTriangles,
	DstStudioEvent,
	DstPostRunCmd,
	DstShutdown,
	DstTxferLocalOverrides,
	DstProcessPlayerState,
	DstTxferPredictionData,
	DstDemo_ReadBuffer,
	DstConnectionlessPacket,
	DstGetHullBounds,
	DstFrame,
	DstKey_Event,
	DstTempEntUpdate,
	DstGetUserEntity,
	DstVoiceStatus,
	DstDirectorMessage,
	DstGetStudioModelInterface,
	DstChatInputPosition,
	DstGetPlayerTeam,
};

void CL_GetSecuredClientAPI(CL_EXPORT_FUNCS F)
{
	modfuncs_t modFuncs;

	// NOODLECOLLIE: Not sure at all what's going on here. The functions passed in here
	// do not match the struct's expected functions at all. I can only assume that
	// whatever uses this struct expects certain specific functions at these locations,
	// which are then replaced with the proper functions after initialisation.
	// Original call is below for posterity.

	// secured client dlls need these
	// cldll_func_src_t cldllFuncSrc = {(void*)&modFuncs, NULL, (void*)&cldllFuncDst};

	typedef int (*InitFuncPtr)(cl_enginefunc_t*, int);
	typedef int (*VidInitFuncPtr)(void);

	cldll_func_src_t cldllFuncSrc;

	memset(&modFuncs, 0, sizeof(modFuncs));
	memset(&cldllFuncSrc, 0, sizeof(cldllFuncSrc));

	cldllFuncSrc.pfnInitialize = (InitFuncPtr)&modFuncs;
	cldllFuncSrc.pfnVidInit = (VidInitFuncPtr)&cldllFuncDst;

	// trying to fill interface now
	F(&cldllFuncSrc);

	// map exports to xash's cldll_func_t
	clgame.dllFuncs.pfnInitialize = cldllFuncSrc.pfnInitialize;
	clgame.dllFuncs.pfnInit = cldllFuncSrc.pfnInit;
	clgame.dllFuncs.pfnVidInit = cldllFuncSrc.pfnVidInit;
	clgame.dllFuncs.pfnRedraw = cldllFuncSrc.pfnRedraw;
	clgame.dllFuncs.pfnUpdateClientData = cldllFuncSrc.pfnUpdateClientData;
	clgame.dllFuncs.pfnReset = cldllFuncSrc.pfnReset;
	clgame.dllFuncs.pfnPlayerMove = cldllFuncSrc.pfnPlayerMove;
	clgame.dllFuncs.pfnPlayerMoveInit = cldllFuncSrc.pfnPlayerMoveInit;
	clgame.dllFuncs.pfnPlayerMoveTexture = cldllFuncSrc.pfnPlayerMoveTexture;
	clgame.dllFuncs.IN_ActivateMouse = cldllFuncSrc.IN_ActivateMouse;
	clgame.dllFuncs.IN_DeactivateMouse = cldllFuncSrc.IN_DeactivateMouse;
	clgame.dllFuncs.IN_MouseEvent = cldllFuncSrc.IN_MouseEvent;
	clgame.dllFuncs.IN_ClearStates = cldllFuncSrc.IN_ClearStates;
	clgame.dllFuncs.IN_Accumulate = cldllFuncSrc.IN_Accumulate;
	clgame.dllFuncs.CL_CreateMove = cldllFuncSrc.CL_CreateMove;
	clgame.dllFuncs.CL_IsThirdPerson = cldllFuncSrc.CL_IsThirdPerson;
	clgame.dllFuncs.CL_CameraOffset = cldllFuncSrc.CL_CameraOffset;
	clgame.dllFuncs.KB_Find = cldllFuncSrc.KB_Find;
	clgame.dllFuncs.CAM_Think = cldllFuncSrc.CAM_Think;
	clgame.dllFuncs.pfnCalcRefdef = cldllFuncSrc.pfnCalcRefdef;
	clgame.dllFuncs.pfnAddEntity = cldllFuncSrc.pfnAddEntity;
	clgame.dllFuncs.pfnCreateEntities = cldllFuncSrc.pfnCreateEntities;
	clgame.dllFuncs.pfnDrawNormalTriangles = cldllFuncSrc.pfnDrawNormalTriangles;
	clgame.dllFuncs.pfnDrawTransparentTriangles = cldllFuncSrc.pfnDrawTransparentTriangles;
	clgame.dllFuncs.pfnStudioEvent = cldllFuncSrc.pfnStudioEvent;
	clgame.dllFuncs.pfnPostRunCmd = cldllFuncSrc.pfnPostRunCmd;
	clgame.dllFuncs.pfnShutdown = cldllFuncSrc.pfnShutdown;
	clgame.dllFuncs.pfnTxferLocalOverrides = cldllFuncSrc.pfnTxferLocalOverrides;
	clgame.dllFuncs.pfnProcessPlayerState = cldllFuncSrc.pfnProcessPlayerState;
	clgame.dllFuncs.pfnTxferPredictionData = cldllFuncSrc.pfnTxferPredictionData;
	clgame.dllFuncs.pfnDemo_ReadBuffer = cldllFuncSrc.pfnDemo_ReadBuffer;
	clgame.dllFuncs.pfnConnectionlessPacket = cldllFuncSrc.pfnConnectionlessPacket;
	clgame.dllFuncs.pfnGetHullBounds = cldllFuncSrc.pfnGetHullBounds;
	clgame.dllFuncs.pfnFrame = cldllFuncSrc.pfnFrame;
	clgame.dllFuncs.pfnKey_Event = cldllFuncSrc.pfnKey_Event;
	clgame.dllFuncs.pfnTempEntUpdate = cldllFuncSrc.pfnTempEntUpdate;
	clgame.dllFuncs.pfnGetUserEntity = cldllFuncSrc.pfnGetUserEntity;
	clgame.dllFuncs.pfnVoiceStatus = cldllFuncSrc.pfnVoiceStatus;
	clgame.dllFuncs.pfnDirectorMessage = cldllFuncSrc.pfnDirectorMessage;
	clgame.dllFuncs.pfnGetStudioModelInterface = cldllFuncSrc.pfnGetStudioModelInterface;
	clgame.dllFuncs.pfnChatInputPosition = cldllFuncSrc.pfnChatInputPosition;
}
