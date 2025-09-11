#include "botrix/waypoint_navigator.h"
#include "botrix/server_plugin.h"
#include "botrix/engine_util.h"
#include "botrix/botrixmod.h"
#include "botrix/parameter_vars.h"

//----------------------------------------------------------------------------------------------------------------
TPathDrawFlags CWaypointNavigator::iPathDrawFlags = FPathDrawNone;

//----------------------------------------------------------------------------------------------------------------
bool CWaypointNavigator::SearchSetup(
	TWaypointId iFrom,
	TWaypointId iTo,
	good::vector<TAreaId> const& aAvoidAreas,
	int iMaxWaypointsInLoop
)
{
	BASSERT(CWaypoint::IsValid(iFrom) && CWaypoint::IsValid(iTo), return false);

	m_cAstar.set_graph(CWaypoints::m_cGraph, 1024);
	m_cAstar.setup_search(iFrom, iTo, CCanUseWaypoint(aAvoidAreas), iMaxWaypointsInLoop);

	m_bSearchStarted = true;
	m_bSearchEnded = false;
	m_iPathIndex = -1;

	return true;
}

//----------------------------------------------------------------------------------------------------------------
void CWaypointNavigator::DrawPath(unsigned char r, unsigned char g, unsigned char b, const Vector& vOrigin)
{
	if ( (iPathDrawFlags == FPathDrawNone) || (CBotrixServerPlugin::GetTime() < m_fNextDrawTime) ||
		 !m_cAstar.has_path() || (m_iPathIndex == 0) || (m_iPathIndex > (int)m_cAstar.path().size()) )
	{
		return;
	}

	float fDrawTime = 0.1f;  // Bug when drawing line for more time than 0.1 seconds.
	m_fNextDrawTime = CBotrixServerPlugin::GetTime() + fDrawTime;

	int index = m_iPathIndex - 1;  // Bot is currently moving to waypoint in path[m_iPathIndex-1].
	astar_t::path_t& path = m_cAstar.path();

	CWaypoints::WaypointNode& first = CWaypoints::m_cGraph[path[index]];

	// Draw waypoints paths lower (can't see it when spectating bot, because its  height is at eye level).
	Vector v1(vOrigin), v2(first.vertex.vOrigin);
	v1.z -= CBotrixParameterVars::PLAYER_EYE / 4.0f;
	v2.z -= CBotrixParameterVars::PLAYER_EYE / 4.0f;

	if ( FLAG_ALL_SET_OR_0(FPathDrawBeam, iPathDrawFlags) )
	{
		CBotrixEngineUtil::DrawBeam(v1, v2, 4, fDrawTime, r, g, b);
	}

	if ( FLAG_ALL_SET_OR_0(FPathDrawLine, iPathDrawFlags) )
	{
		CBotrixEngineUtil::DrawLine(v1, v2, fDrawTime, r, g, b);
	}

	for ( int i = index; i < m_cAstar.path().size() - 1; ++i )
	{
		v1 = CWaypoints::Get(path[i]).vOrigin;
		v2 = CWaypoints::Get(path[i + 1]).vOrigin;
		v1.z -= CBotrixParameterVars::PLAYER_EYE / 4.0f;
		v2.z -= CBotrixParameterVars::PLAYER_EYE / 4.0f;

		if ( FLAG_ALL_SET_OR_0(FPathDrawBeam, iPathDrawFlags) )
		{
			CBotrixEngineUtil::DrawBeam(v1, v2, 4, fDrawTime, r, g, b);
		}

		if ( FLAG_ALL_SET_OR_0(FPathDrawLine, iPathDrawFlags) )
		{
			CBotrixEngineUtil::DrawLine(v1, v2, fDrawTime, r, g, b);
		}
	}
}
