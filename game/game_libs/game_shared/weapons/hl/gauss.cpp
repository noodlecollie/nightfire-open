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
#if !defined(OEM_BUILD) && !defined(HLDEMO_BUILD)

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "EnginePublicAPI/shake.h"
#include "gamerules.h"
#include "game.h"
#include "ammodefs.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "radialdamage.h"
#include "EnginePublicAPI/event_args.h"
#include "eventConstructor/eventConstructor.h"

LINK_ENTITY_TO_CLASS(weapon_gauss, CGauss)

float CGauss::GetFullChargeTime(void)
{
#ifdef CLIENT_DLL
	if ( bIsMultiplayer() )
#else
	if ( g_pGameRules->IsMultiplayer() )
#endif
	{
		return 1.5;
	}

	return 4;
}

#ifdef CLIENT_DLL
extern int g_irunninggausspred;
#endif

void CGauss::Spawn()
{
	Precache();
	m_iId = WEAPON_GAUSS;
	SET_MODEL(ENT(pev), "models/w_gauss.mdl");

	m_iDefaultAmmo = GAUSS_DEFAULT_GIVE;

	FallInit();  // get ready to fall down.
}

void CGauss::Precache(void)
{
	PRECACHE_MODEL("models/w_gauss.mdl");
	PRECACHE_MODEL("models/v_gauss.mdl");
	PRECACHE_MODEL("models/p_gauss.mdl");

	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND("weapons/gauss2.wav");
	PRECACHE_SOUND("weapons/electro4.wav");
	PRECACHE_SOUND("weapons/electro5.wav");
	PRECACHE_SOUND("weapons/electro6.wav");
	PRECACHE_SOUND("ambience/pulsemachine.wav");

	m_iGlow = PRECACHE_MODEL("sprites/hotglow.spr");
	m_iBalls = PRECACHE_MODEL("sprites/hotglow.spr");
	m_iBeam = PRECACHE_MODEL("sprites/smoke.spr");

	m_usGaussFire = PRECACHE_EVENT(1, "events/gauss.sc");
	m_usGaussSpin = PRECACHE_EVENT(1, "events/gaussspin.sc");
}

int CGauss::AddToPlayer(CBasePlayer* pPlayer)
{
	if ( CBasePlayerWeapon::AddToPlayer(pPlayer) )
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

int CGauss::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = AmmoDef_Uranium.AmmoName;
	p->iMaxAmmo1 = AmmoDef_Uranium.MaxCarry;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	p->iPosition = 1;
	p->iId = m_iId = WEAPON_GAUSS;
	p->iFlags = 0;
	p->iWeight = GAUSS_WEIGHT;

	return 1;
}

BOOL CGauss::IsUseable()
{
	// Currently charging, allow the player to fire it first. - Solokiller
	return CBasePlayerWeapon::IsUseable() || m_fInAttack != 0;
}

BOOL CGauss::Deploy()
{
	m_pPlayer->m_flPlayAftershock = 0.0;
	return DefaultDeploy("models/v_gauss.mdl", "models/p_gauss.mdl", GAUSS_DRAW, "gauss");
}

void CGauss::Holster(int)
{
	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(FEV_GLOBAL | FEV_RELIABLE) << Invoker(m_pPlayer->edict()) << EventIndex(m_usGaussFire)
		  << Delay(0.01f) << Origin(m_pPlayer->pev->origin) << Angles(m_pPlayer->pev->angles) << BoolParam2(true);

	event.Send();

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	SendWeaponAnim(GAUSS_HOLSTER);
	m_fInAttack = 0;
}

void CGauss::PrimaryAttack()
{
	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		PlayEmptySound();
		m_flNextSecondaryAttack = m_flNextPrimaryAttack = GetNextAttackDelay(0.15f);
		return;
	}

	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 2 )
	{
		PlayEmptySound();
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
		return;
	}

	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	m_fPrimaryFire = TRUE;

	m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= 2;

	StartFire();
	m_fInAttack = 0;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.2f;
}

void CGauss::SecondaryAttack()
{
	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		if ( m_fInAttack != 0 )
		{
			EMIT_SOUND_DYN(
				ENT(m_pPlayer->pev),
				CHAN_WEAPON,
				"weapons/electro4.wav",
				1.0,
				ATTN_NORM,
				0,
				80 + RANDOM_LONG(0, 0x3f));
			SendWeaponAnim(GAUSS_IDLE);
			m_fInAttack = 0;
		}
		else
		{
			PlayEmptySound();
		}

		m_flNextSecondaryAttack = m_flNextPrimaryAttack = GetNextAttackDelay(0.5);
		return;
	}

	if ( m_fInAttack == 0 )
	{
		if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		{
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8f, ATTN_NORM);
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
			return;
		}

		m_fPrimaryFire = FALSE;

		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;  // take one ammo just to start the spin
		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

		// spin up
		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;

		SendWeaponAnim(GAUSS_SPINUP);
		m_fInAttack = 1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5f;
		m_pPlayer->m_flStartCharge = gpGlobals->time;
		m_pPlayer->m_flAmmoStartCharge = UTIL_WeaponTimeBase() + GetFullChargeTime();

		using namespace EventConstructor;
		CEventConstructor event;

		event << Flags(FEV_NOTHOST) << Invoker(m_pPlayer->edict()) << EventIndex(m_usGaussSpin) << IntParam1(110);

		event.Send();

		m_iSoundState = SND_CHANGE_PITCH;
	}
	else if ( m_fInAttack == 1 )
	{
		if ( m_flTimeWeaponIdle < UTIL_WeaponTimeBase() )
		{
			SendWeaponAnim(GAUSS_SPIN);
			m_fInAttack = 2;
		}
	}
	else
	{
		// Moved to before the ammo burn.
		// Because we drained 1 when m_InAttack == 0, then 1 again now before checking if we're out of ammo,
		// this resuled in the player having -1 ammo, which in turn caused CanDeploy to think it could be deployed.
		// This will need to be fixed further down the line by preventing negative ammo unless explicitly required
		// (infinite ammo?), But this check will prevent the problem for now. - Solokiller
		// TODO: investigate further.
		if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		{
			// out of ammo! force the gun to fire
			StartFire();
			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1;
			return;
		}

		// during the charging process, eat one bit of ammo every once in a while
		if ( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_pPlayer->m_flNextAmmoBurn != 1000 )
		{
#ifdef CLIENT_DLL
			if ( bIsMultiplayer() )
#else
			if ( g_pGameRules->IsMultiplayer() )
#endif
			{
				m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + 0.1f;
			}
			else
			{
				m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + 0.3f;
			}
		}

		if ( UTIL_WeaponTimeBase() >= m_pPlayer->m_flAmmoStartCharge )
		{
			// don't eat any more ammo after gun is fully charged.
			m_pPlayer->m_flNextAmmoBurn = 1000;
		}

		int pitch = (int)((gpGlobals->time - m_pPlayer->m_flStartCharge) * (150 / GetFullChargeTime()) + 100);
		if ( pitch > 250 )
			pitch = 250;

		// ALERT( at_console, "%d %d %d\n", m_fInAttack, m_iSoundState, pitch );

		if ( m_iSoundState == 0 )
			ALERT(at_console, "sound state %d\n", m_iSoundState);

		using namespace EventConstructor;
		CEventConstructor event;

		event << Flags(FEV_NOTHOST) << Invoker(m_pPlayer->edict()) << EventIndex(m_usGaussSpin) << IntParam1(pitch)
			  << BoolParam1(m_iSoundState == SND_CHANGE_PITCH);

		event.Send();

		m_iSoundState = SND_CHANGE_PITCH;  // hack for going through level transitions

		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;

		// m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1;
		if ( m_pPlayer->m_flStartCharge < gpGlobals->time - 10 )
		{
			// Player charged up too long. Zap him.
			EMIT_SOUND_DYN(
				ENT(m_pPlayer->pev),
				CHAN_WEAPON,
				"weapons/electro4.wav",
				1.0,
				ATTN_NORM,
				0,
				80 + RANDOM_LONG(0, 0x3f));
			EMIT_SOUND_DYN(
				ENT(m_pPlayer->pev),
				CHAN_ITEM,
				"weapons/electro6.wav",
				1.0,
				ATTN_NORM,
				0,
				75 + RANDOM_LONG(0, 0x3f));

			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0f;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0f;
#ifndef CLIENT_DLL
			m_pPlayer->TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), 50, DMG_SHOCK);
			UTIL_ScreenFade(m_pPlayer, Vector(255, 128, 0), 2, 0.5, 128, FFADE_IN);
#endif
			SendWeaponAnim(GAUSS_IDLE);

			// Player may have been killed and this weapon dropped, don't execute any more code after this!
			return;
		}
	}
}

//=========================================================
// StartFire- since all of this code has to run and then
// call Fire(), it was easier at this point to rip it out
// of weaponidle() and make its own function then to try to
// merge this into Fire(), which has some identical variable names
//=========================================================
void CGauss::StartFire(void)
{
	float flDamage;

	UTIL_MakeVectors(Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle));
	Vector vecAiming = gpGlobals->v_forward;
	Vector vecSrc = m_pPlayer->GetGunPosition();  // + gpGlobals->v_up * -8 + gpGlobals->v_right * 8;

	if ( gpGlobals->time - m_pPlayer->m_flStartCharge > GetFullChargeTime() )
	{
		flDamage = 200;
	}
	else
	{
		flDamage = 200 * ((gpGlobals->time - m_pPlayer->m_flStartCharge) / GetFullChargeTime());
	}

	if ( m_fPrimaryFire )
	{
		// fixed damage on primary attack
#ifdef CLIENT_DLL
		flDamage = 20;
#else
		flDamage = gSkillData.plrDmgGauss;
#endif
	}

	if ( m_fInAttack != 3 )
	{
		// ALERT( at_console, "Time:%f Damage:%f\n", gpGlobals->time - m_pPlayer->m_flStartCharge, flDamage );
#ifndef CLIENT_DLL
		float flZVel = m_pPlayer->pev->velocity[VEC3_Z];

		if ( !m_fPrimaryFire )
		{
			(Vector(m_pPlayer->pev->velocity) - Vector(gpGlobals->v_forward) * flDamage * 5)
				.CopyToArray(m_pPlayer->pev->velocity);
		}

		if ( !g_pGameRules->IsMultiplayer() )
		{
			// in deathmatch, gauss can pop you up into the air. Not in single play.
			m_pPlayer->pev->velocity[VEC3_Z] = flZVel;
		}
#endif
		// player "shoot" animation
		m_pPlayer->SetAnimation(PLAYER_ATTACK1);
	}

	// time until aftershock 'static discharge' sound
	m_pPlayer->m_flPlayAftershock = gpGlobals->time + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.3f, 0.8f);

	Fire(vecSrc, vecAiming, flDamage);
}

void CGauss::Fire(Vector vecOrigSrc, Vector vecDir, float flDamage)
{
	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	TraceResult tr;
	TraceResult beam_tr;

	(void)tr;
	(void)beam_tr;

#ifndef CLIENT_DLL
	Vector vecSrc = vecOrigSrc;
	Vector vecDest = vecSrc + vecDir * 8192;
	edict_t* pentIgnore;
	float flMaxFrac = 1.0;
	int nTotal = 0;
	int fHasPunched = 0;
	int fFirstBeam = 1;
	int nMaxHits = 10;

	pentIgnore = ENT(m_pPlayer->pev);
#else
	(void)vecOrigSrc;
	(void)vecDir;

	if ( m_fPrimaryFire == false )
	{
		g_irunninggausspred = true;
	}
#endif
	// The main firing event is sent unreliably so it won't be delayed.

	using namespace EventConstructor;

	{
		CEventConstructor event;

		event << Flags(FEV_NOTHOST) << Invoker(m_pPlayer->edict()) << EventIndex(m_usGaussFire)
			  << Origin(m_pPlayer->pev->origin) << Angles(m_pPlayer->pev->angles) << FloatParam1(flDamage)
			  << BoolParam1(m_fPrimaryFire);

		event.Send();
	}

	// This reliable event is used to stop the spinning sound
	// It's delayed by a fraction of second to make sure it is delayed by 1 frame on the client
	// It's sent reliably anyway, which could lead to other delays

	{
		CEventConstructor event;

		event << Flags(FEV_NOTHOST | FEV_RELIABLE) << Invoker(m_pPlayer->edict()) << EventIndex(m_usGaussFire)
			  << Origin(m_pPlayer->pev->origin) << Angles(m_pPlayer->pev->angles) << BoolParam2(true);

		event.Send();
	}

	/*ALERT( at_console, "%f %f %f\n%f %f %f\n",
		vecSrc.x, vecSrc.y, vecSrc.z,
		vecDest.x, vecDest.y, vecDest.z );*/

	// ALERT( at_console, "%f %f\n", tr.flFraction, flMaxFrac );

#ifndef CLIENT_DLL
	while ( flDamage > 10 && nMaxHits > 0 )
	{
		nMaxHits--;

		// ALERT( at_console, "." );
		UTIL_TraceLine(vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr);

		if ( tr.fAllSolid )
			break;

		CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);

		if ( pEntity == NULL )
			break;

		if ( fFirstBeam )
		{
			m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
			fFirstBeam = 0;

			nTotal += 26;
		}

		if ( pEntity->pev->takedamage )
		{
			ClearMultiDamage();
			pEntity->TraceAttack(m_pPlayer->pev, flDamage, vecDir, &tr, DMG_BULLET);
			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);
		}

		if ( pEntity->ReflectGauss() )
		{
			float n;

			pentIgnore = NULL;

			n = -DotProduct(tr.vecPlaneNormal, vecDir);

			if ( n < 0.5 )  // 60 degrees
			{
				// ALERT( at_console, "reflect %f\n", n );
				// reflect
				Vector r;

				r = 2.0 * Vector(tr.vecPlaneNormal) * n + vecDir;
				flMaxFrac = flMaxFrac - tr.flFraction;
				vecDir = r;
				vecSrc = Vector(tr.vecEndPos) + vecDir * 8;
				vecDest = vecSrc + vecDir * 8192;

				// explode a bit
				m_pPlayer->RadiusDamage(tr.vecEndPos, pev, m_pPlayer->pev, flDamage * n, CLASS_NONE, DMG_BLAST);

				nTotal += 34;

				// lose energy
				if ( n == 0 )
					n = 0.1f;
				flDamage = flDamage * (1 - n);
			}
			else
			{
				nTotal += 13;

				// limit it to one hole punch
				if ( fHasPunched )
					break;
				fHasPunched = 1;

				// try punching through wall if secondary attack (primary is incapable of breaking through)
				if ( !m_fPrimaryFire )
				{
					UTIL_TraceLine(
						Vector(tr.vecEndPos) + vecDir * 8,
						vecDest,
						dont_ignore_monsters,
						pentIgnore,
						&beam_tr);

					if ( !beam_tr.fAllSolid )
					{
						// trace backwards to find exit point
						UTIL_TraceLine(beam_tr.vecEndPos, tr.vecEndPos, dont_ignore_monsters, pentIgnore, &beam_tr);

						n = (Vector(beam_tr.vecEndPos) - Vector(tr.vecEndPos)).Length();

						if ( n < flDamage )
						{
							if ( n == 0 )
								n = 1;
							flDamage -= n;

							// ALERT( at_console, "punch %f\n", n );
							nTotal += 21;

							// exit blast damage
							// m_pPlayer->RadiusDamage( beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage,
							// CLASS_NONE, DMG_BLAST );
							float damage_radius;

							if ( g_pGameRules->IsMultiplayer() )
							{
								damage_radius = flDamage * 1.75f;  // Old code == 2.5
							}
							else
							{
								damage_radius = flDamage * 2.5f;
							}

							CRadialDamageInflictor damage;

							damage.SetOrigin(Vector(beam_tr.vecEndPos) + vecDir * 8);
							damage.SetInflictor(pev);
							damage.SetAttacker(m_pPlayer->pev);
							damage.SetDamage(flDamage);
							damage.SetRadius(damage_radius);
							damage.SetDamageTypeBits(DMG_BLAST);

							damage.ApplyDamage();

							CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3.0);

							nTotal += 53;

							vecSrc = Vector(beam_tr.vecEndPos) + vecDir;
						}
						else if ( !selfgauss.value )
						{
							flDamage = 0;
						}
					}
					else
					{
						// ALERT( at_console, "blocked %f\n", n );
						flDamage = 0;
					}
				}
				else
				{
					// ALERT( at_console, "blocked solid\n" );

					flDamage = 0;
				}
			}
		}
		else
		{
			vecSrc = Vector(tr.vecEndPos) + vecDir;
			pentIgnore = ENT(pEntity->pev);
		}
	}
#endif
	// ALERT( at_console, "%d bytes\n", nTotal );
}

void CGauss::WeaponIdle(void)
{
	ResetEmptySound();

	// play aftershock static discharge
	if ( m_pPlayer->m_flPlayAftershock && m_pPlayer->m_flPlayAftershock < gpGlobals->time )
	{
		switch ( RANDOM_LONG(0, 3) )
		{
			case 0:
				EMIT_SOUND(
					ENT(m_pPlayer->pev),
					CHAN_WEAPON,
					"weapons/electro4.wav",
					RANDOM_FLOAT(0.7f, 0.8f),
					ATTN_NORM);
				break;
			case 1:
				EMIT_SOUND(
					ENT(m_pPlayer->pev),
					CHAN_WEAPON,
					"weapons/electro5.wav",
					RANDOM_FLOAT(0.7f, 0.8f),
					ATTN_NORM);
				break;
			case 2:
				EMIT_SOUND(
					ENT(m_pPlayer->pev),
					CHAN_WEAPON,
					"weapons/electro6.wav",
					RANDOM_FLOAT(0.7f, 0.8f),
					ATTN_NORM);
				break;
			case 3:
				break;  // no sound
		}
		m_pPlayer->m_flPlayAftershock = 0.0;
	}

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if ( m_fInAttack != 0 )
	{
		StartFire();
		m_fInAttack = 0;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0f;

		// Need to set m_flNextPrimaryAttack so the weapon gets a chance to complete its secondary fire animation. -
		// Solokiller
		if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5f;
	}
	else
	{
#ifndef CLIENT_DLL
		int iAnim;
#endif
		float flRand = RANDOM_FLOAT(0, 1);

		if ( flRand <= 0.5 )
		{
#ifndef CLIENT_DLL
			iAnim = GAUSS_IDLE;
#endif
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		}
		else if ( flRand <= 0.75 )
		{
#ifndef CLIENT_DLL
			iAnim = GAUSS_IDLE2;
#endif
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		}
		else
		{
#ifndef CLIENT_DLL
			iAnim = GAUSS_FIDGET;
#endif
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
		}

#ifndef CLIENT_DLL
		SendWeaponAnim(iAnim);
#endif
	}
}

bool CGauss::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CBasePlayerWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	m_fInAttack = from->iuser1;
	return true;
}

bool CGauss::WritePredictionData(weapon_data_t* to)
{
	if ( !CBasePlayerWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->iuser1 = m_fInAttack;
	return true;
}

class CGaussAmmo : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_gaussammo.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_gaussammo.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		if ( pOther->GiveAmmo(AMMO_URANIUMBOX_GIVE, AmmoDef_Uranium.AmmoName, AmmoDef_Uranium.MaxCarry) != -1 )
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
};

LINK_ENTITY_TO_CLASS(ammo_gaussclip, CGaussAmmo)
#endif
