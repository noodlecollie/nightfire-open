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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

#include "EnginePublicAPI/usercmd.h"
#include "EnginePublicAPI/entity_state.h"
#include "EnginePublicAPI/demo_api.h"
#include "EnginePublicAPI/pm_defs.h"
#include "EnginePublicAPI/event_api.h"
#include "EnginePublicAPI/r_efx.h"

#include "../hud_iface.h"
#include "../com_weapons.h"
#include "../demo.h"

#include "weaponregistry.h"
#include "genericweapon.h"
#include "weaponatts_collection.h"
#include "miniutl.h"
#include "PlatformLib/String.h"
#include "MathLib/utils.h"

extern globalvars_t* gpGlobals;
extern int g_iUser1;

// Pool of client side entities/entvars_t
static entvars_t ev[32];
static int num_ents = 0;

// The entity we'll use to represent the local client
static CBasePlayer player;

// Local version of game .dll global variables ( time, etc. )
static globalvars_t Globals;

static CBasePlayerWeapon* g_pWpns[MAX_LOCAL_WEAPONS];

float g_flApplyVel = 0.0;
int g_irunninggausspred = 0;

vec3_t previousorigin;

#ifdef HL_CONTENT
// HLDM Weapon placeholder entities.
CGlock g_Glock;
CCrowbar g_Crowbar;
CPython g_Python;
CMP5 g_Mp5;
CCrossbow g_Crossbow;
CShotgun g_Shotgun;
CRpg g_Rpg;
CGauss g_Gauss;
CEgon g_Egon;
CHgun g_HGun;
CHandGrenade g_HandGren;
CSatchel g_Satchel;
CTripmine g_Tripmine;
CSqueak g_Snark;
#endif  // HL_CONTENT

/*
======================
AlertMessage

Print debug messages to console
======================
*/
void AlertMessage(ALERT_TYPE, const char* szFmt, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, szFmt);
	PlatformLib_VSNPrintF(string, sizeof(string), szFmt, argptr);
	va_end(argptr);

	gEngfuncs.Con_Printf("cl:  ");
	gEngfuncs.Con_Printf(string);
}

// Returns if it's multiplayer.
// Mostly used by the client side weapons.
bool bIsMultiplayer(void)
{
	return gEngfuncs.GetMaxClients() == 1 ? 0 : 1;
}

// Just loads a v_ model.
void LoadVModel(const char* szViewModel, CBasePlayer* m_pPlayer)
{
	gEngfuncs.CL_LoadModel(szViewModel, &m_pPlayer->pev->viewmodel);
}

/*
=====================
HUD_PrepEntity

Links the raw entity to an entvars_s holder.  If a player is passed in as the owner, then
we set up the m_pPlayer field.
=====================
*/
void HUD_PrepEntity(CBaseEntity* pEntity, CBasePlayer* pWeaponOwner)
{
	ev[num_ents] = entvars_t {};

	pEntity->pev = &ev[num_ents++];

	pEntity->Precache();
	pEntity->Spawn();

	if ( pWeaponOwner )
	{
		ItemInfo info;

		((CBasePlayerWeapon*)pEntity)->m_pPlayer = pWeaponOwner;

		((CBasePlayerWeapon*)pEntity)->GetItemInfo(&info);

		g_pWpns[info.iId] = (CBasePlayerWeapon*)pEntity;
	}
}

/*
=====================
CBaseEntity::Killed

If weapons code "kills" an entity, just set its effects to EF_NODRAW
=====================
*/
void CBaseEntity::Killed(entvars_t*, int)
{
	pev->effects |= EF_NODRAW;
}

bool CBasePlayerWeapon::IsActiveItem() const
{
	return m_pPlayer->m_pActiveItem == this;
}

BOOL CBasePlayerWeapon::CanAttack(float attack_time, float curtime, BOOL isPredicted)
{
#if defined(CLIENT_WEAPONS)
	if ( !isPredicted )
#else
	if ( 1 )
#endif
	{
		return (attack_time <= curtime) ? TRUE : FALSE;
	}
	else
	{
		return (attack_time <= 0.0) ? TRUE : FALSE;
	}
}

/*
=====================
CBasePlayerWeapon::DefaultReload
=====================
*/
BOOL CBasePlayerWeapon::DefaultReload(int iClipSize, int iAnim, float fDelay, int body)
{
	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		return FALSE;

	int j = Q_min(iClipSize - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);

	if ( j == 0 )
		return FALSE;

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + fDelay;

	//!!UNDONE -- reload sound goes here !!!
	SendWeaponAnim(iAnim, body);

	m_fInReload = TRUE;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
	return TRUE;
}

/*
=====================
CBasePlayerWeapon::CanDeploy
=====================
*/
BOOL CBasePlayerWeapon::CanDeploy(void)
{
	BOOL bHasAmmo = 0;

	if ( !pszAmmo1() )
	{
		// this weapon doesn't use ammo, can always deploy.
		return TRUE;
	}

	if ( pszAmmo1() )
	{
		bHasAmmo |= (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] != 0);
	}
	if ( pszAmmo2() )
	{
		bHasAmmo |= (m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] != 0);
	}
	if ( m_iClip > 0 )
	{
		bHasAmmo |= 1;
	}
	if ( !bHasAmmo )
	{
		return FALSE;
	}

	return TRUE;
}

/*
=====================
CBasePlayerWeapon::DefaultDeploy

=====================
*/
BOOL CBasePlayerWeapon::DefaultDeploy(const char* szViewModel, const char*, int iAnim, const char*, int body)
{
	if ( !CanDeploy() )
		return FALSE;

	gEngfuncs.CL_LoadModel(szViewModel, &m_pPlayer->pev->viewmodel);

	SendWeaponAnim(iAnim, body);

	g_irunninggausspred = false;
	m_pPlayer->m_flNextAttack = 0.5;
	m_flTimeWeaponIdle = 1.0;
	return TRUE;
}

/*
=====================
CBasePlayerWeapon::PlayEmptySound

=====================
*/
BOOL CBasePlayerWeapon::PlayEmptySound(void)
{
	if ( m_iPlayEmptySound )
	{
		HUD_PlaySound("weapons/357_cock1.wav", 0.8f);
		m_iPlayEmptySound = 0;
		return 0;
	}
	return 0;
}

/*
=====================
CBasePlayerWeapon::ResetEmptySound

=====================
*/
void CBasePlayerWeapon::ResetEmptySound(void)
{
	m_iPlayEmptySound = 1;
}

/*
=====================
CBasePlayerWeapon::Holster

Put away weapon
=====================
*/
void CBasePlayerWeapon::Holster(int)
{
	m_fInReload = FALSE;  // cancel any reload in progress.
	g_irunninggausspred = false;
	m_pPlayer->pev->viewmodel = 0;
}

/*
=====================
CBasePlayerWeapon::SendWeaponAnim

Animate weapon model
=====================
*/
void CBasePlayerWeapon::SendWeaponAnim(int iAnim, int body)
{
	m_pPlayer->pev->weaponanim = iAnim;

	HUD_SendWeaponAnim(iAnim, body, 0);
}

/*
=====================
CBaseEntity::FireBulletsPlayer

Only produces random numbers to match the server ones.
=====================
*/
Vector CBaseEntity::FireBulletsPlayer(
	ULONG cShots,
	Vector,
	Vector,
	Vector vecSpread,
	float,
	int,
	int,
	int,
	entvars_t* pevAttacker,
	int shared_rand)
{
	float x = 0.0f, y = 0.0f, z;

	for ( ULONG iShot = 1; iShot <= cShots; iShot++ )
	{
		if ( pevAttacker == NULL )
		{
			// get circular gaussian spread
			do
			{
				x = RANDOM_FLOAT(-0.5, 0.5) + RANDOM_FLOAT(-0.5, 0.5);
				y = RANDOM_FLOAT(-0.5, 0.5) + RANDOM_FLOAT(-0.5, 0.5);
				z = x * x + y * y;
			}
			while ( z > 1 );
		}
		else
		{
			// Use player's random seed.
			//  get circular gaussian spread
			x = UTIL_SharedRandomFloat(shared_rand + iShot, -0.5, 0.5) +
				UTIL_SharedRandomFloat(shared_rand + (1 + iShot), -0.5, 0.5);
			y = UTIL_SharedRandomFloat(shared_rand + (2 + iShot), -0.5, 0.5) +
				UTIL_SharedRandomFloat(shared_rand + (3 + iShot), -0.5, 0.5);
			z = x * x + y * y;
		}
	}

	return Vector(x * vecSpread.x, y * vecSpread.y, 0.0);
}

/*
=====================
CBasePlayerWeapon::ItemPostFrame

Handles weapon firing, reloading, etc.
=====================
*/
void CBasePlayerWeapon::ItemPostFrame(void)
{
	if ( (m_fInReload) && (m_pPlayer->m_flNextAttack <= 0.0) )
	{
#if 0  // FIXME, need ammo on client to make this work right
	   // complete the reload.
		int j = Min( iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] );

		// Add them to the clip
		m_iClip += j;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= j;
#else
		m_iClip += 10;
#endif
		m_fInReload = FALSE;
	}

	if ( (m_pPlayer->pev->button & IN_ATTACK2) && (m_flNextSecondaryAttack <= 0.0) )
	{
		if ( pszAmmo2() && !m_pPlayer->m_rgAmmo[SecondaryAmmoIndex()] )
		{
			m_fFireOnEmpty = TRUE;
		}

		SecondaryAttack();
		m_pPlayer->pev->button &= ~IN_ATTACK2;
	}
	else if ( (m_pPlayer->pev->button & IN_ATTACK) && (m_flNextPrimaryAttack <= 0.0) )
	{
		if ( (m_iClip == 0 && pszAmmo1()) || (iMaxClip() == -1 && !m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()]) )
		{
			m_fFireOnEmpty = TRUE;
		}

		PrimaryAttack();
	}
	else if ( m_pPlayer->pev->button & IN_RELOAD && iMaxClip() != WEAPON_NOCLIP && !m_fInReload )
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
	}
	else if ( !(m_pPlayer->pev->button & (IN_ATTACK | IN_ATTACK2)) )
	{
		// no fire buttons down
		m_fFireOnEmpty = FALSE;

		// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
		if ( m_iClip == 0 && !(iFlags() & ITEM_FLAG_NOAUTORELOAD) && m_flNextPrimaryAttack < 0.0 )
		{
			Reload();
			return;
		}

		WeaponIdle();
		return;
	}

	// catch all
	if ( ShouldWeaponIdle() )
	{
		WeaponIdle();
	}
}

// If there ends up being any prediction data that's common to all weapons, it can be put here.
// By default, we just validate the pointer.
bool CBasePlayerWeapon::ReadPredictionData(const weapon_data_t* from)
{
	return from != NULL;
}

bool CBasePlayerWeapon::WritePredictionData(weapon_data_t* to)
{
	return to != NULL;
}

/*
=====================
CBasePlayer::SelectItem

  Switch weapons
=====================
*/
void CBasePlayer::SelectItem(const char* pstr)
{
	if ( !pstr )
		return;

	CBasePlayerItem* pItem = NULL;

	if ( !pItem )
		return;

	if ( pItem == m_pActiveItem )
		return;

	if ( m_pActiveItem )
		m_pActiveItem->Holster();

	m_pLastItem = m_pActiveItem;
	m_pActiveItem = pItem;

	if ( m_pActiveItem )
	{
		m_pActiveItem->Deploy();
	}
}

/*
=====================
CBasePlayer::SelectLastItem

=====================
*/
void CBasePlayer::SelectLastItem(void)
{
	if ( !m_pLastItem )
	{
		return;
	}

	if ( m_pActiveItem && !m_pActiveItem->CanHolster() )
	{
		return;
	}

	if ( m_pActiveItem )
		m_pActiveItem->Holster();

	CBasePlayerItem* pTemp = m_pActiveItem;
	m_pActiveItem = m_pLastItem;
	m_pLastItem = pTemp;
	m_pActiveItem->Deploy();
}

/*
=====================
CBasePlayer::Killed

=====================
*/
void CBasePlayer::Killed(entvars_t*, int)
{
	// Holster weapon immediately, to allow it to cleanup
	if ( m_pActiveItem )
		m_pActiveItem->Holster();

	g_irunninggausspred = false;
}

/*
=====================
CBasePlayer::Spawn

=====================
*/
void CBasePlayer::Spawn(void)
{
	if ( m_pActiveItem )
		m_pActiveItem->Deploy();

	g_irunninggausspred = false;
}

/*
=====================
UTIL_TraceLine

Don't actually trace, but act like the trace didn't hit anything.
=====================
*/
void UTIL_TraceLine(const Vector&, const Vector&, IGNORE_MONSTERS, edict_t*, TraceResult* ptr)
{
	*ptr = TraceResult {};
	ptr->flFraction = 1.0;
}

void UTIL_TraceHull(
	const Vector&,
	const Vector&,
	IGNORE_MONSTERS,
	int,
	edict_t*,
	TraceResult* ptr)
{
	*ptr = TraceResult {};
	ptr->flFraction = 1.0;
}

void UTIL_TraceHull(
	const Vector&,
	const Vector&,
	IGNORE_MONSTERS,
	const Vector&,
	const Vector&,
	edict_t*,
	TraceResult* ptr)
{
	*ptr = TraceResult {};
	ptr->flFraction = 1.0;
}

/*
=====================
UTIL_ParticleBox

For debugging, draw a box around a player made out of particles
=====================
*/
void UTIL_ParticleBox(
	CBasePlayer* inPlayer,
	float* mins,
	float* maxs,
	float,
	unsigned char r,
	unsigned char g,
	unsigned char b)
{
	int i;
	vec3_t mmin, mmax;

	for ( i = 0; i < 3; i++ )
	{
		mmin[i] = inPlayer->pev->origin[i] + mins[i];
		mmax[i] = inPlayer->pev->origin[i] + maxs[i];
	}

	gEngfuncs.pEfxAPI->R_ParticleBox((float*)mmin, (float*)mmax, r, g, b, 0);
}

/*
=====================
UTIL_ParticleBoxes

For debugging, draw boxes for other collidable players
=====================
*/
void UTIL_ParticleBoxes(void)
{
	int idx;
	physent_t* pe;
	cl_entity_t* localPlayer;
	vec3_t mins, maxs;

	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	localPlayer = gEngfuncs.GetLocalPlayer();
	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers(localPlayer->index - 1);

	for ( idx = 1; idx < 100; idx++ )
	{
		pe = gEngfuncs.pEventAPI->EV_GetPhysent(idx);
		if ( !pe )
			break;

		if ( pe->info >= 1 && pe->info <= gEngfuncs.GetMaxClients() )
		{
			VectorAdd(pe->origin, pe->mins, mins);
			VectorAdd(pe->origin, pe->maxs, maxs);

			gEngfuncs.pEfxAPI->R_ParticleBox((float*)&mins, (float*)&maxs, 0, 0, 255, 2.0);
		}
	}

	gEngfuncs.pEventAPI->EV_PopPMStates();
}

/*
=====================
UTIL_ParticleLine

For debugging, draw a line made out of particles
=====================
*/
void UTIL_ParticleLine(
	CBasePlayer*,
	float* start,
	float* end,
	float life,
	unsigned char r,
	unsigned char g,
	unsigned char b)
{
	gEngfuncs.pEfxAPI->R_ParticleLine(start, end, r, g, b, life);
}

/*
=====================
HUD_InitClientWeapons

Set up weapons, player and functions needed to run weapons code client-side.
=====================
*/
void HUD_InitClientWeapons(void)
{
	static int initialized = 0;
	if ( initialized )
		return;

	initialized = 1;

	// Set up pointer ( dummy object )
	gpGlobals = &Globals;

	// Fill in current time ( probably not needed )
	gpGlobals->time = gEngfuncs.GetClientTime();

	// Fake functions
	g_engfuncs.pfnPrecacheModel = stub_PrecacheModel;
	g_engfuncs.pfnPrecacheSound = stub_PrecacheSound;
	g_engfuncs.pfnPrecacheEvent = stub_PrecacheEvent;
	g_engfuncs.pfnNameForFunction = stub_NameForFunction;
	g_engfuncs.pfnSetModel = stub_SetModel;
	g_engfuncs.pfnSetClientMaxspeed = HUD_SetMaxSpeed;

	// Handled locally
	g_engfuncs.pfnPlaybackEvent = HUD_PlaybackEvent;
	g_engfuncs.pfnAlertMessage = AlertMessage;

	// Pass through to engine
	g_engfuncs.pfnPrecacheEvent = gEngfuncs.pfnPrecacheEvent;
	g_engfuncs.pfnRandomFloat = gEngfuncs.pfnRandomFloat;
	g_engfuncs.pfnRandomLong = gEngfuncs.pfnRandomLong;

	// Allocate a slot for the local player
	HUD_PrepEntity(&player, NULL);

	CWeaponRegistry::StaticInstance().ForEach(
		[](const WeaponAtts::WACollection& atts)
		{
			CBaseEntity* predictionWeapon = atts.Prediction.GetWeapon();
			ASSERT(predictionWeapon);

			HUD_PrepEntity(predictionWeapon, &player);
		});

#ifdef HL_CONTENT
	// Allocate slot(s) for each weapon that we are going to be predicting
	HUD_PrepEntity(&g_Glock, &player);
	HUD_PrepEntity(&g_Crowbar, &player);
	HUD_PrepEntity(&g_Python, &player);
	HUD_PrepEntity(&g_Mp5, &player);
	HUD_PrepEntity(&g_Crossbow, &player);
	HUD_PrepEntity(&g_Shotgun, &player);
	HUD_PrepEntity(&g_Rpg, &player);
	HUD_PrepEntity(&g_Gauss, &player);
	HUD_PrepEntity(&g_Egon, &player);
	HUD_PrepEntity(&g_HGun, &player);
	HUD_PrepEntity(&g_HandGren, &player);
	HUD_PrepEntity(&g_Satchel, &player);
	HUD_PrepEntity(&g_Tripmine, &player);
	HUD_PrepEntity(&g_Snark, &player);
#endif  // HL_CONTENT
}

/*
=====================
HUD_GetLastOrg

Retruns the last position that we stored for egon beam endpoint.
=====================
*/
void HUD_GetLastOrg(float* org)
{
	int i;

	// Return last origin
	for ( i = 0; i < 3; i++ )
	{
		org[i] = previousorigin[i];
	}
}

/*
=====================
HUD_SetLastOrg

Remember our exact predicted origin so we can draw the egon to the right position.
=====================
*/
void HUD_SetLastOrg(void)
{
	int i;

	// Offset final origin by view_offset
	for ( i = 0; i < 3; i++ )
	{
		previousorigin[i] = g_finalstate->playerstate.origin[i] + g_finalstate->client.view_ofs[i];
	}
}

/*
=====================
HUD_WeaponsPostThink

Run Weapon firing code on client
=====================
*/
void HUD_WeaponsPostThink(local_state_s* from, local_state_s* to, usercmd_t* cmd, double time, unsigned int random_seed)
{
	int i;
	int buttonsChanged;
	CBasePlayerWeapon* pWeapon = NULL;
	CBasePlayerWeapon* pCurrent;
	weapon_data_t* pfrom;
	weapon_data_t* pto;
	static int lasthealth;

	// This is a bit of a hack, since we don't have a good way here to store
	// state from the previous frame. If the player picks up a weapon, their
	// active weapon will change without there being a weapon change command
	// sent (because the weapon change is automatic, rather than being
	// invoked by the player). If we only call Deploy() on the new weapon
	// only on a weapon change event, the deploy animation will not be played
	// when the user picks up a new weapon. Therefore, we have to keep track
	// of the last weapon the player was holding, and call Deploy() if that
	// changes.
	static int lastWeaponID = -1;
	static double lastTime = 0.0f;

	HUD_InitClientWeapons();

	// Get current clock
	gpGlobals->time = static_cast<float>(time);

	if ( gpGlobals->time < lastTime )
	{
		// Assume that the previous run of prediction has come to an end,
		// so reset values.
		lasthealth = 0;
		lastWeaponID = -1;
	}

	const WeaponAtts::WACollection* atts = CWeaponRegistry::StaticInstance().Get(from->client.m_iId);
	if ( atts )
	{
		pWeapon = g_pWpns[static_cast<uint32_t>(atts->Core.Id)];
	}

#ifdef HL_CONTENT
	if ( !pWeapon )
	{
		// NFTODO: Once all weapons use attributes, this can go.
		switch ( from->client.m_iId )
		{
			case WEAPON_CROWBAR:
				pWeapon = &g_Crowbar;
				break;
			case WEAPON_GLOCK:
				pWeapon = &g_Glock;
				break;
			case WEAPON_PYTHON:
				pWeapon = &g_Python;
				break;
			case WEAPON_MP5:
				pWeapon = &g_Mp5;
				break;
			case WEAPON_CROSSBOW:
				pWeapon = &g_Crossbow;
				break;
			case WEAPON_SHOTGUN:
				pWeapon = &g_Shotgun;
				break;
			case WEAPON_RPG:
				pWeapon = &g_Rpg;
				break;
			case WEAPON_GAUSS:
				pWeapon = &g_Gauss;
				break;
			case WEAPON_EGON:
				pWeapon = &g_Egon;
				break;
			case WEAPON_HORNETGUN:
				pWeapon = &g_HGun;
				break;
			case WEAPON_HANDGRENADE:
				pWeapon = &g_HandGren;
				break;
			case WEAPON_SATCHEL:
				pWeapon = &g_Satchel;
				break;
			case WEAPON_TRIPMINE:
				pWeapon = &g_Tripmine;
				break;
			case WEAPON_SNARK:
				pWeapon = &g_Snark;
				break;
		}
	}
#endif  // HL_CONTENT

	// Store pointer to our destination entity_state_t so we can get our origin, etc. from it
	//  for setting up events on the client
	g_finalstate = to;

	// If we are running events/etc. go ahead and see if we
	//  managed to die between last frame and this one
	// If so, run the appropriate player killed or spawn function
	if ( g_runfuncs )
	{
		if ( to->client.health <= 0 && lasthealth > 0 )
		{
			player.Killed(NULL, 0);
		}
		else if ( to->client.health > 0 && lasthealth <= 0 )
		{
			player.Spawn();
		}

		lasthealth = static_cast<int>(to->client.health);
	}

	// We are not predicting the current weapon, just bow out here.
	if ( !pWeapon )
	{
		lastWeaponID = from->client.m_iId;
		lastTime = time;
		return;
	}

	for ( i = 0; i < MAX_LOCAL_WEAPONS; i++ )
	{
		pCurrent = g_pWpns[i];
		if ( !pCurrent )
		{
			continue;
		}

		pfrom = &from->weapondata[i];

		pCurrent->m_fInReload = pfrom->m_fInReload;
		pCurrent->m_fInSpecialReload = pfrom->m_fInSpecialReload;
		pCurrent->m_iClip = pfrom->m_iClip;
		pCurrent->m_flNextPrimaryAttack = pfrom->m_flNextPrimaryAttack;
		pCurrent->m_flNextSecondaryAttack = pfrom->m_flNextSecondaryAttack;
		pCurrent->m_flTimeWeaponIdle = pfrom->m_flTimeWeaponIdle;
		pCurrent->m_flEnqueuedMechanicInvocationTime = pfrom->m_flEnqueuedMechanicInvocationTime;
		pCurrent->m_flLastPrimaryAttack = pfrom->m_flLastPrimaryAttack;
		pCurrent->m_flLastSecondaryAttack = pfrom->m_flLastSecondaryAttack;

		pCurrent->ReadPredictionData(pfrom);

		pCurrent->m_iSecondaryAmmoType = (int)from->client.vuser3[2];
		pCurrent->m_iPrimaryAmmoType = (int)from->client.vuser4[0];
		player.m_rgAmmo[pCurrent->m_iPrimaryAmmoType] = (int)from->client.vuser4[1];
		player.m_rgAmmo[pCurrent->m_iSecondaryAmmoType] = (int)from->client.vuser4[2];
	}

	// For random weapon events, use this seed to seed random # generator
	player.random_seed = random_seed;

	// Get old buttons from previous state.
	player.m_afButtonLast = from->playerstate.oldbuttons;

	// Which buttsons chave changed
	buttonsChanged = (player.m_afButtonLast ^ cmd->buttons);  // These buttons have changed this frame

	// Debounced button codes for pressed/released
	// The changed ones still down are "pressed"
	player.m_afButtonPressed = buttonsChanged & cmd->buttons;
	// The ones not down are "released"
	player.m_afButtonReleased = buttonsChanged & (~cmd->buttons);

	// Set player variables that weapons code might check/alter
	player.pev->button = cmd->buttons;

	VectorCopy(from->client.velocity, player.pev->velocity);
	player.pev->flags = from->client.flags;

	player.pev->deadflag = from->client.deadflag;
	player.pev->waterlevel = from->client.waterlevel;
	player.pev->maxspeed = from->client.maxspeed;
	player.pev->fov = from->client.fov;
	player.pev->weaponanim = from->client.weaponanim;
	player.m_iWeaponScreenOverlay = ScreenOverlays::ToOverlayId(from->client.weaponScreenOverlay);
	player.m_flWeaponInaccuracy = from->client.weaponInaccuracy;
	player.pev->viewmodel = from->client.viewmodel;
	player.m_flNextAttack = from->client.m_flNextAttack;
	player.m_flNextAmmoBurn = from->client.fuser2;
	player.m_flAmmoStartCharge = from->client.fuser3;

	// Point to current weapon object
	if ( from->client.m_iId )
	{
		player.m_pActiveItem = g_pWpns[from->client.m_iId];
	}

#ifdef HL_CONTENT
	if ( player.m_pActiveItem->m_iId == WEAPON_RPG )
	{
		((CRpg*)player.m_pActiveItem)->m_fSpotActive = (int)from->client.vuser2[1];
		((CRpg*)player.m_pActiveItem)->m_cActiveRockets = (int)from->client.vuser2[2];
	}
#endif  // HL_CONTENT

	// Don't go firing anything if we have died.
	// Or if we don't have a weapon model deployed
	if ( (player.pev->deadflag != (DEAD_DISCARDBODY + 1)) && !CL_IsDead() && player.pev->viewmodel && !g_iUser1 )
	{
		if ( player.m_flNextAttack <= 0 )
		{
			pWeapon->ItemPostFrame();
		}
	}

	// Assume that we are not going to switch weapons
	to->client.m_iId = from->client.m_iId;

	// NFTODO: The following is ugly. The prediction system really needs to
	// be refactored to make it less icky, but for now we do the following:

	// Assume the next weapon is not different (signified by -1).
	int nextWeaponID = -1;

	if ( cmd->weaponselect )
	{
		// Old method:
		// We explicitly got a weapon select command, so take
		// the next weapon ID from this.
		nextWeaponID = cmd->weaponselect;
	}
	else if ( lastWeaponID != from->client.m_iId )
	{
		// New method:
		// The last weapon was different to the current one,
		// so we may need to call Deploy().
		nextWeaponID = from->client.m_iId;
	}

	// Now see if we need to deal with this weapon, and we're not dead.
	if ( nextWeaponID >= 0 && player.pev->deadflag != (DEAD_DISCARDBODY + 1) )
	{
		// Switched to a different weapon?
		if ( from->weapondata[nextWeaponID].m_iId == nextWeaponID )
		{
			CBasePlayerWeapon* pNew = g_pWpns[nextWeaponID];

			// This used to check pNew != pWeapon, but that check will fail
			// if a weapon select command has not been issued, as pWeapon
			// is already set to the current weapon. Instead, we now check
			// against the last cached weapon ID.
			if ( pNew && lastWeaponID != from->client.m_iId )
			{
				// Put away old weapon
				if ( player.m_pActiveItem )
				{
					player.m_pActiveItem->Holster();
				}

				player.m_pLastItem = player.m_pActiveItem;
				player.m_pActiveItem = pNew;

				// Deploy new weapon
				if ( player.m_pActiveItem )
				{
					player.m_pActiveItem->Deploy();
				}

				// Update weapon id so we can predict things correctly.
				to->client.m_iId = nextWeaponID;
			}
		}
	}

	// Copy in results of prediction code
	to->client.viewmodel = player.pev->viewmodel;
	to->client.fov = player.pev->fov;
	to->client.weaponanim = player.pev->weaponanim;
	to->client.weaponScreenOverlay = player.m_iWeaponScreenOverlay;
	to->client.weaponInaccuracy = player.m_flWeaponInaccuracy;
	to->client.m_flNextAttack = player.m_flNextAttack;
	to->client.fuser2 = player.m_flNextAmmoBurn;
	to->client.fuser3 = player.m_flAmmoStartCharge;
	to->client.maxspeed = player.pev->maxspeed;

#ifdef HL_CONTENT
	if ( player.m_pActiveItem->m_iId == WEAPON_RPG )
	{
		from->client.vuser2[1] = static_cast<float>(((CRpg*)player.m_pActiveItem)->m_fSpotActive);
		from->client.vuser2[2] = static_cast<float>(((CRpg*)player.m_pActiveItem)->m_cActiveRockets);
	}

	// Make sure that weapon animation matches what the game .dll is telling us
	//  over the wire ( fixes some animation glitches )
	// NFTODO: Remove this once all the HL weapons are gone. It doesn't apply to Afterburner weapons.
	if ( g_runfuncs && (HUD_GetWeaponAnim() != to->client.weaponanim) && !dynamic_cast<CGenericWeapon*>(pWeapon) )
	{
		int body = 0;

		// Pop the model to body 0.
		if ( pWeapon == &g_Tripmine )
			body = 0;

		// Show laser sight/scope combo
		if ( pWeapon == &g_Python && bIsMultiplayer() )
			body = 1;

		// Force a fixed anim down to viewmodel
		HUD_SendWeaponAnim(to->client.weaponanim, body, 1);
	}
#endif  // HL_CONTENT

	for ( i = 0; i < MAX_LOCAL_WEAPONS; i++ )
	{
		pCurrent = g_pWpns[i];

		pto = &to->weapondata[i];

		if ( !pCurrent )
		{
			memset(pto, 0, sizeof(weapon_data_t));
			continue;
		}

		pto->m_fInReload = pCurrent->m_fInReload;
		pto->m_fInSpecialReload = pCurrent->m_fInSpecialReload;
		pto->m_iClip = pCurrent->m_iClip;
		pto->m_flNextPrimaryAttack = pCurrent->m_flNextPrimaryAttack;
		pto->m_flNextSecondaryAttack = pCurrent->m_flNextSecondaryAttack;
		pto->m_flTimeWeaponIdle = pCurrent->m_flTimeWeaponIdle;
		pto->m_flEnqueuedMechanicInvocationTime = pCurrent->m_flEnqueuedMechanicInvocationTime;
		pto->m_flLastPrimaryAttack = pCurrent->m_flLastPrimaryAttack;
		pto->m_flLastSecondaryAttack = pCurrent->m_flLastSecondaryAttack;

		pCurrent->WritePredictionData(pto);

		// Decrement weapon counters, server does this at same time ( during post think, after doing everything else )
		pto->m_flNextReload -= cmd->msec / 1000.0f;
		pto->m_fNextAimBonus -= cmd->msec / 1000.0f;
		pto->m_flNextPrimaryAttack -= cmd->msec / 1000.0f;
		pto->m_flNextSecondaryAttack -= cmd->msec / 1000.0f;
		pto->m_flTimeWeaponIdle -= cmd->msec / 1000.0f;
		pto->m_flEnqueuedMechanicInvocationTime -= cmd->msec / 1000.0f;
		pto->m_flLastPrimaryAttack -= cmd->msec / 1000.0f;
		pto->m_flLastSecondaryAttack -= cmd->msec / 1000.0f;
		pto->tuser1 -= cmd->msec / 1000.0f;
		pto->tuser2 -= cmd->msec / 1000.0f;
		pto->tuser3 -= cmd->msec / 1000.0f;
		pto->tuser4 -= cmd->msec / 1000.0f;

		to->client.vuser3[2] = static_cast<float>(pCurrent->m_iSecondaryAmmoType);
		to->client.vuser4[0] = static_cast<float>(pCurrent->m_iPrimaryAmmoType);
		to->client.vuser4[1] = static_cast<float>(player.m_rgAmmo[pCurrent->m_iPrimaryAmmoType]);
		to->client.vuser4[2] = static_cast<float>(player.m_rgAmmo[pCurrent->m_iSecondaryAmmoType]);

		if ( pto->m_fNextAimBonus < -1.0f )
		{
			pto->m_fNextAimBonus = -1.0f;
		}

		if ( pto->m_flNextPrimaryAttack < -1.0f )
		{
			pto->m_flNextPrimaryAttack = -1.0f;
		}

		if ( pto->m_flNextSecondaryAttack < -0.001f )
		{
			pto->m_flNextSecondaryAttack = -0.001f;
		}

		if ( pto->m_flTimeWeaponIdle < -0.001f )
		{
			pto->m_flTimeWeaponIdle = -0.001f;
		}

		if ( pto->m_flEnqueuedMechanicInvocationTime < -0.001f )
		{
			pto->m_flEnqueuedMechanicInvocationTime = -0.001f;
		}

		if ( pto->m_flLastPrimaryAttack < -10.0f )
		{
			pto->m_flLastPrimaryAttack = -10.0f;
		}

		if ( pto->m_flLastSecondaryAttack < -10.0f )
		{
			pto->m_flLastSecondaryAttack = -10.0f;
		}

		if ( pto->m_flNextReload < -0.001f )
		{
			pto->m_flNextReload = -0.001f;
		}

		if ( pto->tuser1 < -0.001f )
		{
			pto->tuser1 = -0.001f;
		}

		if ( pto->tuser2 < -0.001f )
		{
			pto->tuser2 = -0.001f;
		}

		if ( pto->tuser3 < -0.001f )
		{
			pto->tuser3 = -0.001f;
		}

		if ( pto->tuser4 < -0.001f )
		{
			pto->tuser4 = -0.001f;
		}
	}

	// m_flNextAttack is not part of the weapons, but is part of the player instead
	to->client.m_flNextAttack -= cmd->msec / 1000.0f;
	if ( to->client.m_flNextAttack < -0.001f )
	{
		to->client.m_flNextAttack = -0.001f;
	}

	to->client.fuser2 -= cmd->msec / 1000.0f;
	if ( to->client.fuser2 < -0.001f )
	{
		to->client.fuser2 = -0.001f;
	}

	to->client.fuser3 -= cmd->msec / 1000.0f;
	if ( to->client.fuser3 < -0.001f )
	{
		to->client.fuser3 = -0.001f;
	}

	// Store off the last position from the predicted state.
	HUD_SetLastOrg();

	// Wipe it so we can't use it after this frame
	g_finalstate = NULL;

	lastWeaponID = to->client.m_iId;
	lastTime = time;
}

/*
=====================
HUD_PostRunCmd

Client calls this during prediction, after it has moved the player and updated any info changed into to->
time is the current client clock based on prediction
cmd is the command that caused the movement, etc
runfuncs is 1 if this is the first time we've predicted this command.  If so, sounds and effects should play, otherwise,
they should be ignored
=====================
*/
void _DLLEXPORT HUD_PostRunCmd(
	struct local_state_s* from,
	struct local_state_s* to,
	struct usercmd_s* cmd,
	int runfuncs,
	double time,
	unsigned int random_seed)
{
	g_runfuncs = runfuncs;

#if defined(CLIENT_WEAPONS)
	if ( cl_weapon_prediction && cl_weapon_prediction->value )
	{
		HUD_WeaponsPostThink(from, to, cmd, time, random_seed);
	}
	else
#endif
	{
		to->client.fov = g_lastFOV;
	}

	if ( g_irunninggausspred == 1 )
	{
		Vector forward;
		AngleVectors(v_angles, forward, NULL, NULL);
		(Vector(to->client.velocity) - forward * g_flApplyVel * 5).CopyToArray(to->client.velocity);
		g_irunninggausspred = false;
	}

	// All games can use FOV state
	g_lastFOV = to->client.fov;
}
