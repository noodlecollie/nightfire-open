#pragma once

#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	enum class RenderType
	{
		Geometry,
		Text
	};

	void Initialise();
	void VidInit();
	void RenderAllGeometry(RenderType renderType);
	void RenderAdHocGeometry(const GeometryItemPtr_t& item);
	void ClearAllGeometry();
}  // namespace CustomGeometry
