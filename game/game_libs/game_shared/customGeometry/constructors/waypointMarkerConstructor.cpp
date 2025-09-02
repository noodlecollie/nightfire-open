#include "customGeometry/constructors/waypointMarkerConstructor.h"
#include "hulldefs.h"

namespace CustomGeometry
{
	static constexpr float VIEW_HEIGHT = VEC_HULL_MAX.z + VEC_VIEW.z;
	static constexpr float PRONG_LENGTH = 4.0f;

	bool CWaypointMarkerConstructor::IsValid() const
	{
		return true;
	}

	GeometryItemPtr_t CWaypointMarkerConstructor::Construct() const
	{
		GeometryItemPtr_t item(new CGeometryItem());

		item->SetDrawType(DrawType::Lines);

		item->AddLine(
			m_Waypoint.location + Vector(0.0f, 0.0f, PRONG_LENGTH),
			m_Waypoint.location - Vector(0.0f, 0.0f, VIEW_HEIGHT)
		);

		item->AddLine(
			m_Waypoint.location - Vector(PRONG_LENGTH, 0.0f, 0.0f),
			m_Waypoint.location + Vector(PRONG_LENGTH, 0.0f, 0.0f)
		);

		item->AddLine(
			m_Waypoint.location - Vector(0.0f, PRONG_LENGTH, 0.0f),
			m_Waypoint.location + Vector(0.0f, PRONG_LENGTH, 0.0f)
		);

		return item;
	}

	const WaypointMarkerPrimitive& CWaypointMarkerConstructor::Waypoint() const
	{
		return m_Waypoint;
	}

	void CWaypointMarkerConstructor::SetWaypoint(const WaypointMarkerPrimitive& waypoint)
	{
		m_Waypoint = waypoint;
	}
}  // namespace CustomGeometry
