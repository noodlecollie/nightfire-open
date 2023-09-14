#ifndef TEXTURE_PROPERTIES_H
#define TEXTURE_PROPERTIES_H

#include "EnginePublicAPI/com_model.h"

#define TEXTURE_PROPERTY_SURFACEPROP "surfaceprop"

qboolean TextureProperties_Parse(texture_t* texture, const char* key, const char* value);

#endif  // TEXTURE_PROPERTIES_H
