#pragma once

#include <stdint.h>
#include "BuildPlatform/Typedefs.h"
#include "EngineInternalAPI/menu_int.h"

void CL_UIGL_BeginFrame(int viewportX, int viewportY, int viewportWidth, int viewportHeight);
void CL_UIGL_EndFrame(void);
void CL_UIGL_Clear(uint32_t colour, int stencil);
void CL_UIGL_PushMatrixTranslation(float x, float y, float z);
void CL_UIGL_PopMatrix(void);
void CL_UIGL_PrepareToDrawWithoutTexture(const void* data, int objectSize, size_t positionOffset, size_t colourOffset);
void CL_UIGL_PrepareToDrawWithTexture(
	uint32_t texture,
	const void* data,
	int objectSize,
	size_t positionOffset,
	size_t colourOffset,
	size_t textureCoOrdOffset
);
void CL_UIGL_DrawElements(int numIndices, const void* indices);
void CL_UIGL_SetScissorEnabled(qboolean enabled);
void CL_UIGL_SetScissorRegion(int left, int bottom, int width, int height);
void CL_UIGL_SetStencilEnabled(qboolean enabled);
int CL_UIGL_EnableWritingToStencilMask(qboolean clearStencilBuffer);
void CL_UIGL_DisableWritingToStencilMask(int testValue);
void CL_UIGL_SetStencilOpReplace(void);
void CL_UIGL_SetStencilOpIncrement(void);
HIMAGE
CL_UIGL_LoadRGBAImageFromMemory(const char* name, int width, int height, const byte* data, size_t dataSize, int flags);
void CL_UIGL_FreeImage(HIMAGE image);
void CL_UIGL_SetTransform(const float* mat4x4);
