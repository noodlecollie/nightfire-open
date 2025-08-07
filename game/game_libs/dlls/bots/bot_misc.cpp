
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

#include <fstream>
#include "extdll.h"
#include "util.h"
#include "client.h"
#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "bot.h"
#include "bot_misc.h"
#include "nodes.h"
#include "mp_utils.h"
#include "PlatformLib/String.h"

CBaseBotGlobals BotGlobals;

///////////////////////////////////////////////////////////////////////////////
// CBaseBotGlobals Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////

CBaseBotGlobals::CBaseBotGlobals() :
	AutoAdjCheckTime(12)
{
}

CBaseBotGlobals::~CBaseBotGlobals()
{
}

///////////////////////////////////////////////////////////////////////////////
// IncreaseDifficulty
///////////////////////////////////////////////////////////////////////////////

void CBaseBotGlobals::IncreaseDifficulty(void)
{
	char serverCommand[128] = "bot_skill ###";
	int botSkill = (int)(bot_skill.value);

	if ( botSkill < 99 )
	{
		PlatformLib_SNPrintF(serverCommand, sizeof(serverCommand), "bot_skill %d\n", (botSkill + 1));
		//		CVAR_SET_FLOAT("bot_skill", ((bot_skill.value)+1));
	}
	else
	{
		PlatformLib_SNPrintF(serverCommand, sizeof(serverCommand), "bot_skill %d\n", 100);
		//		CVAR_SET_FLOAT("bot_skill", 100);
	}
	SERVER_COMMAND(serverCommand);
	if ( IS_DEDICATED_SERVER() )
		printf("%s", serverCommand);
}

///////////////////////////////////////////////////////////////////////////////
// DecreaseDifficulty
///////////////////////////////////////////////////////////////////////////////

void CBaseBotGlobals::DecreaseDifficulty(void)
{
	char serverCommand[128] = "bot_skill ###";
	int botSkill = (int)(bot_skill.value);

	if ( botSkill > 1 )
	{
		PlatformLib_SNPrintF(serverCommand, sizeof(serverCommand), "bot_skill %d\n", (botSkill - 1));
		//		CVAR_SET_FLOAT("bot_skill", (botSkill-1));
	}
	else
	{
		PlatformLib_SNPrintF(serverCommand, sizeof(serverCommand), "bot_skill %d\n", 0);
		//		CVAR_SET_FLOAT("bot_skill", 0);
	}
	SERVER_COMMAND(serverCommand);
	if ( IS_DEDICATED_SERVER() )
		printf("%s", serverCommand);
}

///////////////////////////////////////////////////////////////////////////////
// HandleAutoAdjDifficulty
///////////////////////////////////////////////////////////////////////////////

void CBaseBotGlobals::HandleAutoAdjDifficulty(void)
{
	if ( (int)(bot_skill_aa.value) != 0 )  // starbreaker - now use cvar value
	{
		if ( AutoAdjCheckTime < gpGlobals->time )
		{
			float BotFragTotal = 1;  // starting at zero so we don't divide by zero
			float BotDeathTotal = 1;
			float HumanFragTotal = 1;
			float HumanDeathTotal = 1;

			for ( int i = 1; i <= gpGlobals->maxClients; i++ )
			{
				CBasePlayer* pPlayer = MPUtils::CBasePlayerFromIndex(i);

				if ( pPlayer )
				{
					if ( pPlayer->IsFakeClient() )
					{
						BotFragTotal += pPlayer->pev->frags;
						BotDeathTotal += pPlayer->m_iDeaths;
					}
					else if ( !pPlayer->IsFakeClient() && pPlayer->IsNetClient() )
					{
						HumanFragTotal += pPlayer->pev->frags;
						HumanDeathTotal += pPlayer->m_iDeaths;
					}
				}
			}

			if ( (BotFragTotal / BotDeathTotal) < (HumanFragTotal / HumanDeathTotal) )
			{
				IncreaseDifficulty();
			}
			else if ( (BotFragTotal / BotDeathTotal) > (HumanFragTotal / HumanDeathTotal) )
			{
				DecreaseDifficulty();
			}

			AutoAdjCheckTime += 12;  // AutoAdj every 12 seconds
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// TakeDamage - turn bot toward attacker, set as enemy
//              Scott: Adapted from botman's Bot10
///////////////////////////////////////////////////////////////////////////////

int CBaseBot::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	CBaseEntity* pAttacker = CBaseEntity::Instance(pevAttacker);
	int ret_damage;

	// do the damage first...
	ret_damage = CBasePlayer::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);

	// if the bot injures itself, or by a nonplayer,
	// do not turn to attack...
	if ( pAttacker == this || !pAttacker->IsPlayer() )
		return ret_damage;

	// if the bot doesn't have an enemy and someone is shooting at it then
	// turn in the attacker's direction...
	if ( GetEnemy() == NULL || ((gpGlobals->time - Memory.GetEnemyLastSeenTime()) > 1) )
	{
		SetEnemy(NULL);  // Make sure enemy is reset

		// face the attacker...
		SetLookAtVec(Vector(pAttacker->pev->origin) - Vector(pev->origin));

		// set enemy if not a teammate
		if ( pAttacker->IsAlive() && g_pGameRules->PlayerRelationship(this, pAttacker) == GR_NOTTEAMMATE )
		{
			SetWantToBeInCombat(TRUE);
			SetEnemy(pAttacker);
		}

		if ( GetEnemy() != NULL )
		{
			//			ActionSpeak("I've found an enemy.");

			ActionChooseWeapon();

			SetLookAtBiasVec(Vector(RANDOM_FLOAT(-1, 1), RANDOM_FLOAT(-1, 1), RANDOM_FLOAT(-1, 1)));

			Memory.EnemySighted(GetEnemy());
		}
	}

	return ret_damage;
}
