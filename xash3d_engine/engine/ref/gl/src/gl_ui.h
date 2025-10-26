#pragma once

#include <stdint.h>
#include "EnginePublicAPI/ref_params.h"

void GL_UI_BeginFrame(const struct ref_viewpass_s* rvp);
void GL_UI_EndFrame(void);
void GL_UI_Clear(uint32_t colour);
void GL_UI_PushMatrixTranslation(float x, float y, float z);
void GL_UI_PopMatrix(void);
void GL_UI_PrepareToDrawWithoutTexture(const void* data, int objectSize, size_t positionOffset, size_t colourOffset);
void GL_UI_PrepareToDrawWithTexture(
	uint32_t texture,
	const void* data,
	int objectSize,
	size_t positionOffset,
	size_t colourOffset,
	size_t textureCoOrdOffset
);
void GL_UI_DrawElements(int numIndices, const void* indices);
void GL_UI_SetScissorEnabled(qboolean enabled);
void GL_UI_SetScissorRegion(int left, int bottom, int width, int height);
void GL_UI_SetStencilEnabled(qboolean enabled);
int GL_UI_EnableWritingToStencilMask(qboolean clearStencilBuffer);
void GL_UI_DisableWritingToStencilMask(int testValue);
void GL_UI_SetStencilOpReplace(void);
void GL_UI_SetStencilOpIncrement(void);
int GL_UI_LoadRGBAImageFromMemory(
	const char* name,
	int width,
	int height,
	const byte* data,
	size_t dataSize,
	int flags
);
void GL_UI_FreeImage(int image);
void GL_UI_SetTransform(const float* mat4x4);
