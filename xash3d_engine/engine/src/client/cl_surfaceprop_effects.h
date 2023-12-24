#pragma once

#include "BuildPlatform/Typedefs.h"

typedef enum _SurfaceProp SurfaceProp;

// We'll probably want to make this into a data table (or flags?) at some point, once we program in more effects.
// For now, we just need this;
qboolean R_SurfaceImpactCreatesSparks(SurfaceProp surfaceProp);
