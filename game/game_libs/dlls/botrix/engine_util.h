#pragma once

#include <good/log.h>
#include "MathLib/vec3.h"
#include "EnginePublicAPI/bspfile.h"
#include "standard_includes.h"

struct edict_s;

namespace Botrix
{
	/// Useful enum to know if one map position can be reached from another one.
	enum TReachs
	{
		EReachNotReachable = 0,  ///< Something blocks our way.
		EReachReachable,  ///< Path free.
		EReachStairs,  ///< Can walk.
		EReachNeedJump,  ///< Need to jump to get there.
		// EReachNeedCrouchJump,                   ///< Not used, will always jump crouched.
		// EReachNeedCrouch,                       ///< Not used, will check player's height instead.
		EReachFallDamage,  ///< Need to take fall damage to reach destination.
	};
	typedef int TReach;

	/// Enum for flags used in CUtil::IsVisible() function.
	enum TVisibilities
	{
		EVisibilityWorld =
			0,  ///< For nearest waypoints & draw, waypoints visibility table (brush only, excludes props).
		EVisibilityWaypoints,  ///< For waypoints creation - reachable areas (player solid brush only, includes some
							   ///< props).
		EVisibilityBots,  ///< Visibility for bots - shooting (player solid brush only, includes some props).
	};
	typedef int TVisibility;  ///< Flags for CUtil::IsVisible() function.

	class CBotrixEngineUtil
	{
	public:
		enum class TraceDirection
		{
			EFirstToSecond,
			ESecondToFirst
		};

		enum class PositionInHull
		{
			Centre,
			Feet,
			Eye
		};

		static constexpr size_t MAX_PVS_DATA_LENGTH = (MAX_MAP_LEAFS + 7) / 8;

		static good::TLogLevel iLogLevel;  /// Console log level. Info by default.

		/// Print a message to given client (must be called from game thread). If message tag is used then message will
		/// start with "[Botrix] ".
		static void Message(good::TLogLevel iLevel, struct edict_s* pEntity, const char* szMsg);

		/// Get entity head
		static void EntityHead(struct edict_s* pEntity, Vector& v);

		/// Get entity center (for this impl, same as entity head).
		static inline void EntityCenter(struct edict_s* pEntity, Vector& v)
		{
			EntityHead(pEntity, v);
		}

		static bool IsPointTouch3d(const Vector& v1, const Vector& v2, int iSqrDiffZ, int iSqrDiffXY)
		{
			// return v2.DistToSqr(v1) < 40*40;
			float zDiff = v1.z - v2.z;

			if ( SQR(zDiff) >= iSqrDiffZ )  // First check if Z difference is too big.
			{
				return false;
			}

			Vector2D vDiff(v1.x - v2.x, v1.y - v2.y);
			return (vDiff.LengthSquared() <= iSqrDiffXY);
		}

		static void SetPVSForVector(const Vector& v);
		static bool IsVisiblePVS(const Vector& v);
		static bool IsVisible(const Vector& vSrc, const Vector& vDest, TVisibility iVisibility, bool bUsePVS = true);
		static bool IsVisible(const Vector& vSrc, struct edict_s* pDest);
		// BOTRIX_TODO: Move statefulness out of this class
		static bool TraceHitSomething();
		static Vector GetHullGroundVec(
			const Vector& vSrc,
			struct edict_s* ignoreEnt = nullptr,
			int hull = human_hull,
			float startSolidNudge = 0.0f
		);
		static bool RayHitsEntity(edict_t* pDoor, const Vector& vSrc, const Vector& vDest);

		// Traces a human hull down from the specified point to see if it hits the ground.
		// The input vector is adjusted before tracing, because depending on the situation
		// it may represent the player's feet position, their eye position, or the centre
		// of their hull. Knowing this information, we can help avoid the trace starting
		// unnecessarily in a solid piece of geometry.
		static Vector GetHumanHullGroundVec(
			const Vector& vSrc,
			PositionInHull posInHull,
			struct edict_s* ignoreEnt = nullptr,
			float startSolidNudge = 0.0f
		);

		static TReach GetWaypointReachableInfoFromTo(
			TraceDirection direction,
			const Vector& vSrcEyePos,
			Vector& vDestEyePos,
			bool& bCrouch,
			float fDistanceSqr,
			float fMaxDistanceSqr,
			bool bShowHelp = false
		);

		static inline bool EqualVectors(const Vector& v1, const Vector& v2, float fEpsilon = 1.0f)
		{
			return fabsf(v2.x - v1.x) < fEpsilon && fabsf(v2.y - v1.y) < fEpsilon && fabsf(v2.z - v1.z) < fEpsilon;
		}

		static void NormalizeAngle(float& fAngle);
		static void DeNormalizeAngle(float& fAngle);
		static bool CanPassSlope(float fAngle, float fMaxSlope);
		static void GetAngleDifference(Vector const& angOrigin, Vector const& angDestination, Vector& angDiff);
		static Vector GetGroundVec(const Vector& vSrc);
		static void HumanHullTrace(const Vector& vGround1, const Vector& vGround2, int hull = human_hull);

		static void DrawBeam(
			const Vector& v1,
			const Vector& v2,
			unsigned char iWidth,
			float fDrawTime,
			unsigned char r,
			unsigned char g,
			unsigned char b
		);

		static void DrawLine(
			const Vector& v1,
			const Vector& v2,
			float fDrawTime,
			unsigned char r,
			unsigned char g,
			unsigned char b
		);

		static void DrawWireBall(
			const Vector& location,
			float radius,
			float fDrawTime,
			unsigned char r,
			unsigned char g,
			unsigned char b
		);

		static void DrawBox(
			const Vector& vOrigin,
			const Vector& vMins,
			const Vector& vMaxs,
			float fDrawTime,
			unsigned char r,
			unsigned char g,
			unsigned char b
		);

		static void DrawTextAtLocation(
			const Vector& vOrigin,
			int iLine,
			float fDrawTime,
			unsigned char r,
			unsigned char g,
			unsigned char b,
			const char* szText
		);

	public:  // Members.
		static constexpr int iMaxMapSize = 32768;  ///< This is max map size for HL2 (-16384..16383).
		static constexpr int iMaxMapSizeSqr = SQR(iMaxMapSize);  ///< This is square of max map size.
		static constexpr int iHalfMaxMapSize = iMaxMapSize / 2;  ///< Half map size.
		static constexpr int iMaxTraceRaysForReachable = 512;  ///< Max tries to get to the other waypoints.

		static TraceResult m_TraceResult;
		static byte pvs[MAX_PVS_DATA_LENGTH];

	private:
		static TReach CanPassOrJump(Vector& vGround, const Vector& vDirectionInc);
		static TReach CanClimbSlope(const Vector& vSrc, const Vector& vDest);
	};
}  // namespace Botrix
