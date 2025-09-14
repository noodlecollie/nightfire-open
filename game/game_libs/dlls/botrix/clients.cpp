#include "botrix/clients.h"
#include "botrix/defines.h"
#include "botrix/type2string.h"
#include "botrix/waypoint.h"
#include "botrix/item.h"

namespace Botrix
{
	void CBotrixClient::Activated()
	{
		CPlayer::Activated();

		m_sNetworkID = m_PlayerInfo.GetNetworkIDString();

		if ( m_pEdict == CBotrixServerPlugin::GetListenServerClient() )
		{
			iCommandAccessFlags = FCommandAccessAll;
		}
		else
		{
			iCommandAccessFlags = 0;
		}

		BLOG_I(
			"User connected %s (network ID \"%s\"), access: %s.",
			GetName(),
			m_sNetworkID.c_str(),
			CTypeToString::AccessFlagsToString(iCommandAccessFlags).c_str()
		);

		iWaypointDrawFlags = FWaypointDrawNone;
		iPathDrawFlags = FPathDrawNone;

		bAutoCreatePaths = FLAG_ALL_SET_OR_0(FCommandAccessWaypoint, iCommandAccessFlags);
		bAutoCreateWaypoints = false;

		iItemDrawFlags = FItemDrawAll;
		iItemTypeFlags = 0;

		iDestinationWaypoint = EWaypointIdInvalid;

#if defined(DEBUG) || defined(_DEBUG)
		bDebuggingEvents = FLAG_ALL_SET_OR_0(FCommandAccessConfig, iCommandAccessFlags);
#else
		bDebuggingEvents = false;
#endif
	}

	//----------------------------------------------------------------------------------------------------------------
	void CBotrixClient::PreThink()
	{
		// int iLastWaypoint = iCurrentWaypoint;
		CPlayer::PreThink();

		// Client don't have access to waypoint modification.
		if ( FLAG_CLEARED(FCommandAccessWaypoint, iCommandAccessFlags) )
		{
			return;
		}

		// Check if lost waypoint, in that case add new one.
		if ( IsAutoCreatingWaypoints() && m_bAlive &&
			 (!CWaypoint::IsValid(iCurrentWaypoint) ||
			  (CWaypoints::Get(iCurrentWaypoint).vOrigin - GetHead()).LengthSquared() >=
				  SQR(CWaypoint::iDefaultDistance)) )
		{
			// Execute command: botrix waypoint create
			// NFTODO: Is there a better way to do this?
			g_engfuncs.pfnServerCommand("botrix waypoint create\n");

			// Old code auto adding waypoint directly
			// Vector vOrigin( GetHead() );

			//// Add new waypoint, but distance from previous one must not be bigger than iDefaultDistance.
			// if ( CWaypoint::IsValid(iLastWaypoint) )
			//{
			//     CWaypoint& wLast = CWaypoints::Get(iLastWaypoint);
			//     vOrigin -= wLast.vOrigin;
			//     vOrigin.NormalizeInPlace();
			//     vOrigin *= CWaypoint::iDefaultDistance;
			//     vOrigin += wLast.vOrigin;
			// }

			//// Add new waypoint.
			// iCurrentWaypoint = CWaypoints::Add(vOrigin);

			//// Add paths from previous to current.
			// if ( CWaypoint::IsValid(iLastWaypoint) )
			//{
			//     float fHeight = GetPlayerInfo()->GetPlayerMaxs().z - GetPlayerInfo()->GetPlayerMins().z + 1;
			//     bool bIsCrouched = (fHeight < CBotrixParameterVars::PLAYER_HEIGHT);

			//    CWaypoints::CreatePathsWithAutoFlags(iLastWaypoint, iCurrentWaypoint, bIsCrouched, true);
			//    iDestinationWaypoint = iLastWaypoint;
			//}
		}

		// Calculate destination waypoint according to angles. Paths should be drawn.
		if ( !bLockDestinationWaypoint && (iPathDrawFlags != FPathDrawNone) &&
			 (CWaypoints::fNextDrawWaypointsTime >= CBotrixServerPlugin::GetTime()) )
		{
			Vector ang;
			GetEyeAngles(ang);
			iDestinationWaypoint = CWaypoints::GetAimedWaypoint(GetHead(), ang);
		}

		// Draw waypoints.
		CWaypoints::Draw(this);  // TODO: should not draw for admins without rights.

		// Draw entities.
		CItems::Draw(this);
	}
}  // namespace Botrix
