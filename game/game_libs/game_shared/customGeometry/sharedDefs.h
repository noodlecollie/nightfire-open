#pragma once

#include <cstddef>

namespace CustomGeometry
{
	static constexpr const char* MESSAGE_NAME = "DrawCustomGeometry";
	static constexpr size_t MAX_POINTS_PER_MSG = 256;
	static constexpr size_t MAX_INDICES_PER_MSG = 256;

	// For grouping geometry by purpose, eg. visualising hitboxes, drawing overlays, etc.
	// When sending a "clear" message, only the geometry from the specified category is cleared.
	// Values must fit in a uint8_t.
#define CATEGORY_LIST \
	LIST_ITEM(None = 0, "None") /* Sending this on a clear message clears all geometry. */ \
	LIST_ITEM(DebugPlayerBounds, "Player Bounds") \
	LIST_ITEM(HitboxDebugging, "Hitbox Debugging") \
	LIST_ITEM(HitscanEvents, "Hitscan Events") /* TODO: Should this be client only? */

#define LIST_ITEM(value, name) value,
	enum class Category
	{
		CATEGORY_LIST
		CategoryCount
	};
#undef LIST_ITEM

	static constexpr size_t CATEGORY_COUNT = static_cast<size_t>(Category::CategoryCount);
	const char* CategoryName(Category category);

	// Values must fit in a uint8_t;
	enum class DrawType
	{
		None = 0,	// For clearing previously drawn geometry.
		Lines,
		TriangleFan,
		TriangleStrip,
		Triangles
	};
}
