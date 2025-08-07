#include <cstdint>
#include "sharedDefs.h"
#include "coreutil/coreutil.h"

namespace CustomGeometry
{
#define LIST_ITEM(value, name) name,
	static constexpr const char* const CategoryNames[] = {CATEGORY_LIST};
#undef LIST_ITEM

	const char* CategoryName(Category category)
	{
		const uint32_t index = static_cast<uint32_t>(category);
		return index < CATEGORY_COUNT ? CategoryNames[index] : "UNKNOWN";
	}
}  // namespace CustomGeometry
