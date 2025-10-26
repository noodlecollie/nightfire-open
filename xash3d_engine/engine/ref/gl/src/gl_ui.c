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

#ifdef _DEBUG
static qboolean CheckOpenGLError(const char* file, int line)
{
	int lastError = pglGetError();

	if ( lastError != GL_NO_ERROR )
	{
		gEngfuncs.Con_Printf(
			S_ERROR "pglGetError returned %s (%d) at %s:%d\n",
			GL_ErrorString(lastError),
			lastError,
			file,
			line
		);

		return false;
	}

	return true;
}
#define CHECK_OPENGL_ERROR() CheckOpenGLError(__FILE__, __LINE__)
#else
#define CHECK_OPENGL_ERROR() (true)
#endif

static float PackedColourByteToFloat(uint32_t colour, ColourChannel channel)
{
	const size_t shift = channel * 8;
	uint8_t value = (uint8_t)((colour & (0xFF << shift)) >> shift);
	return (float)value / 255.0f;
}

void GL_UI_BeginFrame(const struct ref_viewpass_s* rvp)
{
	ASSERT(CHECK_OPENGL_ERROR());

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

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_EndFrame(void)
{
	ASSERT(CHECK_OPENGL_ERROR());

	// TODO: Is this needed, or does the engine end the frame for us?
	R_EndFrame();

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_Clear(uint32_t colour)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglClearStencil(0);
	pglClearColor(
		PackedColourByteToFloat(colour, CHANNEL_RED),
		PackedColourByteToFloat(colour, CHANNEL_GREEN),
		PackedColourByteToFloat(colour, CHANNEL_BLUE),
		PackedColourByteToFloat(colour, CHANNEL_ALPHA)
	);
	pglClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_PushMatrixTranslation(float x, float y, float z)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglPushMatrix();
	pglTranslatef(x, y, z);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_PopMatrix(void)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglPopMatrix();

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_PrepareToDrawWithoutTexture(const void* data, int objectSize, size_t positionOffset, size_t colourOffset)
{
	ASSERT(CHECK_OPENGL_ERROR());

	uint8_t* dataBytes = (uint8_t*)data;

	pglVertexPointer(2, GL_FLOAT, objectSize, dataBytes + positionOffset);
	pglColorPointer(4, GL_UNSIGNED_BYTE, objectSize, dataBytes + colourOffset);

	pglDisable(GL_TEXTURE_2D);
	pglDisableClientState(GL_TEXTURE_COORD_ARRAY);

	ASSERT(CHECK_OPENGL_ERROR());
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
	ASSERT(CHECK_OPENGL_ERROR());

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

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_DrawElements(int numIndices, const void* indices)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_SetScissorEnabled(qboolean enabled)
{
	ASSERT(CHECK_OPENGL_ERROR());

	if ( enabled )
	{
		pglEnable(GL_SCISSOR_TEST);
	}
	else
	{
		pglDisable(GL_SCISSOR_TEST);
	}

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_SetScissorRegion(int left, int bottom, int width, int height)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglScissor(left, bottom, width, height);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_SetStencilEnabled(qboolean enabled)
{
	ASSERT(CHECK_OPENGL_ERROR());

	if ( enabled )
	{
		pglEnable(GL_STENCIL_TEST);
	}
	else
	{
		pglDisable(GL_STENCIL_TEST);
	}

	ASSERT(CHECK_OPENGL_ERROR());
}

int GL_UI_EnableWritingToStencilMask(qboolean clearStencilBuffer)
{
	ASSERT(CHECK_OPENGL_ERROR());
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

	ASSERT(CHECK_OPENGL_ERROR());

	return stencilTestValue;
}

void GL_UI_DisableWritingToStencilMask(int testValue)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	pglStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	pglStencilFunc(GL_EQUAL, testValue, (GLuint)-1);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_SetStencilOpReplace(void)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_SetStencilOpIncrement(void)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	ASSERT(CHECK_OPENGL_ERROR());
}

int GL_UI_LoadRGBAImageFromMemory(const char* name, int width, int height, const byte* data, size_t dataSize, int flags)
{
	ASSERT(CHECK_OPENGL_ERROR());

	if ( width < 1 || height < 1 )
	{
		gEngfuncs.Con_Printf(
			S_ERROR "GL_UI_LoadRGBAImageFromMemory: Invalid image dimensions provided for texture %s.\n",
			name
		);

		return 0;
	}

	if ( width * height * 4 != dataSize )
	{
		gEngfuncs.Con_Printf(
			S_ERROR
			"GL_UI_LoadRGBAImageFromMemory: Texture %s data size of %zu bytes did not match image dimensions %dx%dx4 "
			"(%d "
			"bytes).\n",
			name,
			dataSize,
			width,
			height,
			width * height * 4
		);

		return 0;
	}

	rgbdata_t rgb;
	memset(&rgb, 0, sizeof(rgb));

	rgb.buffer = (byte*)data;
	rgb.size = dataSize;
	rgb.width = (word)width;
	rgb.height = (word)height;
	rgb.depth = 1;  // This is 1D/2D/3D depth, NOT image data depth! The latter is inferred from flags.
	rgb.flags = flags | IMAGE_HAS_COLOR | IMAGE_HAS_ALPHA;
	rgb.encode = DXT_ENCODE_DEFAULT;
	rgb.type = PF_RGBA_32;
	rgb.numMips = 1;

	int texture = GL_LoadTextureFromBuffer(name, &rgb, TF_NOMIPMAP, false);

	ASSERT(CHECK_OPENGL_ERROR());

	return texture;
}

void GL_UI_FreeImage(int image)
{
	ASSERT(CHECK_OPENGL_ERROR());

	pglDeleteTextures(1, (GLuint*)&image);

	ASSERT(CHECK_OPENGL_ERROR());
}

void GL_UI_SetTransform(const float* mat4x4)
{
	ASSERT(CHECK_OPENGL_ERROR());

	if ( mat4x4 )
	{
		pglLoadMatrixf(mat4x4);
	}
	else
	{
		pglLoadIdentity();
	}

	ASSERT(CHECK_OPENGL_ERROR());
}
