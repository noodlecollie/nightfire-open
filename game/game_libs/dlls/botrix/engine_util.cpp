#include "botrix/engine_util.h"
#include "botrix/botrixmod.h"
#include "botrix/waypoint.h"
#include "botrix/botrixgamerulesinterface.h"
#include "botrix/defines.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "MathLib/utils.h"
#include "weapons.h"
#include "customGeometry/messageWriter.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/constructors/aabboxConstructor.h"
#include "customGeometry/rollingLineMessageWriter.h"

good::TLogLevel CBotrixEngineUtil::iLogLevel = good::ELogLevelInfo;
int CBotrixEngineUtil::iTextTime = 20;  // Time in seconds to show text in CUtil::GetReachableInfoFromTo().
TraceResult CBotrixEngineUtil::m_TraceResult = {
	0,
	0,
	0,
	0,
	-1.0f,  // Fraction - used to signify here that the result has not yet been used
	{0.0f, 0.0f, 0.0f},
	0.0f,
	{0.0f, 0.0f, 0.0f},
	nullptr,
	0,
};
uint8_t CBotrixEngineUtil::pvs[MAX_PVS_DATA_LENGTH];

void CBotrixEngineUtil::Message(good::TLogLevel iLevel, struct edict_s* pEntity, const char* szMsg)
{
	bool shouldSendClientMessage = false;

	if ( pEntity )
	{
		if ( IS_DEDICATED_SERVER() )
		{
			shouldSendClientMessage = true;
		}
		else
		{
			edict_t* listenServerPlayer = nullptr;
			CHalfLifeMultiplay* mpGamerules = dynamic_cast<CHalfLifeMultiplay*>(g_pGameRules);
			CBotrixGameRulesInterface* bgri =
				mpGamerules ? dynamic_cast<CBotrixGameRulesInterface*>(mpGamerules->BotGameRulesInterface()) : nullptr;

			if ( bgri )
			{
				listenServerPlayer = bgri->GetListenServerClient();
			}

			shouldSendClientMessage = listenServerPlayer != pEntity;
		}
	}

	if ( shouldSendClientMessage )
	{
		CLIENT_PRINTF(pEntity, print_console, szMsg);
	}

	if ( iLevel >= iLogLevel )
	{
		ALERT_TYPE alertType = at_console;

		switch ( iLevel )
		{
			case good::ELogLevelTrace:
			case good::ELogLevelDebug:
			{
				alertType = at_aiconsole;
				break;
			}

			case good::ELogLevelInfo:
			{
				alertType = at_console;
				break;
			}

			case good::ELogLevelWarning:
			{
				alertType = at_warning;
				break;
			}

			case good::ELogLevelError:
			{
				alertType = at_error;
				break;
			}

			default:
			{
				break;
			}
		}

		ALERT(alertType, szMsg);
	}
}

void CBotrixEngineUtil::EntityHead(edict_t* pEntity, Vector& v)
{
	if ( !pEntity )
	{
		v = Vector();
		return;
	}

	v = Vector(pEntity->v.origin) + Vector(pEntity->v.view_ofs);
}

void CBotrixEngineUtil::SetPVSForVector(const Vector& v)
{
	const byte* pvsData = g_engfuncs.pfnGetPvsForPoint(v);

	if ( pvsData )
	{
		// This will always be MAX_PVS_DATA_LENGTH bytes long.
		memcpy(pvs, pvsData, MAX_PVS_DATA_LENGTH);
	}
	else
	{
		memset(pvs, 0xFF, MAX_PVS_DATA_LENGTH);
	}
}

bool CBotrixEngineUtil::IsVisiblePVS(const Vector& v)
{
	return g_engfuncs.pfnPointInPvs(v, pvs);
}

bool CBotrixEngineUtil::IsVisible(const Vector& vSrc, const Vector& vDest, TVisibility iVisibility, bool bUsePVS)
{
	if ( bUsePVS )
	{
		SetPVSForVector(vSrc);

		if ( !IsVisiblePVS(vDest) )
		{
			return false;
		}
	}

	// NFTODO: We currently don't have the same trace filtering granularity as
	// the Source engine. We need to implement this in future for Ronin
	// collisions anyway, so this should be updated when that happens.
	TRACE_LINE(vSrc, vDest, iVisibility != EVisibilityBots, nullptr, &m_TraceResult);

	return !TraceHitSomething();
}

bool CBotrixEngineUtil::IsVisible(const Vector& vSrc, edict_t* pDest)
{
	Vector v;
	EntityHead(pDest, v);

	return IsVisible(vSrc, v, EVisibilityBots);
}

bool CBotrixEngineUtil::TraceHitSomething()
{
	return m_TraceResult.flFraction >= 0.0f && m_TraceResult.flFraction < 1.0f;
}

Vector CBotrixEngineUtil::GetHullGroundVec(const Vector& vSrc, struct edict_s* ignoreEnt, int hull)
{
	// NFTODO: Cache these somewhere when the map starts
	Vector hullMins;
	g_engfuncs.pfnGetHullBounds(hull, hullMins, nullptr);

	Vector vDest = vSrc;
	vDest.z = -iHalfMaxMapSize;

	TRACE_HULL(vSrc - Vector(0.0f, 0.0f, hullMins.z), vDest, TRUE, hull, ignoreEnt, &m_TraceResult);

	// This can sometimes happen if the source point is extremely close to the ground.
	// In this case, the point is already on the ground.
	if ( m_TraceResult.fAllSolid )
	{
		return vSrc;
	}

	if ( m_TraceResult.flFraction >= 1.0f )
	{
		return vDest;
	}

	// The returned point is the middle of the hull, so make sure we return the base.
	Vector endPos = m_TraceResult.vecEndPos;
	endPos.z += hullMins.z;

	return endPos;
}

bool CBotrixEngineUtil::RayHitsEntity(edict_t* pEntity, const Vector& vSrc, const Vector& vDest)
{
	g_engfuncs.pfnTraceModel(vSrc, vDest, 0, pEntity, &m_TraceResult);
	return m_TraceResult.flFraction >= 0.95f;
}

TReach CBotrixEngineUtil::GetReachableInfoFromTo(
	TraceDirection direction,
	const Vector& vSrc,
	Vector& vDest,
	bool& bCrouch,
	float fDistanceSqr,
	float fMaxDistanceSqr,
	bool bShowHelp)
{
	using namespace CustomGeometry;

	int color = direction == TraceDirection::EFirstToSecond ? 0xFF0000 : 0x00FF00;
	unsigned char r = GET_3RD_BYTE(color);
	unsigned char g = GET_2ND_BYTE(color);
	unsigned char b = GET_1ST_BYTE(color);

	Vector vOffset =
		direction == TraceDirection::EFirstToSecond ? Vector(-0.25f, -0.25f, 0.0f) : Vector(0.25f, 0.25f, 0.0f);

	if ( fDistanceSqr <= 0.0f )
	{
		fDistanceSqr = (vDest.Make2D() - vSrc.Make2D()).LengthSquared();
	}

	if ( fDistanceSqr > fMaxDistanceSqr )
	{
		return EReachNotReachable;
	}

	if ( !IsVisible(vSrc, vDest, EVisibilityWaypoints) )
	{
		return EReachNotReachable;
	}

	// Check if can swim there first.
	int iSrcContent = g_engfuncs.pfnPointContents(vSrc);
	int iDestContent = g_engfuncs.pfnPointContents(vDest);

	if ( iSrcContent == CONTENTS_WATER && iDestContent == CONTENTS_WATER )
	{
		return EReachReachable;
	}

	// Get all needed vars ready.
	float fPlayerEye = CBotrixMod::GetVar(EModVarPlayerEye);
	float fPlayerEyeCrouched = CBotrixMod::GetVar(EModVarPlayerEyeCrouched);

	Vector vMinZ(0, 0, -iHalfMaxMapSize);

	// Vector vGroundMaxs = CBotrixMod::vPlayerCollisionHullMaxsGround;
	// vGroundMaxs.z = 1.0f;

	// Get ground positions.
	Vector vSrcGround = GetHullGroundVec(vSrc);

	if ( vSrcGround.z <= vMinZ.z )
	{
		return EReachNotReachable;
	}

	Vector vDestGround = GetHullGroundVec(vDest);

	if ( vDestGround.z <= vMinZ.z )
	{
		return EReachNotReachable;
	}

	// Check if take damage at fall.
	if ( vSrcGround.z - vDestGround.z >= CBotrixMod::GetVar(EModVarHeightForFallDamage) )
	{
		return EReachNotReachable;
	}

	// Try to get up if needed.
	float zDiff = vDest.z - vDestGround.z;

	if ( zDiff == 0 )
	{
		return EReachNotReachable;  // Can happens when the vDestGround is inside some solid.
	}

	if ( zDiff != fPlayerEye && zDiff != fPlayerEyeCrouched )
	{
		if ( !bCrouch )
		{
			// Try to stand up.
			vDest.z = vDestGround.z + fPlayerEye;
			HumanHullTrace(vDestGround, vDest);
			bCrouch = TraceHitSomething();
		}

		if ( bCrouch )
		{
			// Try to stand up crouching.
			vDest.z = vDestGround.z + fPlayerEyeCrouched;
			HumanHullTrace(vDestGround, vDest, head_hull);

			if ( TraceHitSomething() )
			{
				return EReachNotReachable;
			}
		}
	}

	// Draw waypoints until ground.
	if ( bShowHelp )
	{
		// TODO: draw from the ground position.
		DrawLine(vSrc + vOffset, vSrcGround + vOffset, static_cast<float>(iTextTime), r, g, b);
		DrawLine(vDest + vOffset, vDestGround + vOffset, static_cast<float>(iTextTime), r, g, b);
	}

	// Start off by assuming the destination is reachable.
	TReach iResult = EReachReachable;

	// Need to trace several times to know if can jump up all obstacles.
	Vector vHit = vSrcGround;
	Vector vDirection = vDestGround - vSrcGround;
	vDirection.z = 0.0f;  // We need only X-Y direction.
	vDirection = vDirection.Normalize();

	Vector vLastStair;
	bool bNeedJump = false;
	bool bHasStair = false;
	int i = 0;

	std::unique_ptr<CRollingLineMessageWriter> helperGeomWriter;
	Vector lastDelta;

	if ( bShowHelp )
	{
		helperGeomWriter.reset(new CRollingLineMessageWriter(Category::WaypointVisualisation));

		helperGeomWriter->BeginGeometry(
			DrawType::Lines,
			(static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 26) | (static_cast<uint32_t>(b) << 8) |
				0x000000FF,
			1.0f,
			static_cast<float>(iTextTime));
	}

	// Keep looping while we have attempts left, and while the hit point hasn't reached the destination.
	for ( ; i < iMaxTraceRaysForReachable && !EqualVectors(vHit, vDestGround); ++i )
	{
		// Begin from where we last hit.
		Vector vStart = vHit;

		// Trace from hit point to the floor.
		TReach iReach = CanPassOrJump(vHit, vDirection);

		if ( bShowHelp )
		{
			helperGeomWriter->AddLine(vStart + vOffset, vHit + vOffset);
		}

		switch ( iReach )
		{
			case EReachNotReachable:
			{
				if ( bShowHelp )
				{
					DrawTextAtLocation(vHit, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "High jump");
				}

				return EReachNotReachable;
			}

			case EReachNeedJump:
			{
				if ( bNeedJump )
				{
					if ( bShowHelp )
					{
						DrawTextAtLocation(vHit, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "2 jumps");
					}

					return EReachNotReachable;
				}

				bNeedJump = true;
				break;
			}

			case EReachStairs:
			{
				float fDistSqr = (vHit.Make2D() - vLastStair.Make2D()).LengthSquared();

				if ( bHasStair && fDistSqr <= 9 )  // 3 units at least
				{
					if ( bShowHelp )
					{
						DrawTextAtLocation(vHit, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "Slope");
					}

					return EReachNotReachable;
				}

				bHasStair = true;
				vLastStair = vStart;
				break;
			}

			case EReachReachable:
			{
				break;
			}

			default:
			{
				GoodAssert(false);
				break;
			}
		}
	}

	if ( bShowHelp )
	{
		helperGeomWriter->Finalise();
		helperGeomWriter.reset();
	}

	// Set text position.
	Vector vText = (vSrcGround + vDestGround) / 2;
	vText.z += direction == TraceDirection::EFirstToSecond ? 20 : 10;

	if ( i == iMaxTraceRaysForReachable )
	{
		// We exhausted all our tries without our hit point reaching
		// the destination, so assume not reachable.
		iResult = EReachNotReachable;
	}

	switch ( iResult )
	{
		case EReachReachable:
		{
			if ( bNeedJump )
			{
				iResult = EReachNeedJump;

				if ( bShowHelp )
				{
					DrawTextAtLocation(vText, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "Jump");
				}
			}
			else if ( bShowHelp )
			{
				DrawTextAtLocation(vText, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "Walk");
			}

			break;
		}

		case EReachFallDamage:
		{
			if ( bShowHelp )
			{
				DrawTextAtLocation(vText, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "Fall");
			}

			break;
		}

		case EReachNotReachable:
		{
			if ( bShowHelp )
			{
				DrawTextAtLocation(vText, 0, static_cast<float>(iTextTime), 0xFF, 0xFF, 0xFF, "High");
			}

			break;
		}

		default:
		{
			GoodAssert(false);
			break;
		}
	}

	return iResult;
}

void CBotrixEngineUtil::NormalizeAngle(float& fAngle)
{
	if ( fAngle < 0.0f )
	{
		fAngle += 360.0f;
	}
	else if ( fAngle >= 360.0f )
	{
		fAngle -= 360.0f;
	}
}

void CBotrixEngineUtil::DeNormalizeAngle(float& fAngle)
{
	if ( fAngle < -180.0f )
	{
		fAngle += 360.0f;
	}
	else if ( fAngle >= 180.0f )
	{
		fAngle -= 360.0f;
	}
}

bool CBotrixEngineUtil::CanPassSlope(float fAngle, float fMaxSlope)
{
	DeNormalizeAngle(fAngle);
	fAngle = fabsf(fAngle);

	if ( fAngle <= 90.0f )
	{
		return fAngle < fMaxSlope;
	}
	else
	{
		return fAngle > 180.0f - fMaxSlope;
	}
}

void CBotrixEngineUtil::GetAngleDifference(Vector const& angOrigin, Vector const& angDestination, Vector& angDiff)
{
	angDiff = angDestination - angOrigin;

	DeNormalizeAngle(angDiff.x);
	DeNormalizeAngle(angDiff.y);
}

Vector CBotrixEngineUtil::GetGroundVec(const Vector& vSrc)
{
	Vector vDest = vSrc;
	vDest.z = -iHalfMaxMapSize;

	TRACE_LINE(vSrc, vDest, TRUE, nullptr, &m_TraceResult);
	return m_TraceResult.vecEndPos;
}

//----------------------------------------------------------------------------------------------------------------
// Returns true if can move forward when standing at vGround performing a jump (normal, with crouch, or maybe just
// walking). At return vGround contains new coord which is where player can get after jump.
//----------------------------------------------------------------------------------------------------------------
TReach CBotrixEngineUtil::CanPassOrJump(Vector& vGround, const Vector& vDirectionInc)
{
	// Try to walk unit.
	Vector vHit = vGround + vDirectionInc;

	HumanHullTrace(vGround, vHit);

	if ( !TraceHitSomething() )
	{
		// Make sure to get the ground, in case we need to fall down off an obstacle.
		vGround = GetHullGroundVec(vHit);
		return EReachReachable;
	}

	if ( !EqualVectors(vGround, m_TraceResult.vecEndPos, 0.01f) )
	{
		// We hit something, but we can walk from the ground to this location.
		vGround = GetHullGroundVec(m_TraceResult.vecEndPos);
		return EReachReachable;
	}

	// We have butted up against an obstacle.
	// Try to walk over (one stair height) to see if we can clear it.
	float fMaxWalkHeight = CBotrixMod::GetVar(EModVarPlayerObstacleToJump);
	Vector vStair = vGround;
	vStair.z += fMaxWalkHeight;
	vHit.z += fMaxWalkHeight;

	HumanHullTrace(vStair, vHit);

	if ( !TraceHitSomething() )
	{
		vStair = vGround;
		vGround = GetHullGroundVec(vHit);

		if ( CanClimbSlope(vStair, vGround) )
		{
			// Check if can climb up without making the stair step.
			HumanHullTrace(vStair, vGround);

			if ( !TraceHitSomething() )
			{
				return EReachReachable;
			}
		}

		return EReachStairs;
	}

	// We can't make it over the obstacle with a step, so try jumping.
	float fJumpCrouched = CBotrixMod::GetVar(EModVarPlayerJumpHeightCrouched);
	vGround.z += fJumpCrouched;
	vHit.z += fJumpCrouched - fMaxWalkHeight;  // We added previously fMaxWalkHeight.

	HumanHullTrace(vGround, vHit);

	if ( !TraceHitSomething() )  // We can stand on vHit after jump.
	{
		vGround = GetHullGroundVec(vHit);
		return EReachNeedJump;
	}

	vGround = GetHullGroundVec(m_TraceResult.vecEndPos);
	// vGround = CUtil::GetHullGroundVec( CUtil::TraceResult().endpos );
	return EReachNotReachable;  // Can't jump over.
}

TReach CBotrixEngineUtil::CanClimbSlope(const Vector& vSrc, const Vector& vDest)
{
	Vector vDiff = vDest - vSrc;
	Vector ang;
	VectorAngles(vDiff, ang);  // Get pitch to know if gradient is too big.

	if ( !CWaypoints::IsAnalyzing() )
	{
		BLOG_T("Slope angle %.2f", ang.x);
	}

	float fSlope = CBotrixMod::GetVar(EModVarSlopeGradientToSlideOff);
	return CanPassSlope(ang.x, fSlope) ? EReachReachable : EReachNotReachable;
}

// Trace a hull between two points on the ground.
// Because the hull trace expects the points provided to be at the centre
// of the hull, we accommodate this here and reset the eventual
// collision
void CBotrixEngineUtil::HumanHullTrace(const Vector& vGround1, const Vector& vGround2, int hull)
{
	Vector hullMins;
	g_engfuncs.pfnGetHullBounds(hull, hullMins, nullptr);

	Vector begin = vGround1;
	begin.z -= hullMins.z;

	Vector end = vGround2;
	end.z -= hullMins.z;

	TRACE_HULL(begin, end, ignore_monsters, hull, nullptr, &m_TraceResult);

	m_TraceResult.vecEndPos[VEC3_Z] += hullMins.z;
}

void CBotrixEngineUtil::DrawBeam(
	const Vector& v1,
	const Vector& v2,
	unsigned char iWidth,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b)
{
	MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
	WRITE_BYTE(TE_BEAMPOINTS);
	WRITE_COORD(v1.x);
	WRITE_COORD(v1.y);
	WRITE_COORD(v1.z);
	WRITE_COORD(v2.x);
	WRITE_COORD(v2.y);
	WRITE_COORD(v2.z);
	WRITE_SHORT(g_sModelIndexLaser);
	WRITE_BYTE(0);  // frame start
	WRITE_BYTE(10);  // framerate
	WRITE_BYTE(static_cast<int>(fDrawTime * 10.0f));  // life, 10ths of a second
	WRITE_BYTE(iWidth);  // width
	WRITE_BYTE(0);  // noise
	WRITE_BYTE(r);  // r, g, b
	WRITE_BYTE(g);  // r, g, b
	WRITE_BYTE(b);  // r, g, b
	WRITE_BYTE(255);  // brightness
	WRITE_BYTE(10);  // speed
	MESSAGE_END();
}

void CBotrixEngineUtil::DrawLine(
	const Vector& v1,
	const Vector& v2,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b)
{
	using namespace CustomGeometry;

	CGeometryItem geom;
	geom.SetDrawType(DrawType::Lines);
	geom.AddLine(v1, v2);
	geom.SetColour((r << 24) | (g << 16) | (b << 8) | 0xFF);
	geom.SetLifetimeSecs(fDrawTime);

	CMessageWriter(Category::WaypointVisualisation).WriteMessage(geom);
}

// NFTODO: This is quite an inefficient way to transmit an AABB,
// since it transmits the full geometry rather than the bounds.
// Potentially add a separate message type for this?
void CBotrixEngineUtil::DrawBox(
	const Vector& vOrigin,
	const Vector& vMins,
	const Vector& vMaxs,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b)
{
	using namespace CustomGeometry;

	CAABBoxConstructor constructor;
	constructor.SetBounds(vOrigin + vMins, vOrigin + vMaxs);

	GeometryItemPtr_t geom = constructor.Construct();

	// May happen if bounds were degenerate.
	if ( geom->GetIndices().IsEmpty() )
	{
		return;
	}

	geom->SetColour((r << 24) | (g << 16) | (b << 8) | 0xFF);
	geom->SetLifetimeSecs(fDrawTime);

	CMessageWriter(Category::WaypointVisualisation).WriteMessage(*geom);
}

void CBotrixEngineUtil::DrawTextAtLocation(
	const Vector& vOrigin,
	int iLine,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	const char* szText)
{
	using namespace CustomGeometry;

	if ( !szText || !(*szText) )
	{
		return;
	}

	CGeometryItem item;
	item.SetDrawType(DrawType::Text);
	item.SetLifetimeSecs(fDrawTime);
	item.SetColour((r << 24) | (g << 16) | (b << 8) | 0xFF);
	item.SetText(vOrigin, CUtlString(szText), iLine);

	CMessageWriter(Category::WaypointVisualisation).WriteMessage(item);
}
