#ifndef TEXTURE_PROPERTIES_H
#define TEXTURE_PROPERTIES_H

#include "EnginePublicAPI/com_model.h"

#define TEXTURE_PROPERTY_SURFACEPROP "surfaceprop"

// This is primarily for StudioMDL, but here in case we need it.
#define TEXTURE_PROPERTY_MASKED "masked"

qboolean TextureProperties_Parse(texture_t* texture, const char* key, const char* value);

#endif  // TEXTURE_PROPERTIES_H
