#pragma once

#include <cstddef>
#include <cstdint>
#include "Components.h"

namespace MDLv14
{
	static inline size_t LODFlagsToLODLevels(uint32_t value)
	{
		size_t temp = 0;

		if ( value & LevelOfDetailFlag_TimesOne )
		{
			temp = 1;
		}

		if ( value & LevelOfDetailFlag_TimesTwo )
		{
			temp = 2;
		}

		size_t output = 0;

		if ( value & LevelOfDetailFlag_PlusOne )
		{
			output += 1 * temp;
		}

		if ( value & LevelOfDetailFlag_PlusTwo )
		{
			output += 2 * temp;
		}

		if ( value & LevelOfDetailFlag_PlusFour )
		{
			output += 4 * temp;
		}

		if ( value & LevelOfDetailFlag_TimesFive )
		{
			output *= 5;
		}

		return output;
	}
}  // namespace MDLv14
