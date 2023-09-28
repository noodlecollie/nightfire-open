
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
#include "MathLib/angles.h"

///////////////////////////////////////////////////////////////////////////////
// AimAtBias
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::AimAtBias(void)
{
	// 23-Aug-2001: Scott: Modifed with botman's Bot10 CBot::BotBodyTarget
	Vector target;
	float f_distance;
	float f_scale;

	f_distance = GetLookAtVec().Length();
	f_scale = f_distance / 1000.0f;
	f_scale *= (1.0f - (Stats.GetTraitAccuracy() / 100.0f));

	// max error is 5 degress

	float x_error = RANDOM_FLOAT(-62, 62) * f_scale;
	float y_error = RANDOM_FLOAT(-62, 62) * f_scale;
	float z_error = RANDOM_FLOAT(-87, 87) * f_scale;
	SetLookAtBiasVec(GetLookAtBiasVec() + Vector(x_error, y_error, z_error));
	SetLookAtBiasVec(GetLookAtBiasVec());
	SetLookAtVec(GetLookAtVec() + GetLookAtBiasVec());
	SetLookAtVec(GetLookAtVec().Normalize());
	SetLookAtBiasVec(GetLookAtBiasVec().Normalize() * RANDOM_FLOAT(0, 1));  // Randomly reduce error
}

///////////////////////////////////////////////////////////////////////////////
// AimAtEnemy
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::AimAtEnemy(void)
{
	if ( GetCalledAimThisFrame() == FALSE )
	{
		SetCalledAimThisFrame(TRUE);

		if ( FightStyle.GetAimAt() == AIM_BODY )
		{
			SetLookAtVec(GetEnemy()->Center() - GetGunPosition());
		}
		else if ( FightStyle.GetAimAt() == AIM_HEAD )
		{
			SetLookAtVec(GetEnemy()->EyePosition() - GetGunPosition());
		}
		else if ( FightStyle.GetAimAt() == AIM_SPLASH )  // TODO: this should be aiming at feet's future position
		{
			SetLookAtVec(Vector(GetEnemy()->pev->origin) - GetGunPosition());
		}
		else
		{
			ActionSpeak("I don't know how to aim at my enemy!");
		}

		AimAtBias();
	}
}

///////////////////////////////////////////////////////////////////////////////
// AimAtEntity
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::AimAtEntity(CBaseEntity* pEntity)
{  // BAimAtEntity takes precedence over BAimAtEnemy because it's always invoked first
	if ( GetCalledAimThisFrame() == FALSE )
	{
		SetCalledAimThisFrame(TRUE);

		SetLookAtVec(Vector(pEntity->pev->origin) - GetGunPosition());

		AimAtBias();

		ActionOpenFire();
	}
}

///////////////////////////////////////////////////////////////////////////////
// AimWander - fluctuates around the direction the bot is travelling
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::AimWander(void)
{
	SetCalledAimThisFrame(TRUE);

	Vector newLookAtBiasVec =
		GetLookAtBiasVec() + Vector(RANDOM_FLOAT(-0.1f, 0.1f), RANDOM_FLOAT(-0.1f, 0.1f), RANDOM_FLOAT(-0.1f, 0.1f));
	newLookAtBiasVec.z = 0;
	newLookAtBiasVec = newLookAtBiasVec.Normalize();

	Vector newLookAtVec = Vector(pev->velocity).Normalize();
	newLookAtVec.z = 0;

	SetLookAtVec((newLookAtVec * 2) + newLookAtBiasVec);
}

///////////////////////////////////////////////////////////////////////////////
// CheckPathObstructedFront - ripped from botman's bot number six
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckPathObstructedFront(Vector& IdealVelocity, TraceResult& tr)
{
	Vector TraceSource;
	Vector TraceTo;

	// check at waist level
	TraceSource = pev->origin;
	TraceTo = TraceSource + (IdealVelocity.Normalize() * (AVOID_AHEAD_DISTANCE + WALL_ALIGN_DISTANCE));

	UTIL_TraceLine(TraceSource, TraceTo, ignore_monsters, ENT(pev), &tr);

	if ( tr.flFraction != 1.0 )
	{
		return TRUE;
	}

	// waist clear, check at eye level
	TraceSource = EyePosition();
	TraceTo = TraceSource + (IdealVelocity.Normalize() * (AVOID_AHEAD_DISTANCE + WALL_ALIGN_DISTANCE));

	UTIL_TraceLine(TraceSource, TraceTo, ignore_monsters, ENT(pev), &tr);

	if ( tr.flFraction != 1.0 )
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CheckPathObstructedSides
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckPathObstructedSides(Vector& IdealVelocityAngle, TraceResult& tr, TraceResult& tr2)
{
	Vector TraceSource = pev->origin;

	IdealVelocityAngle.y -= 45;
	Vector TraceTo = TraceSource + (IdealVelocityAngle.Normalize() * SIDE_BUFFER_DISTANCE);
	UTIL_TraceLine(TraceSource, TraceTo, ignore_monsters, ENT(pev), &tr);

	IdealVelocityAngle.y += 90;
	TraceTo = TraceSource + (IdealVelocityAngle.Normalize() * SIDE_BUFFER_DISTANCE);
	UTIL_TraceLine(TraceSource, TraceTo, ignore_monsters, ENT(pev), &tr2);

	if ( tr.flFraction != 1.0 || tr2.flFraction != 1.0 )  // hit anything?
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CheckWallOnLeft - ripped from botman's bot10
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckWallOnLeft(void)
{
	Vector v_src, v_left;
	TraceResult tr;

	UTIL_MakeVectors(pev->v_angle);

	// do a trace to the left...

	v_src = pev->origin;
	v_left = v_src + Vector(gpGlobals->v_right) * -40;  // 40 units to the left

	UTIL_TraceLine(v_src, v_left, dont_ignore_monsters, ENT(pev), &tr);

	// check if the trace hit something...
	if ( tr.flFraction < 1.0 )
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CheckWallOnRight - ripped from botman's bot10
///////////////////////////////////////////////////////////////////////////////

BOOL CBaseBot::CheckWallOnRight(void)
{
	Vector v_src, v_right;
	TraceResult tr;

	UTIL_MakeVectors(pev->v_angle);

	// do a trace to the right...

	v_src = pev->origin;
	v_right = v_src + Vector(gpGlobals->v_right) * 40;  // 40 units to the right

	UTIL_TraceLine(v_src, v_right, dont_ignore_monsters, ENT(pev), &tr);

	// check if the trace hit something...
	if ( tr.flFraction < 1.0 )
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// HandleMovement
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::HandleMovement(void)
{
	if ( bot_dontmove.value != 0.0f )
	{
		return;
	}

	// face a smoothed out LookAtVec
	Vector TrueLookAtVec = GetLookAtVec() + Memory.GetPrev1LookAtVec() + Memory.GetPrev2LookAtVec() +
		Memory.GetPrev3LookAtVec() + Memory.GetPrev4LookAtVec();
	Vector TrueLookAtVecAngles = UTIL_VecToAngles(TrueLookAtVec);

	TrueLookAtVecAngles.CopyToArray(pev->v_angle);

	// Scott: SDK 2.x fix for body direction. From botman's HPB Bot
	if ( pev->v_angle[YAW] > 180 )
	{
		pev->v_angle[YAW] -= 360;
	}

	if ( pev->v_angle[PITCH] > 180 )
	{
		pev->v_angle[PITCH] -= 360;
	}

	pev->angles[PITCH] = pev->v_angle[PITCH] / 3;
	pev->angles[YAW] = pev->v_angle[YAW];
	pev->angles[ROLL] = 0.0;
	pev->v_angle[PITCH] = -pev->v_angle[PITCH];  // invert this dimension for engine
	// End Fix

	float BotMoveSpeed = GetServerMaxSpeed();

	// set Move speeds to accomodate for the independent LookAtVec
	Vector TrueDesiredVelocity = GetDesiredVelocity() + Memory.GetPrev1Velocity() + Memory.GetPrev2Velocity() +
		Memory.GetPrev3Velocity() + Memory.GetPrev4Velocity();
	TrueDesiredVelocity = TrueDesiredVelocity * 0.2f;

	Vector DesiredVelocityAngle = HandleObstacles(TrueDesiredVelocity);
	Vector FacingAngle = pev->v_angle;

	// these next two ifs should ensure our y angles are [0,360]
	while ( DesiredVelocityAngle.y < 0 )
	{
		DesiredVelocityAngle.y += 360;
	}

	while ( FacingAngle.y < 0 )
	{
		FacingAngle.y += 360;
	}

	float Ydiff = FacingAngle.y - DesiredVelocityAngle.y;  // Ydiff now [-360,360]
	while ( Ydiff < 0 )
	{
		Ydiff += 360;  // Ydiff now [0,360]
	}

	/* 28-July-2001:  This appears to be all wrong.  Should use sines and cosines
		float TweakedYdiff = Ydiff;
		if ( TweakedYdiff > 180 )
		{
			TweakedYdiff -= 180; // TweakedYdiff now [0,180]
		}

		if ( TweakedYdiff > 90 )
		{
			TweakedYdiff -= (TweakedYdiff - 90) * 2; // TweakedYdiff now [0,90] w/ 0=>1.0 & 90=>0.0
		}

		TweakedYdiff *= -1; // -90=>0 & 0=>1
		TweakedYdiff += 90; // 0=>0 & 90=>1
		TweakedYdiff /= 90; // 0=>0 & 1=>1

		SetMoveForward( TweakedYdiff * BotMoveSpeed );

		if ( 90 < Ydiff && Ydiff < 270 )
		{
			SetMoveForward( GetMoveForward() * -1 ); // backpedal
		}

		SetMoveStrafe( BotMoveSpeed - GetMoveForward() ); // put into MoveStrafe whatever speed isn't already being used
	   by MoveForward

		if ( Ydiff > 180 )
		{
			SetMoveStrafe( GetMoveStrafe() * -1 ); // strafe left
		} */

	Ydiff *= (static_cast<float>(M_PI) / 180.0f);  // convert from degrees to radians
	SetMoveForward(cosf(Ydiff) * BotMoveSpeed);
	SetMoveStrafe(-sinf(Ydiff) * BotMoveSpeed);
	// 28-July-2001: end

	if ( IsOnLadder() )
	{
		SteerSafeOnLadder();
	}
	else
	{
		SetMoveVertical(0);  // TODO: i'm guessing this needs to be reworked to work in all 3D to handle swimming
	}
}

///////////////////////////////////////////////////////////////////////////////
// HandleObstacles - ripped from botman's bot number six
///////////////////////////////////////////////////////////////////////////////

Vector CBaseBot::HandleObstacles(Vector& IdealVelocity)
{
	TraceResult tr;
	TraceResult tr2;
	Vector DirectionHeadingAngle = UTIL_VecToAngles(IdealVelocity);

	// this tweaks DirectionHeadingAngle to accomdate obstacles in front of us
	if ( CheckPathObstructedFront(IdealVelocity, tr) )
	{
		Vector Normal =
			UTIL_VecToAngles(tr.vecPlaneNormal);  // vector that is perpendicular to the surface from the TraceResult

		float botY = DirectionHeadingAngle.y;  // [???,???]
		// NFTODO: Fix these so that we don't use loops
		while ( botY < 0 )  // [0,???]
		{
			botY += 360;
		}
		while ( botY > 360 )  // [0,360]
		{
			botY -= 360;
		}

		float normY = Normal.y - 180;  // make this point towards wall rather than away from it
		while ( normY < 0 )  // [0,???]
		{
			normY += 360;
		}
		while ( normY > 360 )  // [0,360]
		{
			normY -= 360;
		}

		float wallY1 = normY - 90;  // [-90,270]
		if ( wallY1 < 0 )  // [0,360]
		{
			wallY1 += 360;
		}

		float wallY2 = normY + 90;  // [90,450]
		if ( wallY2 > 360 )  // [0,360]
		{
			wallY2 -= 360;
		}

		float diffWallY1 = botY - wallY1;  // (-360,360]
		if ( diffWallY1 < -180 )  // [-180,360]
		{
			diffWallY1 += 360;
		}
		else if ( diffWallY1 > 180 )  // [-180,180]
		{
			diffWallY1 -= 360;
		}

		float diffWallY2 = botY - wallY2;  // (-360,360]
		if ( diffWallY2 < -180 )  // [-180,360]
		{
			diffWallY2 += 360;
		}
		else if ( diffWallY2 > 180 )  // [-180,180]
		{
			diffWallY2 -= 360;
		}

		float MaxPossibleTraceDistance = AVOID_AHEAD_DISTANCE + WALL_ALIGN_DISTANCE;
		float ActualTraceDistance = tr.flFraction * MaxPossibleTraceDistance;
		float TweakYawPercent = (MaxPossibleTraceDistance - ActualTraceDistance) / AVOID_AHEAD_DISTANCE;

		if ( ((fabs(diffWallY1) > fabs(diffWallY2)) && GetTurningDirection() == NONE) ||
			 GetTurningDirection() ==
				 LEFT )  // turn the way which takes the least effort if we weren't already turning a certain direction
		{
			SetTurningDirection(LEFT);

			DirectionHeadingAngle.y -=
				(diffWallY1 * TweakYawPercent * RANDOM_FLOAT((1 - YAW_VARIATION), (1 + YAW_VARIATION)));
		}
		else
		{
			SetTurningDirection(RIGHT);

			DirectionHeadingAngle.y -=
				(diffWallY2 * TweakYawPercent * RANDOM_FLOAT((1 - YAW_VARIATION), (1 + YAW_VARIATION)));
		}

		if ( fabs(botY - DirectionHeadingAngle.y) > (ALLOWED_TURNING_CHANGE * GetMSec() / 1000) )
		{
			if ( (botY - DirectionHeadingAngle.y) < 0 )
			{
				DirectionHeadingAngle.y = botY - (ALLOWED_TURNING_CHANGE * GetMSec() / 1000);
			}
			else
			{
				DirectionHeadingAngle.y = botY + (ALLOWED_TURNING_CHANGE * GetMSec() / 1000);
			}
		}

		while ( DirectionHeadingAngle.y > 180 )  // [-360,180]
		{
			DirectionHeadingAngle.y -= 360;
		}
		while ( DirectionHeadingAngle.y < -180 )  // [-180,180]
		{
			DirectionHeadingAngle.y += 360;
		}
	}
	else
	{
		SetTurningDirection(NONE);
	}

	Vector IdealVelAngle = DirectionHeadingAngle;
	if ( CheckPathObstructedSides(IdealVelAngle, tr, tr2) )
	{
		if ( tr.flFraction > tr2.flFraction )
		{
			DirectionHeadingAngle.y -= RANDOM_FLOAT(1, (ALLOWED_TURNING_CHANGE * GetMSec() / 1000));
		}
		else
		{
			DirectionHeadingAngle.y += RANDOM_FLOAT(1, (ALLOWED_TURNING_CHANGE * GetMSec() / 1000));
		}

		while ( DirectionHeadingAngle.y > 180 )  // [-360,180]
		{
			DirectionHeadingAngle.y -= 360;
		}
		while ( DirectionHeadingAngle.y < -180 )  // [-180,180]
		{
			DirectionHeadingAngle.y += 360;
		}
	}

	return DirectionHeadingAngle;
}

///////////////////////////////////////////////////////////////////////////////
// SteerGroupAlign
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerGroupAlign(void)
{
	int PlayersInLocality = 0;
	float DistanceToPlayer;
	Vector LocalityVelocitySum = Vector(0, 0, 0);

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity* pPlayer = UTIL_PlayerByIndex(i);

		if ( pPlayer && pPlayer != this && pPlayer->IsAlive() && CheckVisible(pPlayer) )
		{
			DistanceToPlayer = (Vector(pev->origin) - Vector(pPlayer->pev->origin)).Length();

			if ( DistanceToPlayer < GROUPING_DISTANCE )
			{
				PlayersInLocality += 1;

				LocalityVelocitySum = LocalityVelocitySum + pPlayer->pev->velocity;
			}
		}
	}

	Vector AvgLocalVelocity = LocalityVelocitySum / static_cast<float>(PlayersInLocality);
	;

	SetDesiredVelocity(AvgLocalVelocity);
}

///////////////////////////////////////////////////////////////////////////////
// SteerGroupCohesion
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerGroupCohesion(void)
{
	int PlayersInLocality = 0;
	float DistanceToPlayer;
	Vector LocalityVecSum = Vector(0, 0, 0);

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity* pPlayer = UTIL_PlayerByIndex(i);

		if ( pPlayer && pPlayer != this && pPlayer->IsAlive() && CheckVisible(pPlayer) )
		{
			DistanceToPlayer = (Vector(pev->origin) - Vector(pPlayer->pev->origin)).Length();

			if ( DistanceToPlayer < GROUPING_DISTANCE )
			{
				PlayersInLocality += 1;

				LocalityVecSum = LocalityVecSum + pPlayer->pev->origin;
			}
		}
	}

	Vector AvgLocalPosition = LocalityVecSum / static_cast<float>(PlayersInLocality);

	SteerPrimitiveSeek(AvgLocalPosition);
}

///////////////////////////////////////////////////////////////////////////////
// SteerGroupEscort
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerGroupEscort(CBaseEntity* escortTarget)
{
	//	Vector lead_offset = ( escortTarget->pev->velocity ).Normalize() * ESCORT_DISTANCE;
	Vector lead_offset = Vector(0, 0, 0);

	SteerPrimitivePursue(escortTarget, lead_offset);
}

///////////////////////////////////////////////////////////////////////////////
// SteerGroupFlock
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerGroupFlock(void)
{
	SteerSafeGroupAlign();

	SteerSafeGroupCohesion();

	SteerSafeGroupSeparation();
}

///////////////////////////////////////////////////////////////////////////////
// SteerGroupFollow
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerGroupFollow(CBaseEntity* followTarget)
{
	//	Vector follow_offset = ( followTarget->pev->velocity ).Normalize() * FOLLOW_DISTANCE;
	Vector follow_offset = Vector(0, 0, 0);

	SteerPrimitivePursue(followTarget, follow_offset);
}

///////////////////////////////////////////////////////////////////////////////
// SteerGroupSeparation
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerGroupSeparation(void)
{
	Vector VectorToPlayer;
	float DistanceToPlayer;
	Vector DestinationVector = Vector(0, 0, 0);

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity* pPlayer = UTIL_PlayerByIndex(i);

		if ( pPlayer && pPlayer != this && pPlayer->IsAlive() && CheckVisible(pPlayer) )
		{
			VectorSubtract(pev->origin, pPlayer->pev->origin, VectorToPlayer);
			DistanceToPlayer = VectorToPlayer.Length();

			if ( DistanceToPlayer < GROUPING_DISTANCE )
			{
				VectorToPlayer = VectorToPlayer.Normalize();
				DestinationVector = DestinationVector + (VectorToPlayer / (DistanceToPlayer * DistanceToPlayer));
			}
		}
	}

	SteerPrimitiveSeek(DestinationVector);
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveEvade
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveEvade(CBaseEntity* evadeTarget)
{
	float distance = (Vector(evadeTarget->pev->origin) - Vector(pev->origin)).Length();
	SteerPrimitiveFlee((Vector(evadeTarget->pev->velocity) * distance) + Vector(evadeTarget->pev->origin));
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveFlee for seeking entities
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveFlee(CBaseEntity* fleeTarget)
{
	SteerPrimitiveFlee(fleeTarget->pev->origin);
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveFlee for seeking vectors (i.e. target origin)
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveFlee(const Vector& fleeTarget)
{
	//	SetDesiredVelocity( ( fleeTarget - pev->origin ).Normalize() * GetServerMaxSpeed() );
	SetDesiredVelocity((Vector(pev->origin) - fleeTarget).Normalize() * GetServerMaxSpeed());
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveOnLadder
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveOnLadder(void)
{
	if ( GetEnemy() != NULL )  // we don't want to be on a ladder during combat
	{
		pev->button |= IN_JUMP;
		SetMoveVertical(GetServerMaxSpeed());  // HACK
	}
	else
	{
		if ( GetMovedDistance() <= 1 )
		{
			if ( GetGoUpOnLadder() )
			{
				SetGoUpOnLadder(FALSE);
			}
			else
			{
				SetGoUpOnLadder(TRUE);
			}
		}

		if ( GetGoUpOnLadder() )
		{
			pev->button |= IN_FORWARD;
		}
		else
		{
			pev->button |= IN_BACK;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitivePursue
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitivePursue(CBaseEntity* pursueTarget, Vector offset)
{
	float distance = (Vector(pursueTarget->pev->origin) - Vector(pev->origin)).Length();
	SteerPrimitiveSeek((Vector(pursueTarget->pev->velocity) * distance) + Vector(pursueTarget->pev->origin) + offset);
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveSeek for seeking entities
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveSeek(CBaseEntity* seekTarget)
{
	SteerPrimitiveSeek(seekTarget->pev->origin);
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveSeek for seeking vectors (i.e. target origin)
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveSeek(const Vector& seekTarget)
{
	SetDesiredVelocity((seekTarget - Vector(pev->origin)).Normalize() * GetServerMaxSpeed());
}

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveWander
///////////////////////////////////////////////////////////////////////////////

// 05-Sept-01: Scott: Rewritten

void CBaseBot::SteerPrimitiveWander(float WanderRate)
{
	float TrueWanderRate = WanderRate * GetMSec() / 1000;

	Vector DirectionHeadingAngle = UTIL_VecToAngles(GetDesiredVelocity());

	UTIL_MakeVectors(DirectionHeadingAngle);

	DirectionHeadingAngle.y += RANDOM_FLOAT(-TrueWanderRate, TrueWanderRate);

	UTIL_MakeVectors(DirectionHeadingAngle);

	SetDesiredVelocity(gpGlobals->v_forward);

	SetDesiredVelocity(GetDesiredVelocity().Normalize() * GetServerMaxSpeed());
}

// 05-Sept-01: end

/*
void CBaseBot::SteerPrimitiveWander( float WanderRate )
{
	float TrueWanderRate = WanderRate / 100;

	Vector AdjVec = Vector( RANDOM_FLOAT(-TrueWanderRate,TrueWanderRate),
							RANDOM_FLOAT(-TrueWanderRate,TrueWanderRate),
							RANDOM_FLOAT(-TrueWanderRate,TrueWanderRate) );

	SetDesiredVelocity( GetDesiredVelocity().Normalize() + AdjVec );

	SetDesiredVelocity( GetDesiredVelocity().Normalize() * GetServerMaxSpeed() );
}
*/

///////////////////////////////////////////////////////////////////////////////
// SteerPrimitiveWanderImproved
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerPrimitiveWanderImproved(void)
{
	DIRECTION moveThisWay = ActionChooseDirection();

	int tryCounter = 0;
	// Scott:  With Change to enum DIRECTION in bot.h, logic has been simplified to:
	while ( ((GetRoamDirection() & moveThisWay) == 0) &&
			(tryCounter < 10) )  // 30-July-2001: correction && instead of ||
	{
		moveThisWay = ActionChooseDirection();
		tryCounter++;
	}

	if ( UNKNOWN == moveThisWay )
	{
		SteerPrimitiveWander();
	}
	else
	{
		// 30-July-2001: Simplify logic
		Vector moveThisWayVelocity = Vector(0, 0, 0);

		if ( NORTH & moveThisWay )
		{
			moveThisWayVelocity = moveThisWayVelocity + Vector(1, 0, 0);
		}
		else if ( SOUTH & moveThisWay )
		{
			moveThisWayVelocity = moveThisWayVelocity + Vector(-1, 0, 0);
		}

		if ( WEST & moveThisWay )
		{
			moveThisWayVelocity = moveThisWayVelocity + Vector(0, 1, 0);
		}
		else if ( EAST & moveThisWay )
		{
			moveThisWayVelocity = moveThisWayVelocity + Vector(0, -1, 0);
		}

		// 30-July-2001: end
		// 05-Sept-2001: Fix for zero movement

		if ( moveThisWayVelocity.Length() > 0.0 )
		{
			moveThisWayVelocity = moveThisWayVelocity.Normalize() * GetServerMaxSpeed();

			SetRoamDirection(moveThisWay);

			SetDesiredVelocity(moveThisWayVelocity);
		}
		else
			SteerPrimitiveWander();
		// 05-Sept-2001: end
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeGroupAlign
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeGroupAlign(STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerGroupAlign();

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerGroupAlign();

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeGroupCohesion
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeGroupCohesion(STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerGroupCohesion();

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerGroupCohesion();

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeGroupEscort
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeGroupEscort(CBaseEntity* escortTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerGroupEscort(escortTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerGroupEscort(escortTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeGroupFlock
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeGroupFlock(STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerGroupFlock();

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerGroupFlock();

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeGroupFollow
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeGroupFollow(CBaseEntity* followTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerGroupEscort(followTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerGroupEscort(followTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeGroupSeparation
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeGroupSeparation(STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerGroupSeparation();

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerGroupSeparation();

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeEvade
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeEvade(CBaseEntity* evadeTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveEvade(evadeTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveEvade(evadeTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeFlee
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeFlee(CBaseEntity* fleeTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveFlee(fleeTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveFlee(fleeTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeFlee
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeFlee(const Vector& fleeTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveFlee(fleeTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveFlee(fleeTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeOnLadder
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeOnLadder(STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveOnLadder();

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveOnLadder();

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafePursue
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafePursue(CBaseEntity* pursueTarget, STEER precedence, Vector offset)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitivePursue(pursueTarget, offset);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitivePursue(pursueTarget, offset);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeSeek
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeSeek(CBaseEntity* seekTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveSeek(seekTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveSeek(seekTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeSeek
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeSeek(const Vector& seekTarget, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveSeek(seekTarget);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveSeek(seekTarget);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeWander
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeWander(float WanderRate, STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveWander(WanderRate);

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveWander(WanderRate);

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}

///////////////////////////////////////////////////////////////////////////////
// SteerSafeWanderImproved
///////////////////////////////////////////////////////////////////////////////

void CBaseBot::SteerSafeWanderImproved(STEER precedence)
{
	if ( precedence > GetSteerCallPrecedence() )
	{
		SteerPrimitiveWanderImproved();

		SetSteerCallCounter(1);
		SetSteerCallPrecedence(precedence);
	}
	else if ( precedence == GetSteerCallPrecedence() )
	{
		Vector PrevDesiredVelocity = GetDesiredVelocity();
		Vector NewDesiredVelocity = PrevDesiredVelocity * static_cast<float>(GetSteerCallCounter());

		SteerPrimitiveWanderImproved();

		NewDesiredVelocity = NewDesiredVelocity + GetDesiredVelocity();
		SetSteerCallCounter(GetSteerCallCounter() + 1);
		NewDesiredVelocity = NewDesiredVelocity / static_cast<float>(GetSteerCallCounter());
		SetDesiredVelocity(NewDesiredVelocity);
	}
}
