#pragma once

#include "weaponatts_base.h"

namespace WeaponAtts
{
	struct WAPlayerModel : public WABase
	{
		const char* PlayerModelName = nullptr;
		const char* WorldModelName = nullptr;
		int WorldModelSkin = 0;
		int WorldModelBody = 0;
		const char* PlayerAnimExtension = nullptr;
	};
}  // namespace WeaponAtts
