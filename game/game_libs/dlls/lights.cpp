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
/*

===== lights.cpp ========================================================

  spawn and think functions for editor-placed lights

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "PlatformLib/String.h"

class CLight : public CPointEntity
{
public:
	virtual void KeyValue(KeyValueData* pkvd);
	virtual void Spawn(void);
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);

	static TYPEDESCRIPTION m_SaveData[];

private:
	int m_iStyle;
	string_t m_iszPattern;
};

LINK_ENTITY_TO_CLASS(light, CLight)

TYPEDESCRIPTION CLight::m_SaveData[] = {
	DEFINE_FIELD(CLight, m_iStyle, FIELD_INTEGER),
	DEFINE_FIELD(CLight, m_iszPattern, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CLight, CPointEntity)

//
// Cache user-entity-field values until spawn is called.
//
void CLight::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "style") )
	{
		m_iStyle = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "pitch") )
	{
		pev->angles[VEC3_X] = static_cast<float>(atof(pkvd->szValue));
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "pattern") )
	{
		m_iszPattern = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
	{
		CPointEntity::KeyValue(pkvd);
	}
}

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) LIGHT_START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/

void CLight::Spawn(void)
{
	if ( FStringNull(pev->targetname) )
	{
		// inert light
		REMOVE_ENTITY(ENT(pev));
		return;
	}

	if ( m_iStyle >= 32 )
	{
		// CHANGE_METHOD(ENT(pev), em_use, light_use);
		if ( FBitSet(pev->spawnflags, SF_LIGHT_START_OFF) )
			LIGHT_STYLE(m_iStyle, "a");
		else if ( m_iszPattern )
			LIGHT_STYLE(m_iStyle, STRING(m_iszPattern));
		else
			LIGHT_STYLE(m_iStyle, "m");
	}
}

void CLight::Use(CBaseEntity*, CBaseEntity*, USE_TYPE useType, float)
{
	if ( m_iStyle >= 32 )
	{
		if ( !ShouldToggle(useType, !FBitSet(pev->spawnflags, SF_LIGHT_START_OFF)) )
			return;

		if ( FBitSet(pev->spawnflags, SF_LIGHT_START_OFF) )
		{
			if ( m_iszPattern )
				LIGHT_STYLE(m_iStyle, STRING(m_iszPattern));
			else
				LIGHT_STYLE(m_iStyle, "m");
			ClearBits(pev->spawnflags, SF_LIGHT_START_OFF);
		}
		else
		{
			LIGHT_STYLE(m_iStyle, "a");
			SetBits(pev->spawnflags, SF_LIGHT_START_OFF);
		}
	}
}

//
// shut up spawn functions for new spotlights
//
LINK_ENTITY_TO_CLASS(light_spot, CLight)

class CEnvLight : public CLight
{
public:
	void KeyValue(KeyValueData* pkvd);
	void Spawn(void);
};

LINK_ENTITY_TO_CLASS(light_environment, CEnvLight)

void CEnvLight::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "_light") )
	{
		int r, g, b, v, j;
		char szColor[64];
		j = PlatformLib_SScanF(pkvd->szValue, "%d %d %d %d\n", &r, &g, &b, &v);
		if ( j == 1 )
		{
			g = b = r;
		}
		else if ( j == 4 )
		{
			float vf = v / 255.0f;
			r = static_cast<int>(r * vf);
			g = static_cast<int>(g * vf);
			b = static_cast<int>(b * vf);
		}

		// simulate qrad direct, ambient,and gamma adjustments, as well as engine scaling
		r = (int)(pow(r / 114.0, 0.6) * 264.0);
		g = (int)(pow(g / 114.0, 0.6) * 264.0);
		b = (int)(pow(b / 114.0, 0.6) * 264.0);

		pkvd->fHandled = TRUE;
		PlatformLib_SNPrintF(szColor, sizeof(szColor), "%d", r);
		CVAR_SET_STRING("sv_skycolor_r", szColor);
		PlatformLib_SNPrintF(szColor, sizeof(szColor), "%d", g);
		CVAR_SET_STRING("sv_skycolor_g", szColor);
		PlatformLib_SNPrintF(szColor, sizeof(szColor), "%d", b);
		CVAR_SET_STRING("sv_skycolor_b", szColor);
	}
	else
	{
		CLight::KeyValue(pkvd);
	}
}

void CEnvLight::Spawn(void)
{
	char szVector[64];
	UTIL_MakeAimVectors(pev->angles);

	PlatformLib_SNPrintF(szVector, sizeof(szVector), "%f", gpGlobals->v_forward[VEC3_X]);
	CVAR_SET_STRING("sv_skyvec_x", szVector);
	PlatformLib_SNPrintF(szVector, sizeof(szVector), "%f", gpGlobals->v_forward[VEC3_Y]);
	CVAR_SET_STRING("sv_skyvec_y", szVector);
	PlatformLib_SNPrintF(szVector, sizeof(szVector), "%f", gpGlobals->v_forward[VEC3_Z]);
	CVAR_SET_STRING("sv_skyvec_z", szVector);

	CLight::Spawn();
}
