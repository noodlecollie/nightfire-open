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

#include <algorithm>
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

private:
	int m_skyColourValues[4] = {-1, -1, -1, -1};
	float m_lightingScaleValues[3] = {1.0f, 1.0f, 1.0f};
};

LINK_ENTITY_TO_CLASS(light_environment, CEnvLight)

void CEnvLight::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "_light") )
	{
		int numComponents = PlatformLib_SScanF(
			pkvd->szValue,
			"%d %d %d %d\n",
			&m_skyColourValues[0],
			&m_skyColourValues[1],
			&m_skyColourValues[2],
			&m_skyColourValues[3]);

		if ( numComponents == 1 )
		{
			m_skyColourValues[1] = m_skyColourValues[0];
			m_skyColourValues[2] = m_skyColourValues[0];
			m_skyColourValues[3] = 255;
		}
		else if ( numComponents != 4 )
		{
			// Garbage, reset.
			m_skyColourValues[0] = -1;
			m_skyColourValues[1] = -1;
			m_skyColourValues[2] = -1;
			m_skyColourValues[3] = -1;
		}

		if ( m_skyColourValues[3] >= 0 )
		{
			m_skyColourValues[0] = std::max<int>(0, std::min<int>(m_skyColourValues[0], 255));
			m_skyColourValues[1] = std::max<int>(0, std::min<int>(m_skyColourValues[1], 255));
			m_skyColourValues[2] = std::max<int>(0, std::min<int>(m_skyColourValues[2], 255));
			m_skyColourValues[3] = std::max<int>(0, std::min<int>(m_skyColourValues[3], 255));
		}

		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "_rad_lightscale") )
	{
		int numComponents = PlatformLib_SScanF(
			pkvd->szValue,
			"%f %f %f\n",
			&m_lightingScaleValues[0],
			&m_lightingScaleValues[1],
			&m_lightingScaleValues[2]);

		if ( numComponents == 1 )
		{
			m_lightingScaleValues[1] = m_lightingScaleValues[0];
			m_lightingScaleValues[2] = m_lightingScaleValues[0];
		}
		else if ( numComponents != 4 )
		{
			// Garbage, reset.
			m_lightingScaleValues[0] = 1.0f;
			m_lightingScaleValues[1] = 1.0f;
			m_lightingScaleValues[2] = 1.0f;
		}

		if ( numComponents == 1 || numComponents == 4 )
		{
			m_lightingScaleValues[0] = std::max<float>(0, m_lightingScaleValues[0]);
			m_lightingScaleValues[1] = std::max<float>(0, m_lightingScaleValues[1]);
			m_lightingScaleValues[2] = std::max<float>(0, m_lightingScaleValues[2]);
		}

		pkvd->fHandled = TRUE;
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

	if ( m_skyColourValues[3] >= 0 )
	{
		const float vf = static_cast<float>(m_skyColourValues[3]) / 255.0f;
		float redScaled = static_cast<float>(m_skyColourValues[0]) * vf;
		float greenScaled = static_cast<float>(m_skyColourValues[1]) * vf;
		float blueScaled = static_cast<float>(m_skyColourValues[2]) * vf;

		// simulate qrad direct, ambient,and gamma adjustments, as well as engine scaling
		float r = powf(redScaled / 114.0f, 0.6f) * 264.0f;
		float g = powf(greenScaled / 114.0f, 0.6f) * 264.0f;
		float b = powf(blueScaled / 114.0f, 0.6f) * 264.0f;

		r *= m_lightingScaleValues[0];
		g *= m_lightingScaleValues[1];
		b *= m_lightingScaleValues[2];

		char szColor[64];

		PlatformLib_SNPrintF(szColor, sizeof(szColor), "%d", static_cast<int>(r));
		CVAR_SET_STRING("sv_skycolor_r", szColor);

		PlatformLib_SNPrintF(szColor, sizeof(szColor), "%d", static_cast<int>(g));
		CVAR_SET_STRING("sv_skycolor_g", szColor);

		PlatformLib_SNPrintF(szColor, sizeof(szColor), "%d", static_cast<int>(b));
		CVAR_SET_STRING("sv_skycolor_b", szColor);
	}

	CLight::Spawn();
}
