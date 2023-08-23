#pragma once

#include <memory>
#include "weaponattributes/weaponatts_ammobasedattack.h"

class CBaseCrosshairRenderer;

std::shared_ptr<CBaseCrosshairRenderer> GetCrosshairRenderer(WeaponAtts::CrosshairStyle style);
