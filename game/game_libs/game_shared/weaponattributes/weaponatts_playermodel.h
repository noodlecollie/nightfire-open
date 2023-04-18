#pragma once

#include "weaponatts_base.h"

namespace WeaponAtts
{
	struct WAPlayerModel : public WABase
	{
		const char* PlayerModelName = nullptr;
		const char* WorldModelName = nullptr;
		const char* PlayerAnimExtension = nullptr;
	};
}
