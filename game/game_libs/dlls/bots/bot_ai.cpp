
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
#include "client.h"
#include "gamerules.h"
#include "effects.h"
#include "items.h"
#include "soundent.h"
#include "weapons.h"
#include "nodes.h"
#include "bot.h"
#include "botweaponattributes.h"
#include "genericweapon.h"
#include "PlatformLib/String.h"

extern DLL_GLOBAL BOOL g_fGameOver;

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////

CBaseBot::CBaseBot() :
	bCalledAimThisFrame(FALSE),
	DesiredVelocity(Vector(0, 0, 0)),
	bGoUpOnLadder(TRUE),
	LookAtBiasVec(Vector(0, 0, 0)),
	LookAtVec(Vector(0, 0, 0)),
	fNextThink(gpGlobals->time),
	MovedDistance(0.0),
	MoveForward(0.0),
	MoveStrafe(0.0),
	MoveVertical(0.0),
	MSec(0.0),
	MSecCounter(0),
	// Vacindak:  Memory Leak Fix
	// pEnemy( NULL ),
	// pGoal( NULL ),
	// End Memory Leak Fix
	RoamDirection(UNKNOWN),
	TimeGoalCheck(gpGlobals->time),
	TimeGoalCheckDelay(0.3f),
	TimeMSecCheck(gpGlobals->time),
	TurningDirection(NONE),
	bWantToBeInCombat(FALSE),
	bFiredWeapon(false),
	FightStyle(),
	Memory(),
	Stats()
{
	pEnemy = (CBaseEntity*)NULL;
	pGoal = (CBaseEntity*)NULL;
	FightStyle.SetOwner(this);
}

CBaseBot::~CBaseBot()
{
}

///////////////////////////////////////////////////////////////////////////////
// ActionOpenFire
///////////////////////////////////////////////////////////////////////////////

// TODO: This is a bit ugly, could probably be refactored.
void CBaseBot::ActionOpenFire(void)
{
	bool releasedButtonsThisFrame = false;

	if ( !m_pActiveItem || !m_pActiveItem->CanDeploy() || FightStyle.GetNextEvaluationTime() <= gpGlobals->time )
	{
		ActionChooseWeapon();
		bFiredWeapon = false;
		Input.ReleaseButton(IN_ATTACK | IN_ATTACK2);
		releasedButtonsThisFrame = true;
	}

	if ( FightStyle.GetNextShootTime() > gpGlobals->time || bFiredWeapon )
	{
		return;
	}

	CBasePlayerWeapon* pActiveWeapon = (CBasePlayerWeapon*)m_pActiveItem;

	if ( pActiveWeapon->m_iClip <= 0 && pActiveWeapon->iMaxClip() != WEAPON_NOCLIP )
	{
		ActionReload();
		return;
	}

	if ( FightStyle.GetNextShootTime() <= gpGlobals->time && !pActiveWeapon->m_fInReload && !releasedButtonsThisFrame )
	{
		const int lastButtons = Input.GetButtons();

		if ( FightStyle.GetSecondaryFire() )
		{
			if ( m_rgAmmo[pActiveWeapon->m_iSecondaryAmmoType] > 0 && !(lastButtons & IN_ATTACK2) )
			{
				if ( FightStyle.GetHoldDownAttack() )
				{
					Input.HoldButton(IN_ATTACK2);
				}
				else
				{
					Input.PressButton(IN_ATTACK2);
				}

				bFiredWeapon = true;
			}
		}
		else
		{
			if ( (pActiveWeapon->iMaxClip() == WEAPON_NOCLIP || pActiveWeapon->m_iClip > 0) &&
				 !(lastButtons & IN_ATTACK) )
			{
				if ( FightStyle.GetHoldDownAttack() )
				{
					Input.HoldButton(IN_ATTACK);
				}
				else
				{
					Input.PressButton(IN_ATTACK);
				}

				bFiredWeapon = true;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// ActionChooseGoal - sets pGoal by searching through m_pLink's list
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ActionChooseGoal(void)
{
	float BestPickupDesire = 0;
	float DistanceToEntity;
	float PickupDesire;  // [0,100]
	CBaseEntity* pNextEnt = m_pLink;

	while ( pNextEnt != NULL )  // pNextEnt is already NULL if we don't want a new pGoal this frame
	{
		PickupDesire = 0.0;

		if ( !CheckHasPowerfulWeapon() )
		{
			if ( !(pNextEnt->pev->effects & EF_NODRAW) && CheckVisible(pNextEnt) )
			{
				PickupDesire = 100;
			}
		}
		else if ( !(pNextEnt->pev->effects & EF_NODRAW) && CheckVisible(pNextEnt) )
		{
			// TODO: This is a bit horrible. We really need something to be set on CBaseEntity that can
			// help us identify the type of the item, eg. an interface we can query.

			CGenericWeapon* genericWeapon = NULL;
			CGenericAmmo* genericAmmo = NULL;

			if ( (genericAmmo = dynamic_cast<CGenericAmmo*>(pNextEnt)) != NULL )
			{
				const int ammoIndex = GetAmmoIndex(genericAmmo->AmmoName());
				if ( ammoIndex >= 0 && ammoIndex < MAX_AMMO_SLOTS && m_rgAmmo[ammoIndex] < 100 )
				{
					PickupDesire = 100.0f - m_rgAmmo[ammoIndex];
				}
				else
				{
					PickupDesire = 0;
				}
			}
			else if ( (genericWeapon = dynamic_cast<CGenericWeapon*>(pNextEnt)) != NULL )
			{
				// TODO: This is very basic. Could we make it better?
				PickupDesire = 80;
			}
			else if ( FClassnameIs(pNextEnt->pev, "item_health") )
			{
				PickupDesire = 100.0f - pev->health;  // we want health proportional to how much we need
			}
			else if ( FClassnameIs(pNextEnt->pev, "item_battery") )
			{
				PickupDesire = MAX_NORMAL_BATTERY - pev->armorvalue;  // we want armour proportional to how much we need
			}
			else if ( strncmp("item_", STRING(pNextEnt->pev->classname), 5) == 0 )
			{
				PickupDesire = 80;
			}
		}

		if ( PickupDesire > 0 )  // desire this at all?
		{
			DistanceToEntity = (Vector(pNextEnt->pev->origin) - Vector(pev->origin)).Length();

			float diff = SEARCH_DISTANCE - DistanceToEntity;

			// Scott: fix.  Using diff * diff results in bots possibly wanting things
			//        that are far away.  now using diff*diff*diff
			PickupDesire *= (diff * diff * diff);  // want things more the closer they are

			if ( PickupDesire > BestPickupDesire )  // is this the most desirable item so far?
			{
				BestPickupDesire = PickupDesire;
				SetGoal(pNextEnt);
			}
		}

		pNextEnt = pNextEnt->m_pLink;
	}
}

///////////////////////////////////////////////////////////////////////////////
// ActionChooseWeapon
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ActionChooseWeapon(void)
{
	CGenericWeapon* pBestWeapon = NULL;
	int BestWeaponDesire = 0;  // no weapon lower than -1 can be autoswitched to

	if ( m_pActiveItem == NULL || !m_pActiveItem->CanHolster() )
	{
		return;
	}

	CBaseEntity* currentEnemy = GetEnemy();
	float DistanceToEnemy = DISTANCE_MAX;

	if ( currentEnemy )
	{
		DistanceToEnemy = (Vector(currentEnemy->pev->origin) - Vector(pev->origin)).Length();
	}

	for ( int i = 0; i < MAX_ITEM_TYPES; i++ )
	{
		for ( CBasePlayerItem* weapon = m_rgpPlayerItems[i]; weapon; weapon = weapon->m_pNext )
		{
			if ( !weapon->CanDeploy() )
			{
				// No point checking.
				continue;
			}

			CGenericWeapon* genericWeapon = dynamic_cast<CGenericWeapon*>(weapon);

			// Should never happen anyway:
			if ( !genericWeapon )
			{
				continue;
			}

			// This randomizes what weapon a given bot will choose at any particular moment
			// and yet maintains preferencial weapon bias.
			float CheckWeaponDesire = genericWeapon->Bot_CalcDesireToUse(*this, *currentEnemy, DistanceToEnemy);
			CheckWeaponDesire *= (Stats.GetTraitWeaponPreference(genericWeapon->WeaponAttributes().Core.Id) / 100.0f);
			CheckWeaponDesire *= RANDOM_FLOAT(0, 1);

			if ( CheckWeaponDesire > BestWeaponDesire )
			{
				BestWeaponDesire = static_cast<int>(CheckWeaponDesire);
				pBestWeapon = genericWeapon;
			}
		}
	}

	// this->pev check is protecting against us trying to call SwitchWeapon on a dead man's weapon
	if ( pBestWeapon && this->pev )
	{
		SwitchWeapon(pBestWeapon);
	}

	// Use the active item instead of the cached best weapon, just in case the
	// call to SwitchWeapon failed for some reason and the best weapon is not active.
	CGenericWeapon* activeWeapon = dynamic_cast<CGenericWeapon*>(m_pActiveItem);
	if ( activeWeapon )
	{
		FightStyle.DispatchWeaponUse(*activeWeapon);
	}
	else
	{
		FightStyle.UseWeaponDefault();
	}
}

///////////////////////////////////////////////////////////////////////////////
// ActionLook - reacts to immediate threats, and set m_pLink as head of item list
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ActionLook(int SearchDistance)
{
	// section one: handle client entitites

	float DistanceToEntity;
	float NearestEnemyDistance = DISTANCE_MAX;

	// Vacindak: changed "!GetEnemy" to "GetEnemy == NULL"
	if ( GetEnemy() == NULL ||
		 ((gpGlobals->time - Memory.GetEnemyLastSeenTime()) > (3. * Stats.GetTraitAggression() / 100.)) )
	{
		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			CBaseEntity* pPlayerEntity = UTIL_PlayerByIndex(i);
			CBasePlayer* pPlayer = (CBasePlayer*)pPlayerEntity;

			if ( pPlayer  // UTIL_PlayerByIndex can return NULL
				 && pPlayer != this && pPlayer->IsAlive() && CheckVisible(pPlayer) &&
				 FInViewCone(pPlayer)  // Scott: must be in sight cone
				 // the next set of logic uses cvars
				 && (bot_dontshoot.value == 0)  // can shoot
				 && (!pPlayer->IsNetClient() || (bot_observer.value == 0))  // bot or not observer mode
			)
			{
				if ( g_pGameRules->PlayerRelationship(this, pPlayer) == GR_NOTTEAMMATE && CheckNotice(pPlayer) )
				{
					DistanceToEntity = (Vector(pPlayer->pev->origin) - Vector(pev->origin)).Length();

					if ( DistanceToEntity < NearestEnemyDistance )
					{
						NearestEnemyDistance = DistanceToEntity;
						SetEnemy(pPlayer);
					}
				}
				else if (
					!pPlayer->IsFakeClient() && g_pGameRules->PlayerRelationship(this, pPlayer) != GR_NOTTEAMMATE )
				{
					DistanceToEntity = (Vector(pPlayer->pev->origin) - Vector(pev->origin)).Length();

					if ( DistanceToEntity < GROUPING_DISTANCE )
					{
						SteerSafeGroupFollow(pPlayer);
					}
					else
					{
						SteerSafePursue(pPlayer, STEER_GROUP_ASSEMBLE);
					}
				}
				else if ( pPlayer->IsFakeClient() && g_pGameRules->PlayerRelationship(this, pPlayer) != GR_NOTTEAMMATE )
				{
					DistanceToEntity = (Vector(pPlayer->pev->origin) - Vector(pev->origin)).Length();

					if ( DistanceToEntity < GROUPING_DISTANCE )
					{
						SteerSafeGroupFlock();
					}
					else
					{
						SteerSafePursue(pPlayer, STEER_GROUP_ASSEMBLE);
					}
				}
			}
		}

		if ( GetEnemy() != NULL )
		{
			//			ActionSpeak("I've found an enemy.");

			//			ActionChooseWeapon();

			SetLookAtBiasVec(Vector(RANDOM_FLOAT(-1, 1), RANDOM_FLOAT(-1, 1), RANDOM_FLOAT(-1, 1)));

			Memory.EnemySighted(GetEnemy());
		}
	}

	// section two: sets m_pLink as head of a linked list containing all possible pGoal ents

	m_pLink = NULL;
	CBaseEntity* pNextEnt = NULL;  // the current visible entity that we're dealing with
	CBaseEntity* pList[100];
	Vector delta = Vector(
		static_cast<float>(SearchDistance),
		static_cast<float>(SearchDistance),
		static_cast<float>(SearchDistance));
	BOOL CheckGoalThisFrame = FALSE;
	BOOL NoGoalAtStartOfFrame = FALSE;

	if ( GetGoal() == NULL )
	{
		NoGoalAtStartOfFrame = TRUE;
	}

	if ( gpGlobals->time > GetTimeGoalCheck() )
	{
		CheckGoalThisFrame = TRUE;
		SetTimeGoalCheck(gpGlobals->time + GetTimeGoalCheckDelay());
	}

	int count = UTIL_EntitiesInBox(pList, 100, Vector(pev->origin) - delta, Vector(pev->origin) + delta, 0);
	for ( int i = 0; i < count; i++ )
	{
		pNextEnt = pList[i];

		if ( pNextEnt && pNextEnt != this && CheckVisible(pNextEnt) )
		{
			if ( CheckNotice(pNextEnt) )
			{
				DistanceToEntity = (Vector(pNextEnt->pev->origin) - Vector(pev->origin)).Length();

				if ( FClassnameIs(pNextEnt->pev, "monster_satchel") || FClassnameIs(pNextEnt->pev, "monster_snark") ||
					 FClassnameIs(pNextEnt->pev, "hornet") || FClassnameIs(pNextEnt->pev, "rpg_rocket") ||
					 FClassnameIs(pNextEnt->pev, "grenade") )
				{
					SteerSafeEvade(pNextEnt);
				}
				else if ( FClassnameIs(pNextEnt->pev, "laser_spot") )  // rpg laser spot
				{
					SteerSafeFlee(pNextEnt);
				}
				else if ( FClassnameIs(pNextEnt->pev, "beam") )  // tripmine beam
				{
					CBeam* pBeam = (CBeam*)pNextEnt;
					Vector BeamStart = pBeam->GetStartPos();
					if ( (BeamStart - Vector(pev->origin)).Length() < 375 )
					{
						SteerSafeFlee(BeamStart);  // run away from the tripmine if within range
					}
				}
				else if ( FClassnameIs(pNextEnt->pev, "monster_tripmine") )
				{
					if ( DistanceToEntity > 375 )  // safe to shoot tripmine?
					{
						if ( GetEnemy() != NULL )
						{
							if ( (Vector(pNextEnt->pev->origin) - Vector(GetEnemy()->pev->origin)).Length() < 375 )
							{
								AimAtEntity(pNextEnt);
							}
						}
						else
						{
							AimAtEntity(pNextEnt);
						}
					}
					else
					{
						SteerSafeFlee(pNextEnt);
					}
				}
				else if ( FClassnameIs(pNextEnt->pev, "func_button") )  // hit any button within range
				{
					if ( DistanceToEntity < 64 )  // 64 is the usual maximum range of a use command
					{
						pev->button |= IN_USE;
					}
				}
				else if ( NoGoalAtStartOfFrame && CheckGoalThisFrame )
				{
					pNextEnt->m_pLink = m_pLink;  // linked list: link new list head to previous list head
					m_pLink = pNextEnt;  // linked list: link rho-bot to list head
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// ActionReload
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::ActionReload(void)
{
	CBasePlayerWeapon* pActiveWeapon = (CBasePlayerWeapon*)m_pActiveItem;

	if ( pActiveWeapon->pszAmmo1() && pActiveWeapon->iMaxClip() != WEAPON_NOCLIP &&
		 pActiveWeapon->m_iClip != pActiveWeapon->iMaxClip() )
	{
		pev->button |= IN_RELOAD;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// ActionSpeak
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ActionSpeak(const char* pText)
{
	char buffer[256];
	PlatformLib_SNPrintF(buffer, sizeof(buffer), "%s: %s\n", STRING(pev->netname), pText);
	UTIL_SayTextAll(buffer, this);
}

///////////////////////////////////////////////////////////////////////////////
// CheckBotKicked (Scott - adapted from botman's bot10)
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckBotKick()
{
	if ( g_fGameOver )
	{
		ThinkDead();

		return TRUE;
	}

	// if the bot is dead, randomly press fire to respawn...
	if ( (pev->health < 1) || (pev->deadflag != DEAD_NO) )
	{
		HandleTime();
		ThinkDead();

		pev->button = 0;  // make sure no buttons are pressed

		if ( RANDOM_LONG(1, 100) > 50 )
		{
			pev->button = IN_ATTACK;
		}

		g_engfuncs.pfnRunPlayerMove(
			edict(),
			pev->v_angle,
			GetMoveForward(),
			GetMoveStrafe(),
			GetMoveVertical(),
			static_cast<unsigned short>(pev->button),
			0,
			static_cast<byte>(GetMSec()));
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// GetBotThinkDelay() - How often does the bot update its thinking
//                      Range: 0.1 to 0.4 seconds, 0.25 is average
///////////////////////////////////////////////////////////////////////////////

float CBaseBot::GetBotThinkDelay()
{
	return (0.001f + 0.005f * (100 - Stats.GetTraitReflexes()));
}

///////////////////////////////////////////////////////////////////////////////
// BotThink() - the main loop
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::BotThink(void)
{
	if ( CheckBotKick() )
	{
		return;  // Exit if bot has been kicked or is dead
	}

	if ( fNextThink < gpGlobals->time )
	{
		ThinkStart();

		if ( GetEnemy() != NULL )
		{
			ThinkValidateEnemy();
		}

		if ( GetGoal() )
		{
			ThinkValidateGoal();
		}

		ActionLook();

		ActionChooseGoal();

		ThinkMood();

		ThinkSteering();

		if ( GetCalledAimThisFrame() == FALSE )
		{
			AimWander();
		}

		HandleMovement();

		Memory.EndFrameUpdate(this);

		fNextThink = gpGlobals->time + GetBotThinkDelay();
	}
	else
	{
		HandleTime();
	}

	// TODO: One day the input manager should be responsible for all buttons!
	pev->button &= ~(IN_ATTACK | IN_ATTACK2);
	Input.Think();
	pev->button |= Input.GetButtons();

	g_engfuncs.pfnRunPlayerMove(
		edict(),
		pev->v_angle,
		GetMoveForward(),
		GetMoveStrafe(),
		GetMoveVertical(),
		static_cast<unsigned short>(pev->button),
		0,
		static_cast<byte>(GetMSec()));
}

///////////////////////////////////////////////////////////////////////////////
// CheckHasPowerfulWeapon
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckHasPowerfulWeapon(void)
{
	for ( int i = 2; i < 4; i++ )  // have weapon from groups 3 or 4?
	{
		if ( m_rgpPlayerItems[i] )
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CheckNotice
///////////////////////////////////////////////////////////////////////////////

// 26-Aug-2001: Scott: totally rewritten

BOOL CBaseBot::CheckNotice(CBaseEntity* pEntity)
{
	if ( FInViewCone(pEntity) )
	{
		float visibility_helper = 0.;

		if ( FVisible(pEntity->pev->origin) )
		{
			visibility_helper += 0.3f;
		}

		if ( FVisible(pEntity->Center()) )
		{
			visibility_helper += 0.4f;
		}

		if ( FVisible(pEntity->EyePosition()) )
		{
			visibility_helper += 0.3f;
		}

		if ( visibility_helper == 0.0 )
		{
			return FALSE;  // bot can't see entity
		}

		float perceptTweak = 1.0f - Stats.GetTraitPerception() / 100.0f;  // [1.,0]

		if ( perceptTweak == 0.0 )
		{
			return TRUE;  // bot has perfect perception
		}

		Vector NoticeVector = pEntity->Center() - EyePosition();

		float distance_helper = NoticeVector.Length();  // raw distance

		if ( distance_helper > DISTANCE_MAX )  // just in case
		{
			distance_helper = DISTANCE_MAX;
		}

		if ( distance_helper < 32 )  // below this is negligible
		{
			distance_helper = 32;
		}

		distance_helper = DISTANCE_MAX / (16 * distance_helper);  // [1/16, 16]

		distance_helper *= distance_helper;  // [1/256, 256]

		float velocity_helper = VectorLength(pEntity->pev->velocity) / CVAR_GET_FLOAT("sv_maxspeed");  // velocity ratio

		velocity_helper = (velocity_helper * (2.0f * velocity_helper + 1.0f) + 1.0f);  // [1, 4]

		float angle_helper = 1.0;

		if ( NoticeVector.Length() > 128. )
		{
			UTIL_MakeVectors(pev->v_angle);

			angle_helper = DotProduct(
				Vector(gpGlobals->v_forward).Normalize(),
				NoticeVector.Normalize());  // cosine of view angle to entity angle [0, 1]
		}

		float notice_threshold = visibility_helper * distance_helper * velocity_helper * angle_helper;

		if ( notice_threshold > perceptTweak )
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CheckVisible
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckVisible(CBaseEntity* pEntity)
{
	if ( !pEntity || pEntity == this || !(pEntity->pev) )  //
	{
		return FALSE;  // intuition dictates this should return TRUE for this, but our Validate functions will fix
					   // things if this returns FALSE
	}

	return FVisible(pEntity);
}

///////////////////////////////////////////////////////////////////////////////
// HandleTime - from Rich Whitehouse's Advanced Bot Frame
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::HandleTime(void)
{
	if ( GetTimeMSecCheck() <= gpGlobals->time )
	{
		SetTimeMSecCheck(gpGlobals->time + 0.5f);  // Scott:  was + 1

		if ( GetMSecCounter() > 0 )
		{
			SetMSec(static_cast<float>(450 / GetMSecCounter()));  // Scott:  was 1000
		}

		SetMSecCounter(0);
	}
	else
	{
		SetMSecCounter(GetMSecCounter() + 1);
	}

	if ( GetMSec() < 1 )  // was 10
	{
		SetMSec(1);
	}

	if ( GetMSec() > 100 )  // was 75
	{
		SetMSec(100);
	}
}

///////////////////////////////////////////////////////////////////////////////
// ThinkDead
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ThinkDead(void)
{
	SetDesiredVelocity(Vector(0, 0, 0));
	SetGoUpOnLadder(TRUE);
	SetLookAtVec(Vector(0, 0, 0));
	SetMovedDistance(0.0);
	SetMoveForward(0.0);
	SetMoveStrafe(0.0);
	SetMoveVertical(0.0);
	SetEnemy(NULL);
	SetGoal(NULL);
	SetRoamDirection(UNKNOWN);
	SetTimeGoalCheck(gpGlobals->time);
	SetTurningDirection(NONE);
	SetWantToBeInCombat(FALSE);
	//	pev->velocity = Vector(0,0,0);
	//	pev->avelocity = Vector(0,0,0);
	//	pev->speed = 0;
}

///////////////////////////////////////////////////////////////////////////////
// ThinkMood
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ThinkMood(void)
{
	CBasePlayerWeapon* pActiveWeapon = dynamic_cast<CBasePlayerWeapon*>(m_pActiveItem);

	if ( !pActiveWeapon || pActiveWeapon->m_fInReload )
	{
		SetWantToBeInCombat(FALSE);
		return;
	}

	if ( CheckHasPowerfulWeapon() )
	{
		SetWantToBeInCombat(TRUE);
	}
	else if ( GetEnemy() != NULL )
	{
		// Vacindak: Messy, but i did this late at night and errors about with the single line alternative for some
		// reason
		CBaseEntity* enemy = GetEnemy();
		CBasePlayer* enemyPlayer = (CBasePlayer*)enemy;

		if ( (pev->health + pev->armorvalue) + (Stats.GetTraitAggression() - 50) >
			 (enemyPlayer->pev->health + enemyPlayer->pev->armorvalue) )
		{
			SetWantToBeInCombat(TRUE);
		}
		else
		{
			SetWantToBeInCombat(FALSE);
		}
	}
	else
	{
		if ( ((pev->health + pev->armorvalue) > (140 - Stats.GetTraitAggression())) && CheckHasPowerfulWeapon() )
		{
			SetWantToBeInCombat(TRUE);
		}
		else
		{
			SetWantToBeInCombat(FALSE);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// ThinkStart
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ThinkStart(void)
{
	HandleTime();

	SetCalledAimThisFrame(FALSE);

	SetSteerCallCounter(0);

	SetSteerCallPrecedence(STEER_WANDER);

	pev->button = 0;

	SetMovedDistance((Memory.GetPrevOrigin() - Vector(pev->origin)).Length());

	// 26-Aug-2001: Scott: fix marking location, included marking favorable
	if ( IsAlive() && GetSteerCallPrecedence() != STEER_WANDER )
	{
		// Mark location favorable if bot not wandering
		WorldGraph.MarkLocationFavorable(pev->origin);
	}
	if ( GetMovedDistance() <= 1.0 && GetDesiredVelocity().Length2D() > 0.0 )
	{
		// Mark the location the bot was trying to go to as unfavorable

		Vector start = pev->origin;
		UTIL_MakeVectors(pev->v_angle);
		Vector moveDir =
			(Vector(gpGlobals->v_forward) * GetMoveForward() + Vector(gpGlobals->v_right) * GetMoveStrafe())
				.Normalize();
		float scaleFactor = NAV_GRIDBOX_SIZE / (fabs(moveDir.x) > fabs(moveDir.y) ? fabs(moveDir.x) : fabs(moveDir.y));
		Vector dest = start + moveDir * scaleFactor;
		WorldGraph.MarkLocationUnfavorable(dest);
	}
	// 26-Aug-2001: end
}

///////////////////////////////////////////////////////////////////////////////
// ThinkSteering
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ThinkSteering(void)
{
	const bool canJump = bot_dontmove.value == 0.0f;

	// Vacindak: changed "GetEnemy()" to "GetEnemy() != NULL"
	if ( GetEnemy() != NULL && GetEnemy()->IsAlive() )
	{
		if ( CheckVisible(GetEnemy()) )
		{
			AimAtEnemy();

			if ( bot_dontshoot.value == 0.0f )
			{
				ActionOpenFire();
			}

			Memory.EnemyInSight(GetEnemy());
		}
		else
		{
			if ( Memory.GetEnemyInSight() != FALSE )  // i.e. this is the first time through
			{
				if ( !ActionReload() )  // change weapons if we're not going to reload
				{
					ActionChooseWeapon();
				}
			}

			Memory.EnemyOutOfSight();
		}

		if ( CheckHasPowerfulWeapon() )
		{
			if ( GetWantToBeInCombat() )
			{
				if ( canJump && RANDOM_FLOAT(1, 4000) < Stats.GetTraitJumpPropensity() )
				{  // jump a lil bit during a duel
					pev->button |= IN_JUMP;
				}

				if ( Memory.GetEnemyInSight() )
				{
					SteerSafePursue(GetEnemy());
				}
				else
				{
					SteerSafeSeek(Memory.GetEnemyLastSeenPosition(), STEER_WANT_ENEMY);
				}
			}
			else
			{
				if ( canJump && RANDOM_FLOAT(1, 2000) < Stats.GetTraitJumpPropensity() )
				{  // jump a bit more if you don't want to be in combat
					pev->button |= IN_JUMP;
				}

				if ( Memory.GetEnemyInSight() )
				{
					SteerSafeEvade(GetEnemy());
				}
				else
				{
					SteerSafeFlee(Memory.GetEnemyLastSeenPosition());
				}
			}
		}
		else
		{
			if ( canJump && RANDOM_FLOAT(1, 1000) < Stats.GetTraitJumpPropensity() )
			{  // jump a lot if you don't have a decent weapon
				pev->button |= IN_JUMP;
			}

			if ( GetGoal() )
			{
				SteerSafeSeek(GetGoal());
			}
			else
			{
				SteerSafeWanderImproved();
			}
		}
	}
	else
	{
		ActionReload();

		if ( GetGoal() )
		{
			SteerSafeSeek(GetGoal());
		}
		else
		{
			SteerSafeWanderImproved();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// ThinkValidateEnemy
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ThinkValidateEnemy(void)
{
	// Vacindak: changed "GetEnemy()" to "GetEnemy() != NULL"
	if ( GetEnemy() != NULL && CheckVisible(GetEnemy()) && FInViewCone(GetEnemy()) )
	{
		Memory.EnemyInSight(GetEnemy());
	}

	// Vacindak: changed "!GetEnemy()" to "GetEnemy() == NULL"
	if ( GetEnemy() == NULL || GetEnemy()->pev->deadflag != DEAD_NO ||
		 (gpGlobals->time - Memory.GetEnemyLastSeenTime()) > (10 + ((Stats.GetTraitAggression() - 50) / 10)) )
	{
		//		ActionSpeak("I'm giving up on my current target.");
		SetEnemy(NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////
// ThinkValidateGoal
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::ThinkValidateGoal(void)
{  // TODO: eventually rather than check visibility we should should check to make sure there's still a valid path
	// Vacindak: changed "!GetGoal()" to "GetGoal() == NULL"
	if ( GetGoal() == NULL || !CheckVisible(GetGoal()) || (GetGoal()->pev->effects & EF_NODRAW) )
	{
		SetGoal(NULL);
		//		ActionSpeak( "Giving up on goal." );
	}
}
