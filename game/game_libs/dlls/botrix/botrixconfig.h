#pragma once

#include "botrix/types.h"

#include <good/ini_file.h>

namespace WeaponAtts
{
	struct WACollection;
}

//****************************************************************************************************************
/// Class for storing configuration in ini file format.
//****************************************************************************************************************
class CConfiguration
{
public:
	static TModId LoadProgrammatic();
	static void RefreshWeaponConfig();

private:
	static void LoadWeapon(const WeaponAtts::WACollection& atts);
};
