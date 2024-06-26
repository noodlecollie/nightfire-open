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
//
//  hud_msg.cpp
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "EnginePublicAPI/r_efx.h"
#include "debug_assert.h"

#define MAX_CLIENTS 32

extern BEAM* pBeam;
extern BEAM* pBeam2;
extern TEMPENTITY* pFlare;  // Vit_amiN

extern float g_lastFOV;  // Vit_amiN

/// USER-DEFINED SERVER MESSAGE HANDLERS

int CHud::MsgFunc_ResetHUD(const char*, int iSize, void*)
{
	(void)iSize;
	ASSERT(iSize == 0);

	// clear all hud data
	HUDLIST* pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->Reset();
		pList = pList->pNext;
	}

	// reset sensitivity
	m_flMouseSensitivity = 0;

	// reset concussion effect
	m_iConcussionEffect = 0;

	// Vit_amiN: reset the FOV
	m_iFOV = 0;  // default_fov
	g_lastFOV = 0.0f;

	return 1;
}

void CAM_ToFirstPerson(void);

void CHud::MsgFunc_ViewMode(const char*, int, void*)
{
	CAM_ToFirstPerson();
}

void CHud::MsgFunc_InitHUD(const char*, int, void*)
{
	// prepare all hud data
	HUDLIST* pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->InitHUDData();
		pList = pList->pNext;
	}

	// Probably not a good place to put this.
	pBeam = pBeam2 = NULL;
	pFlare = NULL;  // Vit_amiN: clear egon's beam flare
}

int CHud::MsgFunc_GameMode(const char*, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	m_Teamplay = READ_BYTE();

	return 1;
}

int CHud::MsgFunc_Damage(const char*, int iSize, void* pbuf)
{
	int armor, blood;
	Vector from;
	int i;
	float count;

	BEGIN_READ(pbuf, iSize);
	armor = READ_BYTE();
	blood = READ_BYTE();

	for ( i = 0; i < 3; i++ )
		from[i] = READ_COORD();

	count = (blood * 0.5f) + (armor * 0.5f);

	if ( count < 10 )
		count = 10;

	// TODO: kick viewangles,  show damage visually
	return 1;
}

int CHud::MsgFunc_Concuss(const char*, int iSize, void* pbuf)
{
	int r, g, b;
	BEGIN_READ(pbuf, iSize);
	m_iConcussionEffect = READ_BYTE();
	if ( m_iConcussionEffect )
	{
		UnpackRGB(r, g, b, RGB_YELLOWISH);  // Vit_amiN: fixed
		this->m_StatusIcons.EnableIcon(
			"dmg_concuss",
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b));
	}
	else
		this->m_StatusIcons.DisableIcon("dmg_concuss");
	return 1;
}
