#pragma once

#include "standard_includes.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CAABBoxConstructor
	{
	public:
		bool IsValid() const;

		Vector Min() const;
		Vector Max() const;
		void SetBounds(const Vector& min, const Vector& max);

		GeometryItemPtr_t Construct() const;

	private:
		// On which axes (if any) is the bounding box degenerate?
		// Degenerate on axis = both min and max on this axis are the same.
		enum DegenerateAxisFlag
		{
			Degenerate_XAxis = 1 << 0,
			Degenerate_YAxis = 1 << 1,
			Degenerate_ZAxis = 1 << 2,
			Degenerate_AllAxes = (Degenerate_XAxis | Degenerate_YAxis | Degenerate_ZAxis)
		};

		uint32_t GetDegenerateAxes() const;
		void NormaliseBounds();
		void ConstructDegenerateOnOneAxis(GeometryItemPtr_t& geom, uint32_t degenerateAxis) const;
		void ConstructDegenerateOnTwoAxes(GeometryItemPtr_t& geom, uint32_t validAxis) const;
		void ConstructWithAllAxesValid(GeometryItemPtr_t& geom) const;

		Vector m_vecMin;
		Vector m_vecMax;
	};
}  // namespace CustomGeometry
