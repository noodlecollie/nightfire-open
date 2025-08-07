
/**************************************************
 * Rho-Bot Source Code                            *
 * by Eric Bieschke (rhobot@botepidemic.com)      *
 * http://www.botepidemic.com/rhobot/             *
 **************************************************
 * You are free to do whatever you want with this *
 * source. All I ask is that (A) you tell me what *
 * you do with it so that I can check it out, (B) *
 * you credit me for any of my code that you use  *
 * in your project, and (C) if you are now, or    *
 * later become involved in the gaming industry   *
 * and think I would be a valuable contribution   *
 * to the team, contact me.   =]                  *
 **************************************************
 * If you have questions about the source, please *
 * don't hesitate to give me a ring. If you make  *
 * improvements to this source that you'd like to *
 * share with others, please let me know and I'll *
 * release your changes with the next source code *
 * release (you will be fully credited for all of *
 * your work). If I inadvertantly did not credit  *
 * either botman or Rich Whitehouse for code taken*
 * from their respective open-source bots, I      *
 * apologize (I did my best to comment what was   *
 * theirs), and if you let me know, I will credit *
 * them in the next source release.               *
 **************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "bot.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////

CBaseBotMemory::CBaseBotMemory() :
	EnemyFirstSeenTime(gpGlobals->time),
	bEnemyInSight(FALSE),
	EnemyLastSeenPosition(Vector(0, 0, 0)),
	EnemyLastSeenTime(gpGlobals->time),
	Prev1EnemyVelocity(Vector(0, 0, 0)),
	Prev1LookAtVec(Vector(0, 0, 0)),
	Prev1Velocity(Vector(0, 0, 0)),
	Prev2EnemyVelocity(Vector(0, 0, 0)),
	Prev2LookAtVec(Vector(0, 0, 0)),
	Prev2Velocity(Vector(0, 0, 0)),
	Prev3EnemyVelocity(Vector(0, 0, 0)),
	Prev3LookAtVec(Vector(0, 0, 0)),
	Prev3Velocity(Vector(0, 0, 0)),
	Prev4EnemyVelocity(Vector(0, 0, 0)),
	Prev4LookAtVec(Vector(0, 0, 0)),
	Prev4Velocity(Vector(0, 0, 0)),
	PrevOrigin(Vector(0, 0, 0))
{
}

CBaseBotMemory::~CBaseBotMemory()
{
}

///////////////////////////////////////////////////////////////////////////////
// EnemyInSight
///////////////////////////////////////////////////////////////////////////////

void CBaseBotMemory::EnemyInSight(CBaseEntity* pEnemy)
{
	SetEnemyInSight(TRUE);
	SetEnemyLastSeenTime(gpGlobals->time);
	SetEnemyLastSeenPosition(pEnemy->pev->origin);
}

///////////////////////////////////////////////////////////////////////////////
// EnemyOutOfSight
///////////////////////////////////////////////////////////////////////////////

void CBaseBotMemory::EnemyOutOfSight(void)
{
	SetEnemyInSight(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// EnemySighted
///////////////////////////////////////////////////////////////////////////////

void CBaseBotMemory::EnemySighted(CBaseEntity* pEnemy)
{
	EnemyInSight(pEnemy);
	SetEnemyFirstSeenTime(gpGlobals->time);
	SetPrev1EnemyVelocity(pEnemy->pev->velocity);
	SetPrev2EnemyVelocity(GetPrev1EnemyVelocity());
	SetPrev3EnemyVelocity(GetPrev2EnemyVelocity());
	SetPrev4EnemyVelocity(GetPrev3EnemyVelocity());
}

///////////////////////////////////////////////////////////////////////////////
// EndFrameUpdate
///////////////////////////////////////////////////////////////////////////////

void CBaseBotMemory::EndFrameUpdate(CBasePlayer* pOwner)
{
	CBaseBot* pBotOwner = (CBaseBot*)pOwner;

	SetPrevOrigin(pBotOwner->pev->origin);

	SetPrev4LookAtVec(GetPrev3LookAtVec());
	SetPrev3LookAtVec(GetPrev2LookAtVec());
	SetPrev2LookAtVec(GetPrev1LookAtVec());
	SetPrev1LookAtVec(pBotOwner->GetLookAtVec());

	SetPrev4Velocity(GetPrev3Velocity());
	SetPrev3Velocity(GetPrev2Velocity());
	SetPrev2Velocity(GetPrev1Velocity());
	SetPrev1Velocity(pBotOwner->pev->velocity);

	if ( pBotOwner->GetEnemy() != NULL )
	{
		SetPrev4EnemyVelocity(GetPrev3EnemyVelocity());
		SetPrev3EnemyVelocity(GetPrev2EnemyVelocity());
		SetPrev2EnemyVelocity(GetPrev1EnemyVelocity());
		SetPrev1EnemyVelocity(pBotOwner->GetEnemy()->pev->velocity);
	}
}
