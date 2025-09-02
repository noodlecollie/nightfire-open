#pragma once

#include <cstddef>
#include <limits>

namespace CustomGeometry
{
	static constexpr const char* CUSTOM_GEOMETRY_MESSAGE_NAME = "DrawCustomGeometry";
	static constexpr size_t MAX_POINTS_PER_MSG = std::numeric_limits<uint8_t>::max();
	static constexpr size_t MAX_INDICES_PER_MSG = std::numeric_limits<uint8_t>::max();

	// This is only a guideline, since the WRITE_STRING() function may convert escaped
	// characters and shorten the actual transmitted string. For future: evaluate whether
	// we need a custom implementation for writing strings, and to potentially move
	// the escaping code to before the string is sent.
	static constexpr size_t MAX_TEXT_CHARS_PER_MSG = 200;
	static constexpr int MIN_TEXT_LINE_OFFSET = -10;
	static constexpr int MAX_TEXT_LINE_OFFSET = 10;

	// For grouping geometry by purpose, eg. visualising hitboxes, drawing overlays, etc.
	// When sending a "clear" message, only the geometry from the specified category is cleared.
	// Values must fit in a uint8_t.
#define CATEGORY_LIST \
	LIST_ITEM(None = 0, "None") /* Sending this on a clear message clears all geometry. */ \
	LIST_ITEM(General, "General") \
	LIST_ITEM(DebugPlayerBounds, "Player Bounds") \
	LIST_ITEM(HitboxDebugging, "Hitbox Debugging") \
	LIST_ITEM(HitscanEvents, "Hitscan Events") /* TODO: Should this be client only? */ \
	LIST_ITEM(RoninDebugging, "Ronin Debugging") \
	LIST_ITEM(WaypointVisualisation, "Waypoint Visualisation")

#define LIST_ITEM(value, name) value,
	enum class Category
	{
		CATEGORY_LIST CategoryCount
	};
#undef LIST_ITEM

	static constexpr size_t CATEGORY_COUNT = static_cast<size_t>(Category::CategoryCount);
	const char* CategoryName(Category category);

	// Values must fit in a uint8_t;
	enum class DrawType
	{
		None = 0,  // For clearing previously drawn geometry.
		Lines,
		TriangleFan,
		TriangleStrip,
		Triangles,
		Text
	};
}  // namespace CustomGeometry
