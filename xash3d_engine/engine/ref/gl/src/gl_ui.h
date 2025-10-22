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
