#include "Conversions/ConversionHelpers.h"
#include "Conversions/Activity.h"

namespace Conversion
{
	static const Pair<CommonTypes::MotionFlag> COMMONTYPES_MOTION_FLAG_STRING_TO_VALUE[] = {
#define LIST_ITEM(constant, value) {#constant, CommonTypes::constant},
		COMMONTYPES_MOTION_FLAG_LIST
#undef LIST_ITEM
	};

	const char* MotionFlagName(CommonTypes::MotionFlag flag)
	{
		const Pair<CommonTypes::MotionFlag>* pair =
			FindByValue(COMMONTYPES_MOTION_FLAG_STRING_TO_VALUE, flag);

		if ( !pair )
		{
			throw std::invalid_argument("Unknown motion flag value " + std::to_string(flag));
		}

		return pair->first;
	}

	bool IsValidMotionFlag(int32_t value)
	{
		return FindByValue(COMMONTYPES_MOTION_FLAG_STRING_TO_VALUE, value) != nullptr;
	}
}  // namespace Conversion
