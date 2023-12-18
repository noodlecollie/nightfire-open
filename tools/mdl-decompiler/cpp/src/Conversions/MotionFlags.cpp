#include <cassert>
#include <cstring>
#include "Conversions/ConversionHelpers.h"
#include "Conversions/Activity.h"
#include "Utils.h"

namespace Conversion
{
	static const Pair<CommonTypes::MotionFlag> COMMONTYPES_MOTION_FLAG_STRING_TO_VALUE[] = {
#define LIST_ITEM(constant, value) {#constant, CommonTypes::constant},
		COMMONTYPES_MOTION_FLAG_LIST
#undef LIST_ITEM
	};

	const char* MotionFlagName(CommonTypes::MotionFlag flag)
	{
		const Pair<CommonTypes::MotionFlag>* pair = FindByValue(COMMONTYPES_MOTION_FLAG_STRING_TO_VALUE, flag);

		if ( !pair )
		{
			throw std::invalid_argument("Unknown motion flag value " + std::to_string(flag));
		}

		return pair->first;
	}

	const char* MotionFlagShortName(CommonTypes::MotionFlag flag)
	{
#define MOTIONFLAG_PREFIX "MotionFlag_"
		static constexpr size_t PREFIX_LENGTH = LiteralStrLen(MOTIONFLAG_PREFIX);
		static_assert(PREFIX_LENGTH == 11, "Unexpected MotionFlag prefix length");

		const char* name = MotionFlagName(flag);

		assert(
			strlen(name) > PREFIX_LENGTH &&
			std::string(name).substr(0, PREFIX_LENGTH) == std::string(MOTIONFLAG_PREFIX));

		return name + PREFIX_LENGTH;
#undef MOTIONFLAG_PREFIX
	}

	bool IsValidMotionFlag(int32_t value)
	{
		return FindByValue(COMMONTYPES_MOTION_FLAG_STRING_TO_VALUE, value) != nullptr;
	}
}  // namespace Conversion
