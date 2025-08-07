/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#include "hud.h"
#include "cl_util.h"
#include "hulldefs.h"
#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/entity_state.h"
#include "EnginePublicAPI/cl_entity.h"
#include "EnginePublicAPI/entity_types.h"
#include "EnginePublicAPI/usercmd.h"
#include "EnginePublicAPI/pm_defs.h"
#include "pm_materials.h"

#include "eventscripts.h"
#include "ev_hldm.h"

#include "EnginePublicAPI/r_efx.h"
#include "EnginePublicAPI/event_api.h"
#include "EnginePublicAPI/event_args.h"
#include "in_defs.h"

#include <string.h>
#include <stdio.h>

#include "EnginePublicAPI/r_studioint.h"
#include "EnginePublicAPI/com_model.h"

#include "weaponregistry.h"
#include "genericweapon.h"

#include <memory>
#include "baseweaponeventplayer.h"
#include "hitscanweaponeventplayer.h"
#include "projectileweaponeventplayer.h"
#include "meleeweaponeventplayer.h"
#include "animationweaponeventplayer.h"
#include "resources/SoundResources.h"
#include "resources/SurfaceAttributes.h"
#include "sound/ClientSoundInstance.h"
#include "resources/TextureResources.h"
#include "PlatformLib/String.h"
#include "MathLib/angles.h"
#include "MathLib/utils.h"

// TODO: Make into a convar?
static constexpr float BULLET_RICOCHET_NOISE_CHANCE = 0.5f;

static std::unique_ptr<BaseWeaponEventPlayer> EventPlayers[MAX_WEAPONS][WeaponAtts::WACollection::MAX_ATTACK_MODES];

extern engine_studio_api_t IEngineStudio;

void V_PunchAxis(int axis, float punch);
void VectorAngles(const float* forward, float* angles);

extern cvar_t* cl_weapon_prediction;

// These are used elsewhere, apparently.
BEAM* pBeam;
BEAM* pBeam2;
TEMPENTITY* pFlare;  // Vit_amiN: egon's beam flare

void EV_HLDM_Init()
{
	CWeaponRegistry::StaticInstance().ForEach(
		[](const WeaponAtts::WACollection& atts)
		{
			const uint32_t weaponIndex = static_cast<uint32_t>(atts.Core.Id);
			ASSERT(weaponIndex < MAX_WEAPONS);

			FOR_EACH_VEC(atts.AttackModes, attackModeIndex)
			{
				if ( static_cast<size_t>(attackModeIndex) >= WeaponAtts::WACollection::MAX_ATTACK_MODES )
				{
					ASSERTSZ(false, "Attack mode out of range");
					break;
				}

				const WeaponAtts::WABaseAttack* baseAttack = atts.AttackModes[attackModeIndex].get();

				if ( !baseAttack->EventScript )
				{
					ASSERTSZ(false, "No script specified for attack mode");
					ALERT(
						at_error,
						"EV_HLDM_Init: Weapon '%s' does not specify an event script for attack mode %u!\n",
						atts.Core.Classname,
						attackModeIndex);

					continue;
				}

				BaseWeaponEventPlayer* eventPlayer = nullptr;

				switch ( baseAttack->Classify() )
				{
					case WeaponAtts::WABaseAttack::Classification::Hitscan:
					{
						eventPlayer = new HitscanWeaponEventPlayer();
						break;
					}

					case WeaponAtts::WABaseAttack::Classification::Projectile:
					{
						eventPlayer = new ProjectileWeaponEventPlayer();
						break;
					}

					case WeaponAtts::WABaseAttack::Classification::Melee:
					{
						eventPlayer = new MeleeWeaponEventPlayer();
						break;
					}

					case WeaponAtts::WABaseAttack::Classification::Event:
					{
						eventPlayer = new AnimationWeaponEventPlayer();
						break;
					}

					default:
					{
						ASSERTSZ(false, "Unknown attack mode classification");
						gEngfuncs.Con_Printf(
							"EV_HLDM_Init: No event handler for fire mode mechanic ID %u!\n",
							atts.Core.Classname,
							static_cast<uint32_t>(baseAttack->Classify()));

						break;
					}
				}

				if ( !eventPlayer )
				{
					continue;
				}

				EventPlayers[weaponIndex][attackModeIndex].reset(eventPlayer);
				eventPlayer->LoadEventScript(baseAttack->EventScript);
			}
		});
}

void EV_HandleGenericWeaponFire(event_args_t* args)
{
	const WeaponAtts::WABaseAttack::AttackModeSignature* signature =
		static_cast<const WeaponAtts::WABaseAttack::AttackModeSignature*>(args->localUserData);

	const uint32_t weaponId = static_cast<uint32_t>(signature->WeaponId);
	const uint32_t attackModeIndex = signature->Index;

	if ( weaponId >= MAX_WEAPONS || attackModeIndex >= WeaponAtts::WACollection::MAX_ATTACK_MODES )
	{
		return;
	}

	std::unique_ptr<BaseWeaponEventPlayer>& eventPlayer = EventPlayers[weaponId][attackModeIndex];

	if ( !eventPlayer )
	{
		return;
	}

	eventPlayer->PlayEvent(args, signature);
}

SurfaceProp EV_HLDM_GetSurfacePropForTexture(int, pmtrace_t* ptr, float* vecSrc, float* vecEnd)
{
	int entity = gEngfuncs.pEventAPI->EV_IndexFromTrace(ptr);

	if ( entity >= 1 && entity <= gEngfuncs.GetMaxClients() )
	{
		return SurfaceProp_Flesh;
	}

	if ( entity == 0 )
	{
		texture_t* texture = gEngfuncs.pEventAPI->EV_TraceTexture(ptr->ent, vecSrc, vecEnd);

		if ( texture )
		{
			return static_cast<SurfaceProp>(texture->surfaceType);
		}
	}

	return SurfaceProp_None;
}

// play a strike sound based on the texture that was hit by the attack traceline.  VecSrc/VecEnd are the
// original traceline endpoints used by the attacker, iBulletType is the type of bullet that hit the texture.
void EV_HLDM_PlayTextureSound(int idx, pmtrace_t* ptr, float* vecSrc, float* vecEnd, int iBulletType)
{
	SurfaceProp texSurfaceProp = EV_HLDM_GetSurfacePropForTexture(idx, ptr, vecSrc, vecEnd);

	if ( texSurfaceProp == SurfaceProp_None ||
		 (texSurfaceProp == SurfaceProp_Flesh && iBulletType == BULLET_PLAYER_CROWBAR) )
	{
		// Don't play any sound.
		return;
	}

	CSurfaceAttributes& surfAttsMaster = CSurfaceAttributes::StaticInstance();
	const CSurfaceAttributes::Attributes& surfaceAtts =
		surfAttsMaster.GetAttributes(static_cast<SurfaceProp>(texSurfaceProp));

	if ( surfaceAtts.ricochetChance > 0.0f && surfaceAtts.ricochetSoundVol > 0.0f &&
		 gEngfuncs.pfnRandomFloat(0.0f, 1.0f) <= surfaceAtts.ricochetChance )
	{
		const char* soundPath = SoundResources::WeaponSounds.RandomResourcePath(WeaponSoundId::BulletRicochet);
		gEngfuncs.pEventAPI->EV_PlaySound(
			-1,
			ptr->endpos,
			0,
			soundPath,
			surfaceAtts.ricochetSoundVol,
			ATTN_NORM,
			0,
			gEngfuncs.pfnRandomLong(97, 103));
	}
}

char* EV_HLDM_DamageDecal(physent_t* pe)
{
	static char decalname[32];
	int idx;

	if ( pe->classnumber == 1 )
	{
		idx = gEngfuncs.pfnRandomLong(0, 2);
		PlatformLib_SNPrintF(decalname, sizeof(decalname), "{break%i", idx + 1);
	}
	else if ( pe->rendermode != kRenderNormal )
	{
		PlatformLib_SNPrintF(decalname, sizeof(decalname), "{bproof1");
	}
	else
	{
		idx = gEngfuncs.pfnRandomLong(0, 4);
		PlatformLib_SNPrintF(decalname, sizeof(decalname), "{shot%i", idx + 1);
	}

	return decalname;
}

const CSurfaceAttributes::Attributes&
GetSurfaceAttributes(int idx, pmtrace_t* ptr, float* vecSrc, float* vecEnd, SurfaceProp* outSurface = nullptr)
{
	SurfaceProp surfaceProp = EV_HLDM_GetSurfacePropForTexture(idx, ptr, vecSrc, vecEnd);

	if ( outSurface )
	{
		*outSurface = surfaceProp;
	}

	CSurfaceAttributes& surfAttsMaster = CSurfaceAttributes::StaticInstance();
	return surfAttsMaster.GetAttributes(surfaceProp);
}

void EV_HLDM_GunshotDecalTrace(pmtrace_t* pTrace, const char* decalName, float scale)
{
	physent_t* pe = gEngfuncs.pEventAPI->EV_GetPhysent(pTrace->ent);

	// Only decal brush models such as the world etc.
	if ( decalName && decalName[0] && pe && (pe->solid == SOLID_BSP || pe->movetype == MOVETYPE_PUSHSTEP) )
	{
		if ( gEngfuncs.pfnGetCvarFloat("r_decals") )
		{
			gEngfuncs.pEfxAPI->R_FireCustomDecal(
				gEngfuncs.pEfxAPI->Draw_DecalIndex(gEngfuncs.pEfxAPI->Draw_DecalIndexFromName(decalName)),
				gEngfuncs.pEventAPI->EV_IndexFromTrace(pTrace),
				0,
				pTrace->endpos,
				0,
				scale);
		}
	}
}

void EV_HLDM_DecalGunshot(pmtrace_t* pTrace, int iBulletType)
{
	physent_t* pe;

	pe = gEngfuncs.pEventAPI->EV_GetPhysent(pTrace->ent);

	if ( pe && (pe->solid == SOLID_BSP || pe->movetype == MOVETYPE_PUSHSTEP) )
	{
		switch ( iBulletType )
		{
			case BULLET_PLAYER_9MM:
			case BULLET_MONSTER_9MM:
			case BULLET_PLAYER_MP5:
			case BULLET_MONSTER_MP5:
			case BULLET_PLAYER_BUCKSHOT:
			case BULLET_PLAYER_357:
			case BULLET_GENERIC:
			default:
				// smoke and decal
				gEngfuncs.pEfxAPI->R_BulletImpactParticles(pTrace->endpos);
				EV_HLDM_GunshotDecalTrace(pTrace, EV_HLDM_DamageDecal(pe), 1.0f);
				break;
		}
	}
}

void EV_HLDM_DecalGunshotNew(int idx, pmtrace_t* ptr, float* vecSrc, float* vecEnd)
{
	physent_t* pe = gEngfuncs.pEventAPI->EV_GetPhysent(ptr->ent);

	if ( !pe || (pe->solid != SOLID_BSP && pe->movetype != MOVETYPE_PUSHSTEP) )
	{
		return;
	}

	SurfaceProp surfaceProp = SurfaceProp_None;
	const CSurfaceAttributes::Attributes& atts = GetSurfaceAttributes(idx, ptr, vecSrc, vecEnd, &surfaceProp);

	if ( atts.decal == SurfaceDecalId::None )
	{
		return;
	}

	const char* decalPath = TextureResources::Decals.RandomResourcePath(atts.decal);
	const float scale = TextureResources::Decals.GetAttributes(atts.decal).scale;

	if ( scale <= 0.0f )
	{
		return;
	}

	gEngfuncs.pEfxAPI->R_BulletImpactParticlesForSurface(ptr->endpos, surfaceProp);
	EV_HLDM_GunshotDecalTrace(ptr, decalPath, scale);
}

void EV_HLDM_CheckTracer(int idx, float* vecSrc, float* end, float* forward, float* right, int iBulletType)
{
	vec3_t vecTracerSrc;
	qboolean player = idx >= 1 && idx <= gEngfuncs.GetMaxClients() ? true : false;

	if ( player )
	{
		int i;
		Vector offset(0, 0, -4);

		// adjust tracer position for player
		for ( i = 0; i < 3; i++ )
		{
			vecTracerSrc[i] = vecSrc[i] + offset[i] + right[i] * 2 + forward[i] * 16;
		}
	}
	else
	{
		VectorCopy(vecSrc, vecTracerSrc);
	}

	switch ( iBulletType )
	{
		case BULLET_PLAYER_MP5:
		case BULLET_MONSTER_MP5:
		case BULLET_MONSTER_9MM:
		case BULLET_MONSTER_12MM:
		default:
			EV_CreateBulletTracer(vecTracerSrc, end);
			break;
	}
}

/*
================
FireBullets

Go to the trouble of combining multiple pellets into a single damage call.
================
*/
void EV_HLDM_FireBullets(
	int idx,
	float* forward,
	float* right,
	float* up,
	int cShots,
	float* vecSrc,
	float* vecDirShooting,
	float flDistance,
	int iBulletType,
	int iTracerFreq,
	float flSpreadX,
	float flSpreadY)
{
	int i;
	pmtrace_t tr;
	int iShot;

	for ( iShot = 1; iShot <= cShots; iShot++ )
	{
		vec3_t vecDir, vecEnd;
		float x, y, z;
		const qboolean tracer = (iShot % iTracerFreq) == 0;

		// We randomize for the Shotgun.
		if ( iBulletType == BULLET_PLAYER_BUCKSHOT )
		{
			do
			{
				x = gEngfuncs.pfnRandomFloat(-0.5, 0.5) + gEngfuncs.pfnRandomFloat(-0.5, 0.5);
				y = gEngfuncs.pfnRandomFloat(-0.5, 0.5) + gEngfuncs.pfnRandomFloat(-0.5, 0.5);
				z = x * x + y * y;
			}
			while ( z > 1 );

			for ( i = 0; i < 3; i++ )
			{
				vecDir[i] = vecDirShooting[i] + x * flSpreadX * right[i] + y * flSpreadY * up[i];
				vecEnd[i] = vecSrc[i] + flDistance * vecDir[i];
			}
		}  // But other guns already have their spread randomized in the synched spread.
		else
		{
			for ( i = 0; i < 3; i++ )
			{
				vecDir[i] = vecDirShooting[i] + flSpreadX * right[i] + flSpreadY * up[i];
				vecEnd[i] = vecSrc[i] + flDistance * vecDir[i];
			}
		}

		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

		// Store off the old count
		gEngfuncs.pEventAPI->EV_PushPMStates();

		// Now add in all of the players.
		gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);

		gEngfuncs.pEventAPI->EV_SetTraceHull(2);
		gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr);

		if ( tracer )
		{
			EV_HLDM_CheckTracer(idx, vecSrc, tr.endpos, forward, right, iBulletType);
		}

		// do damage, paint decals
		if ( tr.fraction != 1.0 )
		{
			EV_HLDM_PlayTextureSound(idx, &tr, vecSrc, vecEnd, iBulletType);
			EV_HLDM_DecalGunshot(&tr, iBulletType);
		}

		gEngfuncs.pEventAPI->EV_PopPMStates();
	}
}

#ifdef HL_CONTENT

//======================
//	    GLOCK START
//======================
void EV_FireGlock1(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	int empty;

	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	empty = args->bparam1;
	AngleVectors(angles, forward, right, up);

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell.mdl");  // brass shell

	if ( EV_IsLocal(idx) )
	{
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(empty ? GLOCK_SHOOT_EMPTY : GLOCK_SHOOT, 2);

		V_PunchAxis(0, -2.0);
	}

	EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4);

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHELL);

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/pl_gun3.wav",
		gEngfuncs.pfnRandomFloat(0.92f, 1.0f),
		ATTN_NORM,
		0,
		98 + gEngfuncs.pfnRandomLong(0, 3));

	EV_GetGunPosition(args, vecSrc, origin);

	VectorCopy(forward, vecAiming);

	EV_HLDM_FireBullets(
		idx,
		forward,
		right,
		up,
		1,
		vecSrc,
		vecAiming,
		8192,
		BULLET_PLAYER_9MM,
		1,
		args->fparam1,
		args->fparam2);
}

void EV_FireGlock2(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);
	int empty = args->bparam1;

	AngleVectors(angles, forward, right, up);

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell.mdl");  // brass shell

	if ( EV_IsLocal(idx) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(empty ? GLOCK_SHOOT_EMPTY : GLOCK_SHOOT, 2);

		V_PunchAxis(0, -2.0);
	}

	EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4);

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHELL);

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/pl_gun3.wav",
		gEngfuncs.pfnRandomFloat(0.92f, 1.0f),
		ATTN_NORM,
		0,
		98 + gEngfuncs.pfnRandomLong(0, 3));

	EV_GetGunPosition(args, vecSrc, origin);

	VectorCopy(forward, vecAiming);

	EV_HLDM_FireBullets(
		idx,
		forward,
		right,
		up,
		1,
		vecSrc,
		vecAiming,
		8192,
		BULLET_PLAYER_9MM,
		1,
		args->fparam1,
		args->fparam2);
}
//======================
//	   GLOCK END
//======================

//======================
//	  SHOTGUN START
//======================
void EV_FireShotGunDouble(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	int j;
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc;
	vec3_t vecAiming;
	vec3_t up;
	vec3_t right;
	vec3_t forward;
	// float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	AngleVectors(angles, forward, right, up);

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shotgunshell.mdl");  // brass shell

	if ( EV_IsLocal(idx) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(SHOTGUN_FIRE2, 2);
		V_PunchAxis(0, -10.0);
	}

	for ( j = 0; j < 2; j++ )
	{
		EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 32, -12, 6);

		EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHOTSHELL);
	}

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/dbarrel1.wav",
		gEngfuncs.pfnRandomFloat(0.98f, 1.0f),
		ATTN_NORM,
		0,
		85 + gEngfuncs.pfnRandomLong(0, 0x1f));

	EV_GetGunPosition(args, vecSrc, origin);
	VectorCopy(forward, vecAiming);

	if ( gEngfuncs.GetMaxClients() > 1 )
	{
		EV_HLDM_FireBullets(
			idx,
			forward,
			right,
			up,
			8,
			vecSrc,
			vecAiming,
			2048,
			BULLET_PLAYER_BUCKSHOT,
			1,
			0.17365f,
			0.04362f);
	}
	else
	{
		EV_HLDM_FireBullets(
			idx,
			forward,
			right,
			up,
			12,
			vecSrc,
			vecAiming,
			2048,
			BULLET_PLAYER_BUCKSHOT,
			1,
			0.08716f,
			0.08716f);
	}
}

void EV_FireShotGunSingle(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t up;
	vec3_t right;
	vec3_t forward;
	// float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	AngleVectors(angles, forward, right, up);

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shotgunshell.mdl");  // brass shell

	if ( EV_IsLocal(idx) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(SHOTGUN_FIRE, 2);

		V_PunchAxis(0, -5.0);
	}

	EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 32, -12, 6);

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHOTSHELL);

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/sbarrel1.wav",
		gEngfuncs.pfnRandomFloat(0.95f, 1.0f),
		ATTN_NORM,
		0,
		93 + gEngfuncs.pfnRandomLong(0, 0x1f));

	EV_GetGunPosition(args, vecSrc, origin);
	VectorCopy(forward, vecAiming);

	if ( gEngfuncs.GetMaxClients() > 1 )
	{
		EV_HLDM_FireBullets(
			idx,
			forward,
			right,
			up,
			4,
			vecSrc,
			vecAiming,
			2048,
			BULLET_PLAYER_BUCKSHOT,
			1,
			0.08716f,
			0.04362f);
	}
	else
	{
		EV_HLDM_FireBullets(
			idx,
			forward,
			right,
			up,
			6,
			vecSrc,
			vecAiming,
			2048,
			BULLET_PLAYER_BUCKSHOT,
			1,
			0.08716f,
			0.08716f);
	}
}
//======================
//	   SHOTGUN END
//======================

//======================
//	    MP5 START
//======================
void EV_FireMP5(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;
	// float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	AngleVectors(angles, forward, right, up);

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell.mdl");  // brass shell

	if ( EV_IsLocal(idx) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(MP5_FIRE1 + gEngfuncs.pfnRandomLong(0, 2), 2);

		V_PunchAxis(0, gEngfuncs.pfnRandomFloat(-2, 2));
	}

	EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4);

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHELL);

	switch ( gEngfuncs.pfnRandomLong(0, 1) )
	{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound(
				idx,
				origin,
				CHAN_WEAPON,
				"weapons/hks1.wav",
				1,
				ATTN_NORM,
				0,
				94 + gEngfuncs.pfnRandomLong(0, 0xf));
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound(
				idx,
				origin,
				CHAN_WEAPON,
				"weapons/hks2.wav",
				1,
				ATTN_NORM,
				0,
				94 + gEngfuncs.pfnRandomLong(0, 0xf));
			break;
	}

	EV_GetGunPosition(args, vecSrc, origin);
	VectorCopy(forward, vecAiming);

	if ( gEngfuncs.GetMaxClients() > 1 )
	{
		EV_HLDM_FireBullets(
			idx,
			forward,
			right,
			up,
			1,
			vecSrc,
			vecAiming,
			8192,
			BULLET_PLAYER_MP5,
			1,
			args->fparam1,
			args->fparam2);
	}
	else
	{
		EV_HLDM_FireBullets(
			idx,
			forward,
			right,
			up,
			1,
			vecSrc,
			vecAiming,
			8192,
			BULLET_PLAYER_MP5,
			1,
			args->fparam1,
			args->fparam2);
	}
}

// We only predict the animation and sound
// The grenade is still launched from the server.
void EV_FireMP52(event_args_t* args)
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy(args->origin, origin);

	if ( EV_IsLocal(idx) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation(MP5_LAUNCH, 2);
		V_PunchAxis(0, -10);
	}

	switch ( gEngfuncs.pfnRandomLong(0, 1) )
	{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound(
				idx,
				origin,
				CHAN_WEAPON,
				"weapons/glauncher.wav",
				1,
				ATTN_NORM,
				0,
				94 + gEngfuncs.pfnRandomLong(0, 0xf));
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound(
				idx,
				origin,
				CHAN_WEAPON,
				"weapons/glauncher2.wav",
				1,
				ATTN_NORM,
				0,
				94 + gEngfuncs.pfnRandomLong(0, 0xf));
			break;
	}
}
//======================
//		 MP5 END
//======================

//======================
//	   PHYTON START
//	     ( .357 )
//======================
void EV_FirePython(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;
	// float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	AngleVectors(angles, forward, right, up);

	if ( EV_IsLocal(idx) )
	{
		// Python uses different body in multiplayer versus single player
		int multiplayer = gEngfuncs.GetMaxClients() == 1 ? 0 : 1;

		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation(PYTHON_FIRE1, multiplayer ? 1 : 0);

		V_PunchAxis(0, -10.0);
	}

	switch ( gEngfuncs.pfnRandomLong(0, 1) )
	{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound(
				idx,
				origin,
				CHAN_WEAPON,
				"weapons/357_shot1.wav",
				gEngfuncs.pfnRandomFloat(0.8f, 0.9f),
				ATTN_NORM,
				0,
				PITCH_NORM);
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound(
				idx,
				origin,
				CHAN_WEAPON,
				"weapons/357_shot2.wav",
				gEngfuncs.pfnRandomFloat(0.8f, 0.9f),
				ATTN_NORM,
				0,
				PITCH_NORM);
			break;
	}

	EV_GetGunPosition(args, vecSrc, origin);

	VectorCopy(forward, vecAiming);

	EV_HLDM_FireBullets(
		idx,
		forward,
		right,
		up,
		1,
		vecSrc,
		vecAiming,
		8192,
		BULLET_PLAYER_357,
		1,
		args->fparam1,
		args->fparam2);
}
//======================
//	    PHYTON END
//	     ( .357 )
//======================

//======================
//	   GAUSS START
//======================

void EV_SpinGauss(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	int iSoundState = 0;

	int pitch;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	pitch = args->iparam1;

	iSoundState = args->bparam1 ? SND_CHANGE_PITCH : 0;

	gEngfuncs.pEventAPI
		->EV_PlaySound(idx, origin, CHAN_WEAPON, "ambience/pulsemachine.wav", 1.0, ATTN_NORM, iSoundState, pitch);
}

/*
==============================
EV_StopPreviousGauss

==============================
*/
void EV_StopPreviousGauss(int idx)
{
	// Make sure we don't have a gauss spin event in the queue for this guy
	gEngfuncs.pEventAPI->EV_KillEvents(idx, "events/gaussspin.sc");
	gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_WEAPON, "ambience/pulsemachine.wav");
}

extern float g_flApplyVel;

void EV_FireGauss(event_args_t* args)
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	float flDamage = args->fparam1;
	// int primaryfire = args->bparam1;

	int m_fPrimaryFire = args->bparam1;
	// int m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	vec3_t vecSrc;
	vec3_t vecDest;
	// edict_t		*pentIgnore;
	pmtrace_t tr, beam_tr;
	float flMaxFrac = 1.0;
	// int nTotal = 0;
	int fHasPunched = 0;
	int fFirstBeam = 1;
	int nMaxHits = 10;
	physent_t* pEntity;
	int m_iBeam, m_iGlow, m_iBalls;
	vec3_t up, right, forward;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	VectorCopy(args->velocity, velocity);

	if ( args->bparam2 )
	{
		EV_StopPreviousGauss(idx);
		return;
	}

	// Con_Printf( "Firing gauss with %f\n", flDamage );
	EV_GetGunPosition(args, vecSrc, origin);

	m_iBeam = gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/smoke.spr");
	m_iBalls = m_iGlow = gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/hotglow.spr");

	AngleVectors(angles, forward, right, up);

	VectorMA(vecSrc, 8192, forward, vecDest);

	if ( EV_IsLocal(idx) )
	{
		V_PunchAxis(0, -2.0);
		gEngfuncs.pEventAPI->EV_WeaponAnimation(GAUSS_FIRE2, 2);

		if ( m_fPrimaryFire == false )
			g_flApplyVel = flDamage;
	}

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/gauss2.wav",
		0.5f + flDamage * (1.0f / 400.0f),
		ATTN_NORM,
		0,
		85 + gEngfuncs.pfnRandomLong(0, 0x1f));

	while ( flDamage > 10 && nMaxHits > 0 )
	{
		nMaxHits--;

		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

		// Store off the old count
		gEngfuncs.pEventAPI->EV_PushPMStates();

		// Now add in all of the players.
		gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);

		gEngfuncs.pEventAPI->EV_SetTraceHull(2);
		gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecDest, PM_STUDIO_BOX, -1, &tr);

		gEngfuncs.pEventAPI->EV_PopPMStates();

		if ( tr.allsolid )
			break;

		if ( fFirstBeam )
		{
			if ( EV_IsLocal(idx) )
			{
				// Add muzzle flash to current weapon model
				EV_MuzzleFlash();
			}
			fFirstBeam = 0;

			gEngfuncs.pEfxAPI->R_BeamEntPoint(
				idx | 0x1000,
				tr.endpos,
				m_iBeam,
				0.1f,
				m_fPrimaryFire ? 1.0f : 2.5f,
				0.0,
				m_fPrimaryFire ? 128.0f : flDamage,
				0,
				0,
				0,
				255.0f,
				m_fPrimaryFire ? 128.0f : 255.0f,
				m_fPrimaryFire ? 0.0f : 255.0f);
		}
		else
		{
			gEngfuncs.pEfxAPI->R_BeamPoints(
				vecSrc,
				tr.endpos,
				m_iBeam,
				0.1f,
				m_fPrimaryFire ? 1.0f : 2.5f,
				0.0,
				m_fPrimaryFire ? 128.0f : flDamage,
				0,
				0,
				0,
				255,
				m_fPrimaryFire ? 128.0f : 255.0f,
				m_fPrimaryFire ? 0.0f : 255.0f);
		}

		pEntity = gEngfuncs.pEventAPI->EV_GetPhysent(tr.ent);
		if ( pEntity == NULL )
			break;

		if ( pEntity->solid == SOLID_BSP )
		{
			float n;

			// pentIgnore = NULL;

			n = -DotProduct(tr.plane.normal, forward);

			if ( n < 0.5 )  // 60 degrees
			{
				// ALERT( at_console, "reflect %f\n", n );
				// reflect
				vec3_t r;

				VectorMA(forward, 2.0f * n, tr.plane.normal, r);

				flMaxFrac = flMaxFrac - tr.fraction;

				VectorCopy(r, forward);

				VectorMA(tr.endpos, 8.0, forward, vecSrc);
				VectorMA(vecSrc, 8192.0, forward, vecDest);

				gEngfuncs.pEfxAPI->R_TempSprite(
					tr.endpos,
					vec3_origin,
					0.2f,
					m_iGlow,
					kRenderGlow,
					kRenderFxNoDissipation,
					flDamage * n / 255.0f,
					flDamage * n * 0.5f * 0.1f,
					FTENT_FADEOUT);

				vec3_t fwd;
				VectorAdd(tr.endpos, tr.plane.normal, fwd);

				gEngfuncs.pEfxAPI->R_Sprite_Trail(
					TE_SPRITETRAIL,
					tr.endpos,
					fwd,
					m_iBalls,
					3,
					0.1f,
					gEngfuncs.pfnRandomFloat(10.0f, 20.0f) / 100.0f,
					100,
					255,
					100);

				// lose energy
				if ( n == 0 )
				{
					n = 0.1f;
				}

				flDamage = flDamage * (1 - n);
			}
			else
			{
				// tunnel
				EV_HLDM_DecalGunshot(&tr, BULLET_MONSTER_12MM);

				gEngfuncs.pEfxAPI->R_TempSprite(
					tr.endpos,
					vec3_origin,
					1.0,
					m_iGlow,
					kRenderGlow,
					kRenderFxNoDissipation,
					flDamage / 255.0f,
					6.0,
					FTENT_FADEOUT);

				// limit it to one hole punch
				if ( fHasPunched )
				{
					break;
				}
				fHasPunched = 1;

				// try punching through wall if secondary attack (primary is incapable of breaking through)
				if ( !m_fPrimaryFire )
				{
					vec3_t start;

					VectorMA(tr.endpos, 8.0, forward, start);

					// Store off the old count
					gEngfuncs.pEventAPI->EV_PushPMStates();

					// Now add in all of the players.
					gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);

					gEngfuncs.pEventAPI->EV_SetTraceHull(2);
					gEngfuncs.pEventAPI->EV_PlayerTrace(start, vecDest, PM_STUDIO_BOX, -1, &beam_tr);

					if ( !beam_tr.allsolid )
					{
						vec3_t delta;

						// trace backwards to find exit point
						gEngfuncs.pEventAPI->EV_PlayerTrace(beam_tr.endpos, tr.endpos, PM_STUDIO_BOX, -1, &beam_tr);

						VectorSubtract(beam_tr.endpos, tr.endpos, delta);

						n = VectorLength(delta);

						if ( n < flDamage )
						{
							if ( n == 0 )
								n = 1;
							flDamage -= n;

							// absorption balls
							{
								vec3_t fwd;
								VectorSubtract(tr.endpos, forward, fwd);
								gEngfuncs.pEfxAPI->R_Sprite_Trail(
									TE_SPRITETRAIL,
									tr.endpos,
									fwd,
									m_iBalls,
									3,
									0.1f,
									gEngfuncs.pfnRandomFloat(10.0f, 20.0f) / 100.0f,
									100,
									255,
									100);
							}

							//////////////////////////////////// WHAT TO DO HERE
							// CSoundEnt::InsertSound( bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3.0 );

							EV_HLDM_DecalGunshot(&beam_tr, BULLET_MONSTER_12MM);

							gEngfuncs.pEfxAPI->R_TempSprite(
								beam_tr.endpos,
								vec3_origin,
								0.1f,
								m_iGlow,
								kRenderGlow,
								kRenderFxNoDissipation,
								flDamage / 255.0f,
								6.0,
								FTENT_FADEOUT);

							// balls
							{
								vec3_t fwd;
								VectorSubtract(beam_tr.endpos, forward, fwd);
								gEngfuncs.pEfxAPI->R_Sprite_Trail(
									TE_SPRITETRAIL,
									beam_tr.endpos,
									fwd,
									m_iBalls,
									(int)(flDamage * 0.3),
									0.1f,
									gEngfuncs.pfnRandomFloat(10.0f, 20.0f) / 100.0f,
									200,
									255,
									40);
							}

							VectorAdd(beam_tr.endpos, forward, vecSrc);
						}
					}
					else
					{
						flDamage = 0;
					}

					gEngfuncs.pEventAPI->EV_PopPMStates();
				}
				else
				{
					if ( m_fPrimaryFire )
					{
						// slug doesn't punch through ever with primary
						// fire, so leave a little glowy bit and make some balls
						gEngfuncs.pEfxAPI->R_TempSprite(
							tr.endpos,
							vec3_origin,
							0.2f,
							m_iGlow,
							kRenderGlow,
							kRenderFxNoDissipation,
							200.0f / 255.0f,
							0.3f,
							FTENT_FADEOUT);
						{
							vec3_t fwd;
							VectorAdd(tr.endpos, tr.plane.normal, fwd);
							gEngfuncs.pEfxAPI->R_Sprite_Trail(
								TE_SPRITETRAIL,
								tr.endpos,
								fwd,
								m_iBalls,
								8,
								0.6f,
								gEngfuncs.pfnRandomFloat(10.0f, 20.0f) / 100.0f,
								100,
								255,
								200);
						}
					}

					flDamage = 0;
				}
			}
		}
		else
		{
			VectorAdd(tr.endpos, forward, vecSrc);
		}
	}
}
//======================
//	   GAUSS END
//======================

//======================
//	   CROWBAR START
//======================
enum crowbar_e
{
	CROWBAR_IDLE = 0,
	CROWBAR_DRAW,
	CROWBAR_HOLSTER,
	CROWBAR_ATTACK1HIT,
	CROWBAR_ATTACK1MISS,
	CROWBAR_ATTACK2MISS,
	CROWBAR_ATTACK2HIT,
	CROWBAR_ATTACK3MISS,
#ifndef CROWBAR_IDLE_ANIM
	CROWBAR_ATTACK3HIT
#else
	CROWBAR_ATTACK3HIT,
	CROWBAR_IDLE2,
	CROWBAR_IDLE3
#endif
};

int g_iSwing;

// Only predict the miss sounds, hit sounds are still played
// server side, so players don't get the wrong idea.
void EV_Crowbar(event_args_t* args)
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy(args->origin, origin);

	// Play Swing sound
	gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/cbar_miss1.wav", 1, ATTN_NORM, 0, PITCH_NORM);

	if ( EV_IsLocal(idx) )
	{
		switch ( (g_iSwing++) % 3 )
		{
			case 0:
				gEngfuncs.pEventAPI->EV_WeaponAnimation(CROWBAR_ATTACK1MISS, 1);
				break;
			case 1:
				gEngfuncs.pEventAPI->EV_WeaponAnimation(CROWBAR_ATTACK2MISS, 1);
				break;
			case 2:
				gEngfuncs.pEventAPI->EV_WeaponAnimation(CROWBAR_ATTACK3MISS, 1);
				break;
		}
	}
}
//======================
//	   CROWBAR END
//======================

//======================
//	  CROSSBOW START
//======================
enum crossbow_e
{
	CROSSBOW_IDLE1 = 0,  // full
	CROSSBOW_IDLE2,  // empty
	CROSSBOW_FIDGET1,  // full
	CROSSBOW_FIDGET2,  // empty
	CROSSBOW_FIRE1,  // full
	CROSSBOW_FIRE2,  // reload
	CROSSBOW_FIRE3,  // empty
	CROSSBOW_RELOAD,  // from empty
	CROSSBOW_DRAW1,  // full
	CROSSBOW_DRAW2,  // empty
	CROSSBOW_HOLSTER1,  // full
	CROSSBOW_HOLSTER2  // empty
};

//=====================
// EV_BoltCallback
// This function is used to correct the origin and angles
// of the bolt, so it looks like it's stuck on the wall.
//=====================
void EV_BoltCallback(struct tempent_s* ent, float, float)
{
	VectorCopy(ent->entity.baseline.vuser1, ent->entity.origin);
	VectorCopy(ent->entity.baseline.vuser2, ent->entity.angles);
}

void EV_FireCrossbow2(event_args_t* args)
{
	vec3_t vecSrc, vecEnd;
	vec3_t up, right, forward;
	pmtrace_t tr;

	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);

	VectorCopy(args->velocity, velocity);

	AngleVectors(angles, forward, right, up);

	EV_GetGunPosition(args, vecSrc, origin);

	VectorMA(vecSrc, 8192, forward, vecEnd);

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/xbow_fire1.wav",
		1,
		ATTN_NORM,
		0,
		93 + gEngfuncs.pfnRandomLong(0, 0xF));
	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_ITEM,
		"weapons/xbow_reload1.wav",
		gEngfuncs.pfnRandomFloat(0.95f, 1.0f),
		ATTN_NORM,
		0,
		93 + gEngfuncs.pfnRandomLong(0, 0xF));

	if ( EV_IsLocal(idx) )
	{
		if ( args->iparam1 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation(CROSSBOW_FIRE1, 1);
		else if ( args->iparam2 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation(CROSSBOW_FIRE3, 1);
	}

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);
	gEngfuncs.pEventAPI->EV_SetTraceHull(2);
	gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr);

	// We hit something
	if ( tr.fraction < 1.0 )
	{
		physent_t* pe = gEngfuncs.pEventAPI->EV_GetPhysent(tr.ent);

		// Not the world, let's assume we hit something organic ( dog, cat, uncle joe, etc ).
		if ( pe->solid != SOLID_BSP )
		{
			switch ( gEngfuncs.pfnRandomLong(0, 1) )
			{
				case 0:
					gEngfuncs.pEventAPI->EV_PlaySound(
						idx,
						tr.endpos,
						CHAN_BODY,
						"weapons/xbow_hitbod1.wav",
						1,
						ATTN_NORM,
						0,
						PITCH_NORM);
					break;
				case 1:
					gEngfuncs.pEventAPI->EV_PlaySound(
						idx,
						tr.endpos,
						CHAN_BODY,
						"weapons/xbow_hitbod2.wav",
						1,
						ATTN_NORM,
						0,
						PITCH_NORM);
					break;
			}
		}
		// Stick to world but don't stick to glass, it might break and leave the bolt floating. It can still stick to
		// other non-transparent breakables though.
		else if ( pe->rendermode == kRenderNormal )
		{
			gEngfuncs.pEventAPI->EV_PlaySound(
				0,
				tr.endpos,
				CHAN_BODY,
				"weapons/xbow_hit1.wav",
				gEngfuncs.pfnRandomFloat(0.95f, 1.0f),
				ATTN_NORM,
				0,
				PITCH_NORM);

			// Not underwater, do some sparks...
			if ( gEngfuncs.PM_PointContents(tr.endpos, NULL) != CONTENTS_WATER )
				gEngfuncs.pEfxAPI->R_SparkShower(tr.endpos);

			vec3_t vBoltAngles;
			int iModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex("models/crossbow_bolt.mdl");

			VectorAngles(forward, vBoltAngles);

			TEMPENTITY* bolt = gEngfuncs.pEfxAPI->R_TempModel(
				Vector(tr.endpos) - Vector(forward) * 10,
				Vector(0, 0, 0),
				vBoltAngles,
				5,
				iModelIndex,
				TE_BOUNCE_NULL);

			if ( bolt )
			{
				bolt->flags |= (FTENT_CLIENTCUSTOM);  // So it calls the callback function.
				(Vector(tr.endpos) - Vector(forward) * 10)
					.CopyToArray(bolt->entity.baseline.vuser1);  // Pull out a little bit
				VectorCopy(vBoltAngles, bolt->entity.baseline.vuser2);  // Look forward!
				bolt->callback =
					EV_BoltCallback;  // So we can set the angles and origin back. (Stick the bolt to the wall)
			}
		}
	}

	gEngfuncs.pEventAPI->EV_PopPMStates();
}

// TODO: Fully predict the fliying bolt.
void EV_FireCrossbow(event_args_t* args)
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy(args->origin, origin);

	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_WEAPON,
		"weapons/xbow_fire1.wav",
		1,
		ATTN_NORM,
		0,
		93 + gEngfuncs.pfnRandomLong(0, 0xF));
	gEngfuncs.pEventAPI->EV_PlaySound(
		idx,
		origin,
		CHAN_ITEM,
		"weapons/xbow_reload1.wav",
		gEngfuncs.pfnRandomFloat(0.95f, 1.0f),
		ATTN_NORM,
		0,
		93 + gEngfuncs.pfnRandomLong(0, 0xF));

	// Only play the weapon anims if I shot it.
	if ( EV_IsLocal(idx) )
	{
		if ( args->iparam1 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation(CROSSBOW_FIRE1, 1);
		else if ( args->iparam2 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation(CROSSBOW_FIRE3, 1);

		V_PunchAxis(0, -2.0);
	}
}
//======================
//	   CROSSBOW END
//======================

//======================
//	    RPG START
//======================

void EV_FireRpg(event_args_t* args)
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy(args->origin, origin);

	gEngfuncs.pEventAPI
		->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/rocketfire1.wav", 0.9f, ATTN_NORM, 0, PITCH_NORM);
	gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_ITEM, "weapons/glauncher.wav", 0.7f, ATTN_NORM, 0, PITCH_NORM);

	// Only play the weapon anims if I shot it.
	if ( EV_IsLocal(idx) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation(RPG_FIRE2, 1);

		V_PunchAxis(0, -5.0);
	}
}
//======================
//	     RPG END
//======================

//======================
//	    EGON END
//======================
enum egon_e
{
	EGON_IDLE1 = 0,
	EGON_FIDGET1,
	EGON_ALTFIREON,
	EGON_ALTFIRECYCLE,
	EGON_ALTFIREOFF,
	EGON_FIRE1,
	EGON_FIRE2,
	EGON_FIRE3,
	EGON_FIRE4,
	EGON_DRAW,
	EGON_HOLSTER
};

int g_fireAnims1[] = {EGON_FIRE1, EGON_FIRE2, EGON_FIRE3, EGON_FIRE4};
int g_fireAnims2[] = {EGON_ALTFIRECYCLE};

enum EGON_FIRESTATE
{
	FIRE_OFF,
	FIRE_CHARGE
};

enum EGON_FIREMODE
{
	FIRE_NARROW,
	FIRE_WIDE
};

#define EGON_PRIMARY_VOLUME 450
#define EGON_BEAM_SPRITE "sprites/xbeam1.spr"
#define EGON_FLARE_SPRITE "sprites/XSpark1.spr"
#define EGON_SOUND_OFF "weapons/egon_off1.wav"
#define EGON_SOUND_RUN "weapons/egon_run3.wav"
#define EGON_SOUND_STARTUP "weapons/egon_windup2.wav"

void EV_EgonFlareCallback(struct tempent_s* ent, float, float currenttime)
{
	float delta = currenttime - ent->tentOffset[2];  // time past since the last scale
	if ( delta >= ent->tentOffset[1] )
	{
		ent->entity.curstate.scale += ent->tentOffset[0] * delta;
		ent->tentOffset[2] = currenttime;
	}
}

void EV_EgonFire(event_args_t* args)
{
	int idx, /*iFireState,*/ iFireMode;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	// iFireState = args->iparam1;
	iFireMode = args->iparam2;
	int iStartup = args->bparam1;

	if ( iStartup )
	{
		if ( iFireMode == FIRE_WIDE )
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, EGON_SOUND_STARTUP, 0.98f, ATTN_NORM, 0, 125);
		else
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, EGON_SOUND_STARTUP, 0.9f, ATTN_NORM, 0, 100);
	}
	else
	{
		// If there is any sound playing already, kill it. - Solokiller
		// This is necessary because multiple sounds can play on the same channel at the same time.
		// In some cases, more than 1 run sound plays when the egon stops firing, in which case only the earliest entry
		// in the list is stopped. This ensures no more than 1 of those is ever active at the same time.
		gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_STATIC, EGON_SOUND_RUN);

		if ( iFireMode == FIRE_WIDE )
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_STATIC, EGON_SOUND_RUN, 0.98f, ATTN_NORM, 0, 125);
		else
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_STATIC, EGON_SOUND_RUN, 0.9f, ATTN_NORM, 0, 100);
	}

	// Only play the weapon anims if I shot it.
	if ( EV_IsLocal(idx) )
		gEngfuncs.pEventAPI->EV_WeaponAnimation(g_fireAnims1[gEngfuncs.pfnRandomLong(0, 3)], 1);

	if ( iStartup == 1 && EV_IsLocal(idx) && !(pBeam || pBeam2 || pFlare) &&
		 cl_weapon_prediction->value )  // Adrian: Added the cl_weapon_prediction check for those lital people that hate
										// weapon prediction.
	{
		vec3_t vecSrc, vecEnd, angles, forward, right, up;
		pmtrace_t tr;

		cl_entity_t* pl = gEngfuncs.GetEntityByIndex(idx);

		if ( pl )
		{
			VectorCopy(gHUD.m_vecAngles, angles);

			AngleVectors(angles, forward, right, up);

			EV_GetGunPosition(args, vecSrc, pl->origin);

			VectorMA(vecSrc, 2048, forward, vecEnd);

			gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

			// Store off the old count
			gEngfuncs.pEventAPI->EV_PushPMStates();

			// Now add in all of the players.
			gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);

			gEngfuncs.pEventAPI->EV_SetTraceHull(2);
			gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr);

			gEngfuncs.pEventAPI->EV_PopPMStates();

			int iBeamModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex(EGON_BEAM_SPRITE);

			float r = 50.0f;
			float g = 50.0f;
			float b = 125.0f;

			if ( IEngineStudio.IsHardware() )
			{
				r /= 100.0f;
				g /= 100.0f;
			}

			pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint(
				idx | 0x1000,
				tr.endpos,
				iBeamModelIndex,
				99999,
				3.5f,
				0.2f,
				0.7f,
				55,
				0,
				0,
				r,
				g,
				b);

			if ( pBeam )
				pBeam->flags |= (FBEAM_SINENOISE);

			pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint(
				idx | 0x1000,
				tr.endpos,
				iBeamModelIndex,
				99999,
				5.0,
				0.08f,
				0.7f,
				25,
				0,
				0,
				r,
				g,
				b);

			// Vit_amiN: egon beam flare
			pFlare = gEngfuncs.pEfxAPI->R_TempSprite(
				tr.endpos,
				vec3_origin,
				1.0,
				gEngfuncs.pEventAPI->EV_FindModelIndex(EGON_FLARE_SPRITE),
				kRenderGlow,
				kRenderFxNoDissipation,
				1.0,
				99999,
				FTENT_SPRCYCLE | FTENT_PERSIST);
		}
	}

	if ( pFlare )  // Vit_amiN: store the last mode for EV_EgonStop()
	{
		pFlare->tentOffset[0] = (iFireMode == FIRE_WIDE) ? 1.0f : 0.0f;
	}
}

void EV_EgonStop(event_args_t* args)
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy(args->origin, origin);

	gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_STATIC, EGON_SOUND_RUN);

	if ( args->iparam1 )
		gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, EGON_SOUND_OFF, 0.98f, ATTN_NORM, 0, 100);

	if ( EV_IsLocal(idx) )
	{
		if ( pBeam )
		{
			pBeam->die = 0.0;
			pBeam = NULL;
		}

		if ( pBeam2 )
		{
			pBeam2->die = 0.0;
			pBeam2 = NULL;
		}

		if ( pFlare )  // Vit_amiN: egon beam flare
		{
			pFlare->die = gEngfuncs.GetClientTime();

			if ( gEngfuncs.GetMaxClients() == 1 || !(pFlare->flags & FTENT_NOMODEL) )
			{
				if ( pFlare->tentOffset[0] != 0.0f )  // true for iFireMode == FIRE_WIDE
				{
					pFlare->callback = &EV_EgonFlareCallback;
					pFlare->fadeSpeed = 2.0f;  // fade out will take 0.5 sec
					pFlare->tentOffset[0] = 10.0f;  // scaling speed per second
					pFlare->tentOffset[1] = 0.1f;  // min time between two scales
					pFlare->tentOffset[2] = pFlare->die;  // the last callback run time
					pFlare->flags = FTENT_FADEOUT | FTENT_CLIENTCUSTOM;
				}
			}

			pFlare = NULL;
		}
	}
}
//======================
//	    EGON END
//======================

//======================
//	   HORNET START
//======================
enum hgun_e
{
	HGUN_IDLE1 = 0,
	HGUN_FIDGETSWAY,
	HGUN_FIDGETSHAKE,
	HGUN_DOWN,
	HGUN_UP,
	HGUN_SHOOT
};

void EV_HornetGunFire(event_args_t* args)
{
	int idx;  //, iFireMode;
	vec3_t origin;
	vec3_t angles;

	idx = args->entindex;
	VectorCopy(args->origin, origin);
	VectorCopy(args->angles, angles);
	// iFireMode = args->iparam1;

	// Only play the weapon anims if I shot it.
	if ( EV_IsLocal(idx) )
	{
		V_PunchAxis(0, static_cast<float>(gEngfuncs.pfnRandomLong(0, 2)));
		gEngfuncs.pEventAPI->EV_WeaponAnimation(HGUN_SHOOT, 1);
	}

	switch ( gEngfuncs.pfnRandomLong(0, 2) )
	{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "agrunt/ag_fire1.wav", 1, ATTN_NORM, 0, 100);
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "agrunt/ag_fire2.wav", 1, ATTN_NORM, 0, 100);
			break;
		case 2:
			gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "agrunt/ag_fire3.wav", 1, ATTN_NORM, 0, 100);
			break;
	}
}
//======================
//	   HORNET END
//======================

//======================
//	   TRIPMINE START
//======================
enum tripmine_e
{
	TRIPMINE_IDLE1 = 0,
	TRIPMINE_IDLE2,
	TRIPMINE_ARM1,
	TRIPMINE_ARM2,
	TRIPMINE_FIDGET,
	TRIPMINE_HOLSTER,
	TRIPMINE_DRAW,
	TRIPMINE_WORLD,
	TRIPMINE_GROUND
};

// We only check if it's possible to put a trip mine
// and if it is, then we play the animation. Server still places it.
void EV_TripmineFire(event_args_t* args)
{
	int idx;
	vec3_t vecSrc, angles, view_ofs, forward;
	pmtrace_t tr;

	idx = args->entindex;
	VectorCopy(args->origin, vecSrc);
	VectorCopy(args->angles, angles);

	AngleVectors(angles, forward, NULL, NULL);

	if ( !EV_IsLocal(idx) )
		return;

	// Grab predicted result for local player
	gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(view_ofs);

	VectorAdd(vecSrc, view_ofs, vecSrc);

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);
	gEngfuncs.pEventAPI->EV_SetTraceHull(2);
	gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, Vector(vecSrc) + Vector(forward) * 128, PM_NORMAL, -1, &tr);

	// Hit something solid
	if ( tr.fraction < 1.0 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation(TRIPMINE_DRAW, 0);

	gEngfuncs.pEventAPI->EV_PopPMStates();
}
//======================
//	   TRIPMINE END
//======================

//======================
//	   SQUEAK START
//======================
enum squeak_e
{
	SQUEAK_IDLE1 = 0,
	SQUEAK_FIDGETFIT,
	SQUEAK_FIDGETNIP,
	SQUEAK_DOWN,
	SQUEAK_UP,
	SQUEAK_THROW
};

void EV_SnarkFire(event_args_t* args)
{
	int idx;
	vec3_t vecSrc;
	vec3_t angles;
	vec3_t forward;
	pmtrace_t tr;

	idx = args->entindex;
	VectorCopy(args->origin, vecSrc);
	VectorCopy(args->angles, angles);

	AngleVectors(angles, forward, NULL, NULL);

	if ( !EV_IsLocal(idx) )
		return;

	if ( args->ducking )
	{
		VectorSubtract(vecSrc, (VEC_HULL_MIN - VEC_DUCK_HULL_MIN), vecSrc);
	}

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);
	gEngfuncs.pEventAPI->EV_SetTraceHull(2);
	gEngfuncs.pEventAPI->EV_PlayerTrace(
		Vector(vecSrc) + Vector(forward) * 20,
		Vector(vecSrc) + Vector(forward) * 64,
		PM_NORMAL,
		-1,
		&tr);

	// Find space to drop the thing.
	if ( tr.allsolid == 0 && tr.startsolid == 0 && tr.fraction > 0.25 )
		gEngfuncs.pEventAPI->EV_WeaponAnimation(SQUEAK_THROW, 0);

	gEngfuncs.pEventAPI->EV_PopPMStates();
}
//======================
//	   SQUEAK END
//======================

#endif  // HL_CONTENT

void EV_TrainPitchAdjust(event_args_t* args)
{
	int idx;
	vec3_t origin;

	unsigned short us_params;
	int noise;
	float m_flVolume;
	int pitch;
	int stop;

	const char* pszSound;

	idx = args->entindex;

	VectorCopy(args->origin, origin);

	us_params = (unsigned short)args->iparam1;
	stop = args->bparam1;

	m_flVolume = (float)(us_params & 0x003f) / 40.0f;
	noise = (int)(((us_params) >> 12) & 0x0007);
	pitch = (int)(10.0 * (float)((us_params >> 6) & 0x003f));

	switch ( noise )
	{
		case 1:
			pszSound = "plats/ttrain1.wav";
			break;
		case 2:
			pszSound = "plats/ttrain2.wav";
			break;
		case 3:
			pszSound = "plats/ttrain3.wav";
			break;
		case 4:
			pszSound = "plats/ttrain4.wav";
			break;
		case 5:
			pszSound = "plats/ttrain6.wav";
			break;
		case 6:
			pszSound = "plats/ttrain7.wav";
			break;
		default:
			// no sound
			return;
	}

	if ( stop )
	{
		gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_STATIC, pszSound);
	}
	else
	{
		gEngfuncs.pEventAPI
			->EV_PlaySound(idx, origin, CHAN_STATIC, pszSound, m_flVolume, ATTN_NORM, SND_CHANGE_PITCH, pitch);
	}
}

int EV_TFC_IsAllyTeam(int, int)
{
	return 0;
}
