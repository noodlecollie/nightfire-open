#include "client/cl_uigl.h"
#include "common/common.h"
#include "ref_common.h"
#include "EnginePublicAPI/ref_params.h"

void CL_UIGL_BeginFrame(int viewportX, int viewportY, int viewportWidth, int viewportHeight)
{
	if ( !ref.dllUiFuncs.beginFrame )
	{
		return;
	}

	struct ref_viewpass_s viewPass;
	memset(&viewPass, 0, sizeof(viewPass));

	viewPass.flags |= RF_ONLY_CLIENTDRAW;
	viewPass.viewport[0] = viewportX;
	viewPass.viewport[1] = viewportY;
	viewPass.viewport[2] = viewportWidth;
	viewPass.viewport[3] = viewportHeight;

	ref.dllUiFuncs.beginFrame(&viewPass);
}

void CL_UIGL_EndFrame()
{
	if ( !ref.dllUiFuncs.endFrame )
	{
		return;
	}

	ref.dllUiFuncs.endFrame();
}
