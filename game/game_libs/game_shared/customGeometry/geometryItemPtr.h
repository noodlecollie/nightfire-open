#pragma once

#include <memory>

namespace CustomGeometry
{
	// We have to separate these so that we can use this class
	// in the client without weird mysterious errors happening
	// because of other includes. I hate you, Half Life devs.
	class CGeometryItem;
	typedef std::shared_ptr<CGeometryItem> GeometryItemPtr_t;
}  // namespace CustomGeometry
