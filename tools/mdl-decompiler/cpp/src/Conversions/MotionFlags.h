#pragma once

#include <cstdint>
#include "Common/Types.h"

namespace Conversion
{
	const char* MotionFlagName(CommonTypes::MotionFlag flag);
	const char* MotionFlagShortName(CommonTypes::MotionFlag flag);
	bool IsValidMotionFlag(int32_t value);
}
