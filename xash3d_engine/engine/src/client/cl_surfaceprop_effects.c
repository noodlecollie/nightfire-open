#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/com_model.h"
#include "client/cl_surfaceprop_effects.h"
#include "EnginePublicAPI/surfaceprops.h"

qboolean R_SurfaceImpactCreatesSparks(SurfaceProp surfaceProp)
{
	switch ( surfaceProp )
	{
		case SurfaceProp_Metal:
		case SurfaceProp_MetalGrate:
		case SurfaceProp_Gold:
		case SurfaceProp_Computer:
		case SurfaceProp_Rock:
		case SurfaceProp_Concrete:
		{
			return true;
		}

		default:
		{
			return false;
		}
	}
}
