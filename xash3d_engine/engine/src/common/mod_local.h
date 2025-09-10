/*
mod_local.h - model loader
Copyright (C) 2007 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "EngineInternalAPI/mod_local_defs.h"
#include "MathLib/quaternion.h"
#include "MathLib/vec3.h"
#include "cvar.h"

#ifndef REF_DLL
extern world_static_t world;
extern poolhandle_t com_studiocache;
extern model_t* loadmodel;
extern convar_t* mod_studiocache;
extern convar_t* r_wadtextures;
extern convar_t* r_showhull;

//
// model.c
//
void Mod_Init(void);
void Mod_FreeModel(model_t* mod);
void Mod_FreeAll(void);
void Mod_Shutdown(void);
void Mod_ClearUserData(void);
model_t* Mod_LoadWorld(const char* name, qboolean preload);
void* Mod_Calloc(int number, size_t size);
void* Mod_CacheCheck(struct cache_user_s* c);
void Mod_LoadCacheFile(const char* path, struct cache_user_s* cu);
void* Mod_AliasExtradata(model_t* mod);
void* Mod_StudioExtradata(model_t* mod);
model_t* Mod_FindName(const char* name, qboolean trackCRC);
model_t* Mod_LoadModel(model_t* mod, qboolean crash);
model_t* Mod_ForName(const char* name, qboolean crash, qboolean trackCRC);
qboolean Mod_ValidateCRC(const char* name, CRC32_t crc);
void Mod_NeedCRC(const char* name, qboolean needCRC);
void Mod_FreeUnused(void);

//
// mod_bmodel.c
//
void Mod_LoadBrushModel(model_t* mod, const void* buffer, size_t length, qboolean* loaded);
qboolean Mod_TestBmodelLumps(
	file_t* f,
	const char* name,
	const byte* mod_base,
	size_t length,
	qboolean silent,
	dlump_t* entities
);
qboolean Mod_HeadnodeVisible(mnode_t* node, const byte* visbits, int* lastleaf);
int Mod_FatPVS(const vec3_t org, float radius, byte* visbuffer, int visbytes, qboolean merge, qboolean fullvis);
qboolean Mod_BoxVisible(const vec3_t mins, const vec3_t maxs, const byte* visbits);
int Mod_CheckLump(const char* filename, const int lump, int* lumpsize);
int Mod_ReadLump(const char* filename, const int lump, void** lumpdata, int* lumpsize);
int Mod_SaveLump(const char* filename, const int lump, void* lumpdata, int lumpsize);
mleaf_t* Mod_PointInLeaf(const vec3_t p, mnode_t* node);
int Mod_SampleSizeForFace(msurface_t* surf);
byte* Mod_GetPVSForPoint(const vec3_t p);
void Mod_UnloadBrushModel(model_t* mod);
void Mod_PrintWorldStats_f(void);

//
// mod_dbghulls.c
//
void Mod_InitDebugHulls(void);
void Mod_CreatePolygonsForHull(int hullnum);
void Mod_ReleaseHullPolygons(void);

//
// mod_studio.c
//
void Mod_LoadStudioModel(model_t* mod, const void* buffer, qboolean* loaded);
void Mod_UnloadStudioModel(model_t* mod);
void Mod_InitStudioAPI(void);
void Mod_InitStudioHull(void);
void Mod_ResetStudioAPI(void);
const char* Mod_StudioTexName(const char* modname);
int Mod_LookUpStudioSequence(model_t* model, const char* name);
qboolean Mod_GetStudioBounds(const char* name, vec3_t mins, vec3_t maxs);
void Mod_StudioGetAttachment(const edict_t* e, int iAttachment, float* org, float* ang);
void Mod_GetBonePosition(const edict_t* e, int iBone, float* org, float* ang);
hull_t* Mod_HullForStudio(
	model_t* m,
	float frame,
	int seq,
	vec3_t ang,
	vec3_t org,
	vec3_t size,
	byte* pcnt,
	byte* pbl,
	int* hitboxes,
	edict_t* ed
);
uint32_t Mod_GetHitboxCount(const edict_t* edict);
qboolean Mod_GetTransformedHitboxPoints(const edict_t* edict, uint32_t hitboxIndex, Mod_BoxPoints* box);
int Mod_GetHitboxHitGroup(const edict_t* edict, uint32_t hitboxIndex);
void R_StudioSlerpBones(int numbones, quat_t q1[], float pos1[][3], const quat_t q2[], const float pos2[][3], float s);
void R_StudioCalcBoneQuaternion(
	int frame,
	float s,
	const mstudiobone_t* pbone,
	const mstudioanim_t* panim,
	const float* adj,
	quat_t q
);
void R_StudioCalcBonePosition(
	int frame,
	float s,
	const mstudiobone_t* pbone,
	const mstudioanim_t* panim,
	const vec3_t adj,
	vec3_t pos
);
void* R_StudioGetAnim(studiohdr_t* m_pStudioHeader, model_t* m_pSubModel, mstudioseqdesc_t* pseqdesc);
void Mod_StudioComputeBounds(void* buffer, vec3_t mins, vec3_t maxs, qboolean ignore_sequences);
int Mod_HitgroupForStudioHull(int index);
void Mod_ClearStudioCache(void);
int Mod_StudioGetSequenceCount(model_t* model);
int Mod_StudioGetSkinCount(model_t* model);
int Mod_StudioGetBodyGroupCount(model_t* model);
const char* Mod_StudioGetSequenceName(model_t* model, int anim);
float Mod_StudioGetSequenceDuration(model_t* model, int anim);
void Mod_StudioGetEyePosition(model_t* model, float* outVec3Pos);
qboolean Mod_StudioGetSequenceBounds(model_t* model, int anim, vec3_t outVecMins, vec3_t outVecMaxs);
int Mod_StudioBodyVariations(model_t* model);
void Mod_StudioPlayerBlend(mstudioseqdesc_t* pseqdesc, int* pBlend, float* pPitch);
void Mod_PrecacheEventSounds(model_t* model);
int Mod_GetNumTags(model_t* model);
void Mod_GetTag(model_t* model, int tagIndex, char* buffer, size_t length);
float Mod_GetTextureMaskThreshold(model_t* model, int textureIndex);

//
// mod_sprite.c
//
void Mod_LoadSpriteModel(model_t* mod, const void* buffer, qboolean* loaded, uint texFlags);
#endif
