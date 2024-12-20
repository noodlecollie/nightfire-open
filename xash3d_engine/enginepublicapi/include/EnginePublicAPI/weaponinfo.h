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

#pragma once

// Info about weapons player might have in his/her possession
typedef struct weapon_data_s
{
	int m_iId;
	int m_iClip;

	float m_flNextPrimaryAttack;
	float m_flNextSecondaryAttack;
	float m_flTimeWeaponIdle;
	float m_flEnqueuedMechanicInvocationTime;
	float m_flLastPrimaryAttack;
	float m_flLastSecondaryAttack;

	int m_fInReload;
	int m_fInSpecialReload;
	float m_flNextReload;
	float m_fReloadTime;

	float m_fAimedDamage;
	float m_fNextAimBonus;
	int m_fInZoom;
	int m_iWeaponState;
	float m_flInaccuracy;
	int m_PrimaryAttackMechanicIndex;
	int m_SecondaryAttackMechanicIndex;
	int m_EnqueuedMechanicIndex;
	int m_EnqueuedAttackMode;
	int m_NextEnqueuedAttackStep;

	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	float tuser1;
	float tuser2;
	float tuser3;
	float tuser4;
} weapon_data_t;
