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
//=========================================================
// skill.h - skill level concerns
//=========================================================
#pragma once
#ifndef SKILL_H
#define SKILL_H

struct skilldata_t
{
	int iSkillLevel;  // game skill level

	float turretHealth;
	float miniturretHealth;
	float sentryHealth;

	// health/suit charge
	float suitchargerCapacity;
	float batteryCapacity;
	float healthchargerCapacity;
	float healthkitCapacity;
	float scientistHeal;

	// monster damage adj
	float monHead;
	float monChest;
	float monStomach;
	float monLeg;
	float monArm;

	// player damage adj
	float plrHead;
	float plrChest;
	float plrStomach;
	float plrLeg;
	float plrArm;

	// Nightfire weapon damage
	float plrDmgP99;
	float plrDmgMP9;
	float plrDmgFrinesiAuto;
	float plrDmgFrinesiPump;
	float plrDmgRaptor;
	float plrDmgFists;
	float plrDmgFistsAlt;
	float plrDmgL96A1;
	float plrDmgRoninBullet;
	float plrDmgRoninExplosion;
	float roninBaseHealth;

	float plrDmgGrenadeLauncher;
	float plrSelfDmgMultGrenadeLauncher;
	float plrDmgMultGrenadelauncherHit;

	float plrDmgFragGrenade;
	float plrSelfDmgMultFragGrenade;
};

extern DLL_GLOBAL skilldata_t gSkillData;
float GetSkillCvar(const char* pName);

extern DLL_GLOBAL int g_iSkillLevel;

#define SKILL_EASY 1
#define SKILL_MEDIUM 2
#define SKILL_HARD 3
#define TOTAL_SKILL_LEVELS 3
#endif  // SKILL_H
