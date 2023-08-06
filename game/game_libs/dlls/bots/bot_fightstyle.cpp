
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
#include "botweaponattributes.h"
#include "genericweapon.h"

///////////////////////////////////////////////////////////////////////////////
// Constructors/Destructors
///////////////////////////////////////////////////////////////////////////////

CBaseBotFightStyle::CBaseBotFightStyle() :
	AimAt(AIM_BODY),
	bHoldDownAttack(FALSE),
	bSecondaryFire(FALSE),
	fNextShootTime(gpGlobals->time)
{
}

CBaseBotFightStyle::~CBaseBotFightStyle()
{
}

///////////////////////////////////////////////////////////////////////////////
// DispatchWeaponUse
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::DispatchWeaponUse(CGenericWeapon& weapon)
{
	SetHoldDownAttack(FALSE);  // unless the particular weapon sets this TRUE we want it false
	weapon.Bot_SetFightStyle(*this);
}

///////////////////////////////////////////////////////////////////////////////
// RandomizeAimAtHead
///////////////////////////////////////////////////////////////////////////////
// Scott: now modified by bot accuracy.
// The better the shot, the more likely a head shot

void CBaseBotFightStyle::RandomizeAimAtHead(const int AimAtHeadPropensity)
{
	int BotAccuracy = static_cast<int>(pOwner->Stats.GetTraitAccuracy());
	int ModifiedAim = (AimAtHeadPropensity * BotAccuracy) / 100;

	if ( (RANDOM_LONG(0, 100) < ModifiedAim) || (BotAccuracy >= 99) )
	{
		SetAimAt(AIM_HEAD);
	}
	else
	{
		SetAimAt(AIM_BODY);
	}
}

///////////////////////////////////////////////////////////////////////////////
// RandomizeSecondaryFire
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::RandomizeSecondaryFire(const int SecondaryFirePropensity)
{
	SetSecondaryFire(SecondaryFirePropensity > 99 || RANDOM_LONG(0, 100) < SecondaryFirePropensity);
}

///////////////////////////////////////////////////////////////////////////////
// SetNextShootTime - Scott: set the time for the next bot shot
//    The delay parameters are based on Botman's HPB bot
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::SetNextShootTime(
	const float nextAllowedShootTime,
	const float extraWaitMin,
	const float extraWaitMax)
{
	float delayFactor = 1.0f - (pOwner->Stats.GetTraitReflexes() / 100.0f);

	fNextShootTime = nextAllowedShootTime + (RANDOM_FLOAT(extraWaitMin, extraWaitMax) * delayFactor);

	fNextEvaluationTime = fNextShootTime > gpGlobals->time ? fNextShootTime : gpGlobals->time;

	if ( GetHoldDownAttack() )  // for continuous firing, stop every two seconds
	{
		fNextEvaluationTime += 2.0f;
	}
	else  // Stop shooting 0.1+GetBotThinkDelay seconds after the first shot
	{
		fNextEvaluationTime += 0.1f + pOwner->GetBotThinkDelay();
	}
}

void CBaseBotFightStyle::UseWeaponDefault(void)
{
	SetSecondaryFire(FALSE);
	RandomizeAimAtHead(50);
	SetNextShootTime(0.75f, 1.5f, 2.2f);
}

// Old Half-Life weapon use functions are below, for posterity.

///////////////////////////////////////////////////////////////////////////////
// UseCrossbow
///////////////////////////////////////////////////////////////////////////////

/*
void CBaseBotFightStyle::UseCrossbow( void )
{
	CBaseBot *pBotOwner = (CBaseBot *)pOwner;

	float DistanceToEnemy = ( pBotOwner->GetEnemy()->pev->origin - pBotOwner->pev->origin ).Length();

	if (  DistanceToEnemy > DISTANCE_FAR )
	{
		RandomizeSecondaryFire( 90 );
	}
	else if ( DistanceToEnemy > DISTANCE_NEAR )
	{
		RandomizeSecondaryFire( 60 );
	}
	else
	{
		RandomizeSecondaryFire( 20 );
	}

	if ( GetSecondaryFire() )
	{
		RandomizeAimAtHead( 60 );
		SetNextShootTime ( 0.2, 0.0, 0.0 );
	}
	else
	{
		SetAimAt( AIM_SPLASH );
		SetNextShootTime ( 0.75, 1.0, 1.3 );
	}
}

///////////////////////////////////////////////////////////////////////////////
// UseCrowbar
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseCrowbar( void )
{
	SetSecondaryFire( FALSE );

	RandomizeAimAtHead( 80 );

	SetHoldDownAttack( TRUE );
	SetNextShootTime ( 0.3, 0.6, 1.0 );

}

///////////////////////////////////////////////////////////////////////////////
// UseEgon
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseEgon( void )
{
	SetSecondaryFire( FALSE );

	RandomizeAimAtHead( 5 );

	SetHoldDownAttack( TRUE );
	SetNextShootTime ( 0.2, 0.0, 0.0 );
}

///////////////////////////////////////////////////////////////////////////////
// UseGauss
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseGauss( void )
{
	RandomizeSecondaryFire( 70 );
//	SetSecondaryFire( FALSE );

	if ( GetSecondaryFire() )
	{
		RandomizeAimAtHead( 5 );
		SetHoldDownAttack( TRUE );
		SetNextShootTime ( 1.0, 1.2, 2.0 );
	}
	else
	{
		RandomizeAimAtHead( 20 );
		SetNextShootTime ( 0.2, 1.0, 1.2 );
	}
}

///////////////////////////////////////////////////////////////////////////////
// UseGlock
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseGlock( void )
{
	RandomizeSecondaryFire( 90 );

	RandomizeAimAtHead( 80 );

	SetHoldDownAttack( TRUE );
	SetNextShootTime ( 0.2, 0.2, 0.4 );
}

///////////////////////////////////////////////////////////////////////////////
// UseHandGrenade
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseHandGrenade( void )
{ // TODO: use grenadeToss ai (valve), also needs to do timing as well
	SetSecondaryFire( FALSE );

	SetAimAt( AIM_SPLASH );
	SetNextShootTime ( 0.1, 5.0, 7.0 );
}

///////////////////////////////////////////////////////////////////////////////
// UseHornetGun
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseHornetGun( void )
{
	RandomizeSecondaryFire( 20 );

	RandomizeAimAtHead( 50 );

	SetHoldDownAttack( TRUE );
	SetNextShootTime ( 0.25, 1.0, 1.5 );
}

///////////////////////////////////////////////////////////////////////////////
// UseMP5
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseMP5( void )
{ // TODO: should mixup ATTACK1/ATTACK2 mid-attack
	CBaseBot *pBotOwner = (CBaseBot *)pOwner;

	if ( (pBotOwner->GetEnemy()->pev->origin - pBotOwner->pev->origin).Length() < DISTANCE_NEAR )
	{
		SetSecondaryFire( FALSE );
	}
	else
	{
		RandomizeSecondaryFire( 10 );
	}

	if ( GetSecondaryFire() )
	{
		SetAimAt( AIM_SPLASH );
		SetNextShootTime ( 1.0, 1.4, 2.0 );
	}
	else
	{
		RandomizeAimAtHead( 30 );
		SetHoldDownAttack( TRUE );
		SetNextShootTime ( 0.1, 0.5, 0.8 );
	}
}

///////////////////////////////////////////////////////////////////////////////
// UsePython
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UsePython( void )
{
	SetSecondaryFire( FALSE );

	RandomizeAimAtHead( 80 );
	SetNextShootTime ( 0.75, 1.5, 2.2 );
}

///////////////////////////////////////////////////////////////////////////////
// UseRPG
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseRPG( void )
{ // TODO: should use dot sometimes
	SetSecondaryFire( FALSE );

	SetAimAt( AIM_SPLASH );
	SetNextShootTime ( 1.5, 5.0, 7.0 );
}

///////////////////////////////////////////////////////////////////////////////
// UseSatchel
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseSatchel( void )
{
	CBaseBot *pBotOwner = (CBaseBot *)pOwner;

	pBotOwner->ActionChooseWeapon(); // don't fight with satchels
}

///////////////////////////////////////////////////////////////////////////////
// UseShotgun
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseShotgun( void )
{
	CBaseBot *pBotOwner = (CBaseBot *)pOwner;

	if ( (pBotOwner->GetEnemy()->pev->origin - pBotOwner->pev->origin).Length() > DISTANCE_FAR )
	{
		RandomizeSecondaryFire( 20 );
	}
	else
	{
		RandomizeSecondaryFire( 90 );
	}

	RandomizeAimAtHead( 60 );
	SetNextShootTime ( 0.75, 0.8, 2.0 );
}

///////////////////////////////////////////////////////////////////////////////
// UseSnark
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseSnark( void )
{
	SetSecondaryFire( FALSE );

	SetAimAt( AIM_SPLASH );

	SetHoldDownAttack( TRUE );
	SetNextShootTime ( 0.2, 0.6, 1.0 );
}

///////////////////////////////////////////////////////////////////////////////
// UseTripMine
///////////////////////////////////////////////////////////////////////////////

void CBaseBotFightStyle::UseTripMine( void )
{
	CBaseBot *pBotOwner = (CBaseBot *)pOwner;

	pBotOwner->ActionChooseWeapon(); // don't fight with a tripmine
}
*/
