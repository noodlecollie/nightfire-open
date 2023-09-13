
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
#include "weapons.h"
#include "bot.h"
#include "game.h"
#include "nodes.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////

CBaseBotStats::CBaseBotStats() :
	TraitAccuracy(75 + RANDOM_FLOAT(0, 25)),
	TraitAggression(10 + RANDOM_FLOAT(0, 70)),
	TraitChatPropensity(0 + RANDOM_FLOAT(0, 60)),
	TraitJumpPropensity(5 + RANDOM_FLOAT(0, 94)),
	TraitPerception(50 + RANDOM_FLOAT(0, 50)),
	TraitReflexes(30 + RANDOM_FLOAT(0, 40))
{
	// Index 0 is no weapon.
	for ( uint32_t index = 1; index < MAX_WEAPONS; ++index )
	{
		TraitWeaponPreference[index] = 30 + RANDOM_FLOAT(0, 40);
	}
}

CBaseBotStats::~CBaseBotStats()
{
}

///////////////////////////////////////////////////////////////////////////////
// FDifficultyAdjusted - tweaks a Trait to adjust for bot_skill.value
///////////////////////////////////////////////////////////////////////////////
// Scott: simplified logic

float CBaseBotStats::FDifficultyAdjusted(float currentTrait) const
{
	float tweakedDiffLevel = (bot_skill.value / 50.0f) - 1.0f;  // [-1,1]

	float adjustedTrait = currentTrait;

	if ( tweakedDiffLevel < 0 )
	{
		adjustedTrait *= (1 + tweakedDiffLevel);
	}
	else
	{
		adjustedTrait += ((100 - adjustedTrait) * tweakedDiffLevel);
	}

	return adjustedTrait;
}

void CBaseBotStats::SetTraitWeaponPreference(WeaponId_e id, const float newTrait)
{
	if ( id != WeaponId_e::WeaponNone )
	{
		TraitWeaponPreference[static_cast<uint32_t>(id)] = newTrait;
	}
}

float CBaseBotStats::GetTraitWeaponPreference(WeaponId_e id) const
{
	return id != WeaponId_e::WeaponNone ? TraitWeaponPreference[static_cast<uint32_t>(id)] : 0.0f;
}
