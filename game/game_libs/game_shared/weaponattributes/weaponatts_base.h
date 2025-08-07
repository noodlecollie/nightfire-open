#pragma once

#include "debug_assert.h"

namespace WeaponAtts
{
	struct WABase
	{
		// Override to validate specific members of the class using assertions.
		virtual void Validate() const
		{
		}
	};
}  // namespace WeaponAtts
