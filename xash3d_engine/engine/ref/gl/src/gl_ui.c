#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "gl_ui.h"
#include "gl_local.h"
#include "gl_export.h"

typedef enum ColourChannel
{
	CHANNEL_ALPHA = 0,
	CHANNEL_BLUE,
	CHANNEL_GREEN,
	CHANNEL_RED
} ColourChannel;

static float PackedColourByteToFloat(uint32_t colour, ColourChannel channel)
{
	const size_t shift = channel * 8;
	uint8_t value = (uint8_t)((colour & (0xFF << shift)) >> shift);
	return (float)value / 255.0f;
}

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

void GL_UI_Clear(uint32_t colour)
{
	pglClearStencil(0);
	pglClearColor(
		PackedColourByteToFloat(colour, CHANNEL_RED),
		PackedColourByteToFloat(colour, CHANNEL_GREEN),
		PackedColourByteToFloat(colour, CHANNEL_BLUE),
		PackedColourByteToFloat(colour, CHANNEL_ALPHA)
	);
	pglClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GL_UI_PushMatrixTranslation(float x, float y, float z)
{
	pglPushMatrix();
	pglTranslatef(x, y, z);
}

void GL_UI_PopMatrix(void)
{
	pglPopMatrix();
}

void GL_UI_PrepareToDrawWithoutTexture(const void* data, int objectSize, size_t positionOffset, size_t colourOffset)
{
	uint8_t* dataBytes = (uint8_t*)data;

	pglVertexPointer(2, GL_FLOAT, objectSize, dataBytes + positionOffset);
	pglColorPointer(4, GL_UNSIGNED_BYTE, objectSize, dataBytes + colourOffset);

	pglDisable(GL_TEXTURE_2D);
	pglDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GL_UI_PrepareToDrawWithTexture(
	uint32_t texture,
	const void* data,
	int objectSize,
	size_t positionOffset,
	size_t colourOffset,
	size_t textureCoOrdOffset
)
{
	uint8_t* dataBytes = (uint8_t*)data;

	pglVertexPointer(2, GL_FLOAT, objectSize, dataBytes + positionOffset);
	pglColorPointer(4, GL_UNSIGNED_BYTE, objectSize, dataBytes + colourOffset);

	pglEnable(GL_TEXTURE_2D);

	// If texture is 0, don't re-bind. We assume that whatever
	// texture that is required is already bound.
	if ( texture != 0 )
	{
		pglBindTexture(GL_TEXTURE_2D, (GLuint)texture);
	}

	pglEnableClientState(GL_TEXTURE_COORD_ARRAY);
	pglTexCoordPointer(2, GL_FLOAT, objectSize, dataBytes + textureCoOrdOffset);
}

void GL_UI_DrawElements(int numIndices, const void* indices)
{
	pglDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);
}

void GL_UI_SetScissorEnabled(qboolean enabled)
{
	if ( enabled )
	{
		pglEnable(GL_SCISSOR_TEST);
	}
	else
	{
		pglDisable(GL_SCISSOR_TEST);
	}
}

void GL_UI_SetScissorRegion(int left, int bottom, int width, int height)
{
	pglScissor(left, bottom, width, height);
}

void GL_UI_SetStencilEnabled(qboolean enabled)
{
	if ( enabled )
	{
		pglEnable(GL_STENCIL_TEST);
	}
	else
	{
		pglDisable(GL_STENCIL_TEST);
	}
}

int GL_UI_EnableWritingToStencilMask(qboolean clearStencilBuffer)
{
	ASSERT(pglIsEnabled(GL_STENCIL_TEST));

	if ( clearStencilBuffer )
	{
		// @performance Increment the reference value instead of clearing each time.
		pglClear(GL_STENCIL_BUFFER_BIT);
	}

	GLint stencilTestValue = 0;
	pglGetIntegerv(GL_STENCIL_REF, &stencilTestValue);

	pglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	pglStencilFunc(GL_ALWAYS, (GLint)1, (GLuint)-1);

	return stencilTestValue;
}

void GL_UI_DisableWritingToStencilMask(int testValue)
{
	pglColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	pglStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	pglStencilFunc(GL_EQUAL, testValue, (GLuint)-1);
}

void GL_UI_SetStencilOpReplace(void)
{
	pglStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void GL_UI_SetStencilOpIncrement(void)
{
	pglStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
}

void GL_UI_FreeImage(int image)
{
	pglDeleteTextures(1, (GLuint*)&image);
}

void GL_UI_SetTransform(const float* mat4x4)
{
	if ( mat4x4 )
	{
		pglLoadMatrixf(mat4x4);
	}
	else
	{
		pglLoadIdentity();
	}
}
