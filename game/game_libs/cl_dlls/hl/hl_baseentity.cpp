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
==========================
This file contains "stubs" of class member implementations so that we can predict certain
 weapons client side.  From time to time you might find that you need to implement part of the
 these functions.  If so, cut it from here, paste it in hl_weapons.cpp or somewhere else and
 add in the functionality you need.
==========================
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "nodes.h"
#include "soundent.h"
#include "skill.h"
#include "cl_dll.h"

// Globals used by game logic
const Vector g_vecZero = Vector(0, 0, 0);
int gmsgWeapPickup = 0;
enginefuncs_t g_engfuncs;
globalvars_t* gpGlobals;

ItemInfo CBasePlayerItem::ItemInfoArray[MAX_WEAPONS];

void EMIT_SOUND_DYN(edict_t*, int, const char*, float, float, int, int)
{
}

// CBaseEntity Stubs
int CBaseEntity::TakeHealth(float, int)
{
	return 1;
}
int CBaseEntity::TakeDamage(entvars_t*, entvars_t*, float, int)
{
	return 1;
}
CBaseEntity* CBaseEntity::GetNextTarget(void)
{
	return NULL;
}
int CBaseEntity::Save(CSave&)
{
	return 1;
}
int CBaseEntity::Restore(CRestore&)
{
	return 1;
}
void CBaseEntity::SetObjectCollisionBox(void)
{
}
int CBaseEntity::Intersects(CBaseEntity*)
{
	return 0;
}
void CBaseEntity::MakeDormant(void)
{
}
int CBaseEntity::IsDormant(void)
{
	return 0;
}
BOOL CBaseEntity::IsInWorld(void)
{
	return TRUE;
}
int CBaseEntity::ShouldToggle(USE_TYPE, BOOL)
{
	return 0;
}
int CBaseEntity::DamageDecal(int)
{
	return -1;
}
CBaseEntity* CBaseEntity::Create(const char*, const Vector&, const Vector&, edict_t*)
{
	return NULL;
}
void CBaseEntity::UpdateOnRemove(void)
{
}
void CBaseEntity::SUB_Remove(void)
{
}

// CBaseDelay Stubs
void CBaseDelay::KeyValue(struct KeyValueData_s*)
{
}
int CBaseDelay::Restore(class CRestore&)
{
	return 1;
}
int CBaseDelay::Save(class CSave&)
{
	return 1;
}

// CBaseAnimating Stubs
int CBaseAnimating::Restore(class CRestore&)
{
	return 1;
}
int CBaseAnimating::Save(class CSave&)
{
	return 1;
}

// DEBUG Stubs
edict_t* DBG_EntOfVars(const entvars_t*)
{
	return NULL;
}
void DBG_AssertFunction(
	bool fExpr,
	const char* szExpr,
	const char* szFile,
	int szLine,
	const char* szMessage,
	bool showAlert
)
{
	if ( fExpr )
	{
		return;
	}

	if ( showAlert )
	{
		char szOut[512];
		if ( szMessage != NULL )
		{
			PlatformLib_SNPrintF(
				szOut,
				sizeof(szOut),
				"ASSERT FAILED:\n %s \n(%s@%d)\n%s",
				szExpr,
				szFile,
				szLine,
				szMessage
			);
		}
		else
		{
			PlatformLib_SNPrintF(szOut, sizeof(szOut), "ASSERT FAILED:\n %s \n(%s@%d)", szExpr, szFile, szLine);
		}

		gEngfuncs.Con_Printf("%s", szOut);
		assert(false);
	}
	else
	{
		// Engine not set up yet - just fail a standard assertion so that we can see.
		assert(false);
	}
}

// UTIL_* Stubs
void UTIL_PrecacheOther(const char*)
{
}
void UTIL_BloodDrips(const Vector&, const Vector&, int, int)
{
}
void UTIL_DecalTrace(TraceResult*, int)
{
}
void UTIL_GunshotDecalTrace(TraceResult*, int)
{
}
void UTIL_MakeVectors(const Vector&)
{
}
BOOL UTIL_IsValidEntity(edict_t*)
{
	return TRUE;
}
void UTIL_SetOrigin(entvars_t*, const Vector&)
{
}
BOOL UTIL_GetNextBestWeapon(CBasePlayer*, CBasePlayerItem*)
{
	return TRUE;
}
void UTIL_LogPrintf(char*, ...)
{
}
void UTIL_ClientPrintAll(int, char const*, char const*, char const*, char const*, char const*)
{
}
void ClientPrint(entvars_t*, int, const char*, const char*, const char*, const char*, const char*)
{
}

void ClientPrintMsg(entvars_t*, int, const char*, ...)
{
}

// CBaseToggle Stubs
int CBaseToggle::Restore(class CRestore&)
{
	return 1;
}
int CBaseToggle::Save(class CSave&)
{
	return 1;
}
void CBaseToggle::KeyValue(struct KeyValueData_s*)
{
}

// CGrenade Stubs
CGrenade::CGrenade() :
	CBaseMonster(),
	m_iExplodeSpriteScale(0)
{
}
void CGrenade::BounceSound(void)
{
}
void CGrenade::Explode(Vector, Vector)
{
}
void CGrenade::Explode(TraceResult*, int)
{
}
void CGrenade::Killed(entvars_t*, int)
{
}
void CGrenade::Spawn(void)
{
}
CGrenade* CGrenade::ShootTimed(entvars_t*, Vector, Vector, float)
{
	return 0;
}
CGrenade* CGrenade::ShootContact(entvars_t*, Vector, Vector)
{
	return 0;
}
void CGrenade::DetonateUse(CBaseEntity*, CBaseEntity*, USE_TYPE, float)
{
}

void UTIL_Remove(CBaseEntity*)
{
}
struct skilldata_t gSkillData;
void UTIL_SetSize(entvars_t*, const Vector&, const Vector&)
{
}
CBaseEntity* UTIL_FindEntityInSphere(CBaseEntity*, const Vector&, float)
{
	return 0;
}

Vector UTIL_VecToAngles(const Vector&)
{
	return Vector();
}
CSprite* CSprite::SpriteCreate(const char*, const Vector&, BOOL)
{
	return 0;
}
void CBeam::PointEntInit(const Vector&, int)
{
}
CBeam* CBeam::BeamCreate(const char*, int)
{
	return NULL;
}
void CSprite::Expand(float, float)
{
}

CBaseEntity* CBaseMonster::CheckTraceHullAttack(float, int, int)
{
	return NULL;
}
void CBaseMonster::Eat(float)
{
}
BOOL CBaseMonster::FShouldEat(void)
{
	return TRUE;
}
void CBaseMonster::BarnacleVictimBitten(entvars_t*)
{
}
void CBaseMonster::BarnacleVictimReleased(void)
{
}
void CBaseMonster::Listen(void)
{
}
float CBaseMonster::FLSoundVolume(CSound*)
{
	return 0.0;
}
BOOL CBaseMonster::FValidateHintType(short)
{
	return FALSE;
}
void CBaseMonster::Look(int)
{
}
int CBaseMonster::ISoundMask(void)
{
	return 0;
}
CSound* CBaseMonster::PBestSound(void)
{
	return NULL;
}
CSound* CBaseMonster::PBestScent(void)
{
	return NULL;
}
float CBaseAnimating::StudioFrameAdvance(float)
{
	return 0.0;
}
void CBaseMonster::MonsterThink(void)
{
}
void CBaseMonster::MonsterUse(CBaseEntity*, CBaseEntity*, USE_TYPE, float)
{
}
int CBaseMonster::IgnoreConditions(void)
{
	return 0;
}
void CBaseMonster::RouteClear(void)
{
}
void CBaseMonster::RouteNew(void)
{
}
BOOL CBaseMonster::FRouteClear(void)
{
	return FALSE;
}
BOOL CBaseMonster::FRefreshRoute(void)
{
	return 0;
}
BOOL CBaseMonster::MoveToEnemy(Activity, float)
{
	return FALSE;
}
BOOL CBaseMonster::MoveToLocation(Activity, float, const Vector&)
{
	return FALSE;
}
BOOL CBaseMonster::MoveToTarget(Activity, float)
{
	return FALSE;
}
BOOL CBaseMonster::MoveToNode(Activity, float, const Vector&)
{
	return FALSE;
}
int ShouldSimplify(int)
{
	return TRUE;
}
void CBaseMonster::RouteSimplify(CBaseEntity*)
{
}
BOOL CBaseMonster::FBecomeProne(void)
{
	return TRUE;
}
BOOL CBaseMonster::CheckRangeAttack1(float, float)
{
	return FALSE;
}
BOOL CBaseMonster::CheckRangeAttack2(float, float)
{
	return FALSE;
}
BOOL CBaseMonster::CheckMeleeAttack1(float, float)
{
	return FALSE;
}
BOOL CBaseMonster::CheckMeleeAttack2(float, float)
{
	return FALSE;
}
void CBaseMonster::CheckAttacks(CBaseEntity*, float)
{
}
BOOL CBaseMonster::FCanCheckAttacks(void)
{
	return FALSE;
}
int CBaseMonster::CheckEnemy(CBaseEntity*)
{
	return 0;
}
void CBaseMonster::PushEnemy(CBaseEntity*, Vector&)
{
}
BOOL CBaseMonster::PopEnemy()
{
	return FALSE;
}
void CBaseMonster::SetActivity(Activity)
{
}
void CBaseMonster::SetSequenceByName(const char*)
{
}
int CBaseMonster::CheckLocalMove(const Vector&, const Vector&, CBaseEntity*, float*)
{
	return 0;
}
float CBaseMonster::OpenDoorAndWait(entvars_t*)
{
	return 0.0;
}
void CBaseMonster::AdvanceRoute(float)
{
}
int CBaseMonster::RouteClassify(int)
{
	return 0;
}
BOOL CBaseMonster::BuildRoute(const Vector&, int, CBaseEntity*)
{
	return FALSE;
}
void CBaseMonster::InsertWaypoint(Vector, int)
{
}
BOOL CBaseMonster::FTriangulate(const Vector&, const Vector&, float, CBaseEntity*, Vector*)
{
	return FALSE;
}
void CBaseMonster::Move(float)
{
}
BOOL CBaseMonster::ShouldAdvanceRoute(float)
{
	return FALSE;
}
void CBaseMonster::MoveExecute(CBaseEntity*, const Vector&, float)
{
}
void CBaseMonster::MonsterInit(void)
{
}
void CBaseMonster::MonsterInitThink(void)
{
}
void CBaseMonster::StartMonster(void)
{
}
void CBaseMonster::MovementComplete(void)
{
}
int CBaseMonster::TaskIsRunning(void)
{
	return 0;
}
int CBaseMonster::IRelationship(CBaseEntity*)
{
	return 0;
}
BOOL CBaseMonster::FindCover(Vector, Vector, float, float)
{
	return FALSE;
}
BOOL CBaseMonster::BuildNearestRoute(Vector, Vector, float, float)
{
	return FALSE;
}
CBaseEntity* CBaseMonster::BestVisibleEnemy(void)
{
	return NULL;
}
BOOL CBaseMonster::FInViewCone(CBaseEntity*)
{
	return FALSE;
}
BOOL CBaseMonster::FInViewCone(Vector*)
{
	return FALSE;
}
BOOL CBaseEntity::FVisible(CBaseEntity*)
{
	return FALSE;
}
BOOL CBaseEntity::FVisible(const Vector&)
{
	return FALSE;
}
void CBaseMonster::MakeIdealYaw(Vector)
{
}
float CBaseMonster::FlYawDiff(void)
{
	return 0.0;
}
float CBaseMonster::ChangeYaw(int)
{
	return 0;
}
float CBaseMonster::VecToYaw(Vector)
{
	return 0.0;
}
int CBaseAnimating::LookupActivity(int)
{
	return 0;
}
int CBaseAnimating::LookupActivityHeaviest(int)
{
	return 0;
}
void CBaseMonster::SetEyePosition(void)
{
}
int CBaseAnimating::LookupSequence(const char*)
{
	return 0;
}
void CBaseAnimating::ResetSequenceInfo()
{
}
BOOL CBaseAnimating::GetSequenceFlags()
{
	return FALSE;
}
void CBaseAnimating::DispatchAnimEvents(float)
{
}
void CBaseMonster::HandleAnimEvent(MonsterEvent_t*)
{
}
float CBaseAnimating::SetBoneController(int, float)
{
	return 0.0;
}
void CBaseAnimating::InitBoneControllers(void)
{
}
float CBaseAnimating::SetBlending(int, float)
{
	return 0;
}
void CBaseAnimating::GetBonePosition(int, Vector&, Vector&)
{
}
void CBaseAnimating::GetAttachment(int, Vector&, Vector&) const
{
}
int CBaseAnimating::FindTransition(int, int, int*)
{
	return -1;
}
void CBaseAnimating::GetAutomovement(Vector&, Vector&, float)
{
}
void CBaseAnimating::SetBodygroup(int, int)
{
}
int CBaseAnimating::GetBodygroup(int)
{
	return 0;
}
Vector CBaseMonster::GetGunPosition(void)
{
	return g_vecZero;
}
void CBaseEntity::TraceAttack(entvars_t*, float, Vector, const TraceResult*, int)
{
}
void CBaseEntity::FireBullets(ULONG, Vector, Vector, Vector, float, int, int, int, entvars_t*)
{
}
void CBaseEntity::TraceBleed(float, Vector, const TraceResult*, int)
{
}
void CBaseMonster::MakeDamageBloodDecal(int, float, TraceResult*, const Vector&)
{
}
BOOL CBaseMonster::FGetNodeRoute(Vector)
{
	return TRUE;
}
int CBaseMonster::FindHintNode(void)
{
	return NO_NODE;
}
void CBaseMonster::ReportAIState(void)
{
}
void CBaseMonster::KeyValue(KeyValueData*)
{
}
BOOL CBaseMonster::FCheckAITrigger(void)
{
	return FALSE;
}
int CBaseMonster::CanPlaySequence(BOOL, int)
{
	return FALSE;
}
BOOL CBaseMonster::FindLateralCover(const Vector&, const Vector&)
{
	return FALSE;
}
Vector CBaseMonster::ShootAtEnemy(const Vector&)
{
	return g_vecZero;
}
BOOL CBaseMonster::FacingIdeal(void)
{
	return FALSE;
}
BOOL CBaseMonster::FCanActiveIdle(void)
{
	return FALSE;
}
void CBaseMonster::PlaySentence(const char*, float, float, float)
{
}
void CBaseMonster::PlayScriptedSentence(const char*, float, float, float, BOOL, CBaseEntity*)
{
}
void CBaseMonster::SentenceStop(void)
{
}
void CBaseMonster::CorpseFallThink(void)
{
}
void CBaseMonster::MonsterInitDead(void)
{
}
BOOL CBaseMonster::BBoxFlat(void)
{
	return TRUE;
}
BOOL CBaseMonster::GetEnemy(void)
{
	return FALSE;
}
void CBaseMonster::TraceAttack(entvars_t*, float, Vector, const TraceResult*, int)
{
}
CBaseEntity* CBaseMonster::DropItem(const char*, const Vector&, const Vector&)
{
	return NULL;
}
BOOL CBaseMonster::ShouldFadeOnDeath(void)
{
	return FALSE;
}
void CBaseMonster::RadiusDamage(entvars_t*, entvars_t*, float, int, int)
{
}
void CBaseMonster::RadiusDamage(Vector, entvars_t*, entvars_t*, float, int, int)
{
}
void CBaseMonster::FadeMonster(void)
{
}
void CBaseMonster::GibMonster(void)
{
}
BOOL CBaseMonster::HasHumanGibs(void)
{
	return FALSE;
}
BOOL CBaseMonster::HasAlienGibs(void)
{
	return FALSE;
}
Activity CBaseMonster::GetDeathActivity(void)
{
	return ACT_DIE_HEADSHOT;
}
MONSTERSTATE CBaseMonster::GetIdealState(void)
{
	return MONSTERSTATE_ALERT;
}
Schedule_t* CBaseMonster::GetScheduleOfType(int)
{
	return NULL;
}
Schedule_t* CBaseMonster::GetSchedule(void)
{
	return NULL;
}
void CBaseMonster::RunTask(Task_t*)
{
}
void CBaseMonster::StartTask(Task_t*)
{
}
Schedule_t* CBaseMonster::ScheduleFromName(const char*)
{
	return NULL;
}
void CBaseMonster::BecomeDead(void)
{
}
void CBaseMonster::RunAI(void)
{
}
void CBaseMonster::Killed(entvars_t*, int)
{
}
int CBaseMonster::TakeHealth(float, int)
{
	return 0;
}
int CBaseMonster::TakeDamage(entvars_t*, entvars_t*, float, int)
{
	return 0;
}
int CBaseMonster::Restore(class CRestore&)
{
	return 1;
}
int CBaseMonster::Save(class CSave&)
{
	return 1;
}

int TrainSpeed(int, int)
{
	return 0;
}
void CBasePlayer::DeathSound(void)
{
}
int CBasePlayer::TakeHealth(float, int)
{
	return 0;
}
void CBasePlayer::TraceAttack(entvars_t*, float, Vector, const TraceResult*, int)
{
}
int CBasePlayer::TakeDamage(entvars_t*, entvars_t*, float, int)
{
	return 0;
}
void CBasePlayer::PackDeadPlayerItems(void)
{
}
void CBasePlayer::RemoveAllItems(BOOL)
{
}
void CBasePlayer::SetAnimation(PLAYER_ANIM)
{
}
void CBasePlayer::WaterMove()
{
}
BOOL CBasePlayer::IsOnLadder(void)
{
	return FALSE;
}
void CBasePlayer::PlayerDeathThink(void)
{
}
void CBasePlayer::StartDeathCam(void)
{
}
void CBasePlayer::StartObserver(Vector, Vector)
{
}
void CBasePlayer::PlayerUse(void)
{
}
void CBasePlayer::Jump()
{
}
void CBasePlayer::Duck()
{
}
int CBasePlayer::Classify(void)
{
	return 0;
}
void CBasePlayer::PreThink(void)
{
}
void CBasePlayer::CheckTimeBasedDamage()
{
}
void CBasePlayer::UpdateGeigerCounter(void)
{
}
void CBasePlayer::CheckSuitUpdate()
{
}
void CBasePlayer::SetSuitUpdate(const char*, int, int)
{
}
void CBasePlayer::UpdatePlayerSound(void)
{
}
void CBasePlayer::PostThink()
{
}
void CBasePlayer::Precache(void)
{
}
int CBasePlayer::Save(CSave&)
{
	return 0;
}
void CBasePlayer::RenewItems(void)
{
}
int CBasePlayer::Restore(CRestore&)
{
	return 0;
}
void CBasePlayer::SelectNextItem(int)
{
}
BOOL CBasePlayer::HasWeapons(void)
{
	return FALSE;
}
CBasePlayerItem* CBasePlayer::GetNamedItem(const char*)
{
	return nullptr;
}
void CBasePlayer::SelectPrevItem(int)
{
}
CBaseEntity* FindEntityForward(CBaseEntity*)
{
	return NULL;
}
BOOL CBasePlayer::FlashlightIsOn(void)
{
	return FALSE;
}
void CBasePlayer::FlashlightTurnOn(void)
{
}
void CBasePlayer::FlashlightTurnOff(void)
{
}
void CBasePlayer::ForceClientDllUpdate(void)
{
}
void CBasePlayer::ImpulseCommands()
{
}
void CBasePlayer::CheatImpulseCommands(int)
{
}
int CBasePlayer::AddPlayerItem(CBasePlayerItem*)
{
	return FALSE;
}
int CBasePlayer::RemovePlayerItem(CBasePlayerItem*, bool)
{
	return FALSE;
}
void CBasePlayer::ItemPreFrame()
{
}
void CBasePlayer::ItemPostFrame()
{
}
int CBasePlayer::AmmoInventory(int)
{
	return -1;
}
int CBasePlayer::GetAmmoIndex(const char*)
{
	return -1;
}
void CBasePlayer::SendAmmoUpdate(void)
{
}
void CBasePlayer::UpdateClientData(void)
{
}
BOOL CBasePlayer::FBecomeProne(void)
{
	return TRUE;
}
void CBasePlayer::BarnacleVictimBitten(entvars_t*)
{
}
void CBasePlayer::BarnacleVictimReleased(void)
{
}
int CBasePlayer::Illumination(void)
{
	return 0;
}
void CBasePlayer::EnableControl(BOOL)
{
}
Vector CBasePlayer::GetAutoaimVector(float)
{
	return g_vecZero;
}
Vector CBasePlayer::AutoaimDeflection(Vector&, float, float)
{
	return g_vecZero;
}
void CBasePlayer::ResetAutoaim()
{
}
void CBasePlayer::SetCustomDecalFrames(int)
{
}
int CBasePlayer::GetCustomDecalFrames(void)
{
	return -1;
}
void CBasePlayer::DropPlayerItem(const char*)
{
}
BOOL CBasePlayer::HasPlayerItem(CBasePlayerItem*)
{
	return FALSE;
}
BOOL CBasePlayer::SwitchWeapon(CBasePlayerItem*)
{
	return FALSE;
}
Vector CBasePlayer::GetGunPosition(void)
{
	return g_vecZero;
}
const char* CBasePlayer::TeamID(void)
{
	return "";
}
int CBasePlayer::GiveAmmo(int, const char*, int, CBasePlayerAmmo*)
{
	return 0;
}
void CBasePlayer::AddPoints(int, BOOL)
{
}
void CBasePlayer::AddPointsToTeam(int, BOOL)
{
}
void CBasePlayer::SetScreenOverlay(ScreenOverlays::OverlayId)
{
}

void CBasePlayer::SetFOV(float)
{
}

void CBasePlayer::ResetFOV()
{
}

void ClearMultiDamage(void)
{
}
void ApplyMultiDamage(entvars_t*, entvars_t*)
{
}
void AddMultiDamage(entvars_t*, CBaseEntity*, float, int)
{
}
void SpawnBlood(Vector, int, float)
{
}
int DamageDecal(CBaseEntity*, int)
{
	return 0;
}
void DecalGunshot(TraceResult*, int)
{
}
void EjectBrass(const Vector&, const Vector&, float, int, int)
{
}
void AddAmmoNameToAmmoRegistry(const char*)
{
}
int CBasePlayerItem::Restore(class CRestore&)
{
	return 1;
}
int CBasePlayerItem::Save(class CSave&)
{
	return 1;
}
int CBasePlayerWeapon::Restore(class CRestore&)
{
	return 1;
}
int CBasePlayerWeapon::Save(class CSave&)
{
	return 1;
}
float CBasePlayerWeapon::GetNextAttackDelay(float flTime)
{
	return flTime;
}
void CBasePlayerItem::SetObjectCollisionBox(void)
{
}
void CBasePlayerItem::FallInit(void)
{
}
void CBasePlayerItem::FallThink(void)
{
}
void CBasePlayerItem::Materialize(void)
{
}
void CBasePlayerItem::AttemptToMaterialize(void)
{
}
void CBasePlayerItem::CheckRespawn(void)
{
}
CBaseEntity* CBasePlayerItem::Respawn(void)
{
	return NULL;
}
void CBasePlayerItem::DefaultTouch(CBaseEntity*)
{
}
void CBasePlayerItem::DestroyItem(void)
{
}
int CBasePlayerItem::AddToPlayer(CBasePlayer*)
{
	return TRUE;
}
void CBasePlayerItem::Drop(void)
{
}
void CBasePlayerItem::Kill(void)
{
}
void CBasePlayerItem::Holster(int)
{
}
void CBasePlayerItem::AttachToPlayer(CBasePlayer*)
{
}
void CBasePlayerItem::DetachFromPlayer(CBasePlayer*)
{
}
int CBasePlayerWeapon::AddDuplicate(CBasePlayerItem*)
{
	return 0;
}
int CBasePlayerWeapon::AddToPlayer(CBasePlayer*)
{
	return FALSE;
}
int CBasePlayerWeapon::UpdateClientData(CBasePlayer*)
{
	return 0;
}
BOOL CBasePlayerWeapon::AddPrimaryAmmo(int, char*, int, int)
{
	return TRUE;
}
BOOL CBasePlayerWeapon::AddSecondaryAmmo(int, char*, int)
{
	return TRUE;
}
BOOL CBasePlayerWeapon::IsUseable(void)
{
	return TRUE;
}
int CBasePlayerWeapon::PrimaryAmmoIndex(void)
{
	return -1;
}
int CBasePlayerWeapon::SecondaryAmmoIndex(void)
{
	return -1;
}
void CBasePlayerAmmo::Spawn(void)
{
}
CBaseEntity* CBasePlayerAmmo::Respawn(void)
{
	return this;
}
void CBasePlayerAmmo::Materialize(void)
{
}
void CBasePlayerAmmo::DefaultTouch(CBaseEntity*)
{
}
int CBasePlayerWeapon::ExtractAmmo(CBasePlayerWeapon*)
{
	return 0;
}
int CBasePlayerWeapon::ExtractClipAmmo(CBasePlayerWeapon*)
{
	return 0;
}
void CBasePlayerWeapon::RetireWeapon(void)
{
}
const char* CBasePlayerItem::PickupSound() const
{
	return nullptr;
}
void CSoundEnt::InsertSound(int, const Vector&, int, float)
{
}
void RadiusDamage(Vector, entvars_t*, entvars_t*, float, float, int, int)
{
}
