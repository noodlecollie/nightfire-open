#pragma once

#include "customGeometry/constructors/baseConstructor.h"
#include "customGeometry/primitiveDefs.h"

namespace CustomGeometry
{
	class CWaypointMarkerConstructor : public CBaseConstructor
	{
	public:
		bool IsValid() const override;
		GeometryItemPtr_t Construct() const override;

		const WaypointMarkerPrimitive& Waypoint() const;
		void SetWaypoint(const WaypointMarkerPrimitive& waypoint);

	private:
		WaypointMarkerPrimitive m_Waypoint {};
	};
}  // namespace CustomGeometry
