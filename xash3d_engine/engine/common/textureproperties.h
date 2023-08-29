#ifndef TEXTURE_PROPERTIES_H
#define TEXTURE_PROPERTIES_H

#include "common.h"
#include "studio.h"
#include "CommonUtils/xash3d_mathlib.h"
#include "const.h"
#include "com_model.h"

#define TEXTURE_PROPERTY_SURFACEPROP "surfaceprop"

qboolean TextureProperties_Parse(texture_t* texture, const char* key, const char* value);

#endif  // TEXTURE_PROPERTIES_H
