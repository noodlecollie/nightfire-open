#pragma once

#include "standard_includes.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CBaseConstructor
	{
	public:
		virtual bool IsValid() const = 0;
		virtual GeometryItemPtr_t Construct() const = 0;
	};
}  // namespace CustomGeometry
