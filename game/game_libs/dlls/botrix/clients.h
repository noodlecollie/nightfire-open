#pragma once

#include "botrix/players.h"

// NFTODO: Rename to CBotrixClient
class CClient : public CPlayer
{
public:  // Methods.
	/// Constructor.
	CClient(edict_t* pPlayer) :
		CPlayer(pPlayer, false),
		iCommandAccessFlags(0),
		iDestinationWaypoint(-1),
		iWaypointDrawFlags(0),
		iPathDrawFlags(0),
		iVisiblesDrawFlags(0),
		iItemTypeFlags(0),
		iItemDrawFlags(0),
		bAutoCreateWaypoints(false),
		bAutoCreatePaths(true),
		bDebuggingEvents(false),
		bLockDestinationWaypoint(false)
	{
	}

	/// Get Steam id of this player.
	const good::string& GetNetworkID()
	{
		return m_sNetworkID;
	}

	/// Returns true, if auto-creating waypoints.
	bool IsAutoCreatingWaypoints()
	{
		return bAutoCreateWaypoints;
	}

	/// Called when client finished connecting with server (becomes active).
	virtual void Activated();

	/// Called each frame. Will draw waypoints for this player.
	virtual void PreThink();

public:  // Members and constants.
	TCommandAccessFlags iCommandAccessFlags = 0;  ///< Access for console commands.

	TWaypointId iDestinationWaypoint = 0;  ///< Path destination (path origin is iCurrentWaypoint).

	TWaypointDrawFlags iWaypointDrawFlags = 0;  ///< Draw waypoint flags for this player.
	TPathDrawFlags iPathDrawFlags = 0;  ///< Draw path flags for this player.
	TPathDrawFlags iVisiblesDrawFlags = 0;  ///< Draw visible waypoints flags for this player.

	TItemTypeFlags iItemTypeFlags = 0;  ///< Items to draw for this player.
	TItemDrawFlags iItemDrawFlags = 0;  ///< Draw item flags for this player.

	bool bAutoCreateWaypoints = false;  ///< Generate automatically new waypoints, if distance is too far.
	bool bAutoCreatePaths = false;  ///< Generate automatically paths for new waypoint.

	bool bDebuggingEvents = false;  ///< Print info when game event is fired.
	bool bLockDestinationWaypoint = false;  ///< Destination waypoint was set by console command.

protected:
	good::string m_sNetworkID;
};
