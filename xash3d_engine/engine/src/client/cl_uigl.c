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

void CL_UIGL_EndFrame(void)
{
	if ( !ref.dllUiFuncs.endFrame )
	{
		return;
	}

	ref.dllUiFuncs.endFrame();
}

void CL_UIGL_Clear(uint32_t colour)
{
	if ( !ref.dllUiFuncs.clear )
	{
		return;
	}

	ref.dllUiFuncs.clear(colour);
}

void CL_UIGL_PushMatrixTranslation(float x, float y, float z)
{
	if ( !ref.dllUiFuncs.pushMatrixTranslation )
	{
		return;
	}

	ref.dllUiFuncs.pushMatrixTranslation(x, y, z);
}

void CL_UIGL_PopMatrix(void)
{
	if ( !ref.dllUiFuncs.popMatrix )
	{
		return;
	}

	ref.dllUiFuncs.popMatrix();
}

void CL_UIGL_PrepareToDrawWithoutTexture(const void* data, int objectSize, size_t positionOffset, size_t colourOffset)
{
	if ( !ref.dllUiFuncs.prepareToDrawWithoutTexture )
	{
		return;
	}

	ASSERT(data);
	ASSERT(objectSize > 0);

	ref.dllUiFuncs.prepareToDrawWithoutTexture(data, objectSize, positionOffset, colourOffset);
}

void CL_UIGL_PrepareToDrawWithTexture(
	uint32_t texture,
	const void* data,
	int objectSize,
	size_t positionOffset,
	size_t colourOffset,
	size_t textureCoOrdOffset
)
{
	if ( !ref.dllUiFuncs.prepareToDrawWithTexture )
	{
		return;
	}

	ASSERT(data);
	ASSERT(objectSize > 0);

	ref.dllUiFuncs
		.prepareToDrawWithTexture(texture, data, objectSize, positionOffset, colourOffset, textureCoOrdOffset);
}

void CL_UIGL_DrawElements(int numIndices, const void* indices)
{
	if ( !ref.dllUiFuncs.drawElements )
	{
		return;
	}

	ASSERT(numIndices > 0);
	ASSERT(indices);

	ref.dllUiFuncs.drawElements(numIndices, indices);
}

void CL_UIGL_SetScissorEnabled(qboolean enabled)
{
	if ( !ref.dllUiFuncs.setScissorEnabled )
	{
		return;
	}

	ref.dllUiFuncs.setScissorEnabled(enabled);
}

void CL_UIGL_SetScissorRegion(int left, int bottom, int width, int height)
{
	if ( !ref.dllUiFuncs.setScissorRegion )
	{
		return;
	}

	ref.dllUiFuncs.setScissorRegion(left, bottom, width, height);
}

void CL_UIGL_SetStencilEnabled(qboolean enabled)
{
	if ( !ref.dllUiFuncs.setStencilEnabled )
	{
		return;
	}

	ref.dllUiFuncs.setStencilEnabled(enabled);
}

int CL_UIGL_EnableWritingToStencilMask(qboolean clearStencilBuffer)
{
	if ( !ref.dllUiFuncs.enableWritingToStencilMask )
	{
		return 0;
	}

	return ref.dllUiFuncs.enableWritingToStencilMask(clearStencilBuffer);
}

void CL_UIGL_DisableWritingToStencilMask(int testValue)
{
	if ( !ref.dllUiFuncs.disableWritingToStencilMask )
	{
		return;
	}

	ref.dllUiFuncs.disableWritingToStencilMask(testValue);
}

void CL_UIGL_SetStencilOpReplace(void)
{
	if ( !ref.dllUiFuncs.setStencilOpReplace )
	{
		return;
	}

	ref.dllUiFuncs.setStencilOpReplace();
}

void CL_UIGL_SetStencilOpIncrement(void)
{
	if ( !ref.dllUiFuncs.setStencilOpIncrement )
	{
		return;
	}

	ref.dllUiFuncs.setStencilOpIncrement();
}

void CL_UIGL_FreeImage(HIMAGE image)
{
	if ( !ref.dllUiFuncs.freeImage )
	{
		return;
	}

	ref.dllUiFuncs.freeImage(image);
}

void CL_UIGL_SetTransform(const float* mat4x4)
{
	if ( !ref.dllUiFuncs.setTransform )
	{
		return;
	}

	ref.dllUiFuncs.setTransform(mat4x4);
}
