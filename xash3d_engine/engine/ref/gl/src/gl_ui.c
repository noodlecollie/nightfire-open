#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "gl_ui.h"
#include "gl_local.h"
#include "gl_export.h"

void GL_UI_BeginFrame(const struct ref_viewpass_s* rvp)
{
	glConfig.softwareGammaUpdate = false;
	R_CheckGamma();
	R_Set2DMode(true);
	R_SetupRefParams(rvp);

	// Redo projection matrix according to how RmlUi does it.
	pglMatrixMode(GL_PROJECTION);
	pglLoadIdentity();
	pglOrtho(RI.viewport[0], RI.viewport[2], RI.viewport[3], RI.viewport[1], -10000, 10000);

	pglMatrixMode(GL_TEXTURE);
	pglLoadIdentity();

	pglEnableClientState(GL_VERTEX_ARRAY);
	pglEnableClientState(GL_COLOR_ARRAY);
	pglDisableClientState(GL_TEXTURE_COORD_ARRAY);

	pglEnable(GL_BLEND);
	pglBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	pglEnable(GL_STENCIL_TEST);
	pglStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	pglStencilMask(0xFFFFFFFF);
	pglStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void GL_UI_EndFrame(void)
{
	// TODO: Is this needed, or does the engine end the frame for us?
	R_EndFrame();
}
