#include "rmlui/RenderInterfaceImpl.h"
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Platform.h>
#include <string.h>
#include "udll_int.h"
#include "PlatformLib/String.h"

RenderInterfaceImpl::RenderInterfaceImpl(RmlUiBackend* backend) :
	m_Backend(backend)
{
}

void RenderInterfaceImpl::SetViewport(int in_viewport_width, int in_viewport_height)
{
	m_ViewportWidth = in_viewport_width;
	m_ViewportHeight = in_viewport_height;
}

void RenderInterfaceImpl::BeginFrame()
{
#ifdef RMLUI_REFERENCE_CODE
	RMLUI_ASSERT(viewport_width >= 0 && viewport_height >= 0);
	glViewport(0, 0, viewport_width, viewport_height);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, GLuint(-1));
	glStencilMask(GLuint(-1));
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	Rml::Matrix4f projection =
		Rml::Matrix4f::ProjectOrtho(0, (float)viewport_width, (float)viewport_height, 0, -10000, 10000);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.data());
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	transform_enabled = false;
#else
	gUiGlFuncs.beginFrame(0, 0, m_ViewportWidth, m_ViewportHeight);
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::EndFrame()
{
	gUiGlFuncs.endFrame();
}

void RenderInterfaceImpl::Clear()
{
#ifdef RMLUI_REFERENCE_CODE
	glClearStencil(0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#else
	gUiGlFuncs.clear(0x00000000);
#endif  // RMLUI_REFERENCE_CODE
}

Rml::CompiledGeometryHandle
RenderInterfaceImpl::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices)
{
	GeometryView* data = new GeometryView {vertices, indices};
	return reinterpret_cast<Rml::CompiledGeometryHandle>(data);
}

void RenderInterfaceImpl::ReleaseGeometry(Rml::CompiledGeometryHandle geometry)
{
	delete reinterpret_cast<GeometryView*>(geometry);
}

void RenderInterfaceImpl::RenderGeometry(
	Rml::CompiledGeometryHandle handle,
	Rml::Vector2f translation,
	Rml::TextureHandle texture
)
{
	const GeometryView* geometry = reinterpret_cast<GeometryView*>(handle);
	const Rml::Vertex* vertices = geometry->vertices.data();
	const int* indices = geometry->indices.data();
	const int num_indices = static_cast<int>(geometry->indices.size());

#ifdef RMLUI_REFERENCE_CODE
	glPushMatrix();
	glTranslatef(translation.x, translation.y, 0);

	glVertexPointer(2, GL_FLOAT, sizeof(Rml::Vertex), &vertices[0].position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rml::Vertex), &vertices[0].colour);

	if ( !texture )
	{
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		if ( texture != TextureEnableWithoutBinding )
		{
			glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
		}

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Rml::Vertex), &vertices[0].tex_coord);
	}

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

	glPopMatrix();
#else
	gUiGlFuncs.pushMatrixTranslation(translation.x, translation.y, 0.0f);

	if ( texture )
	{
		gUiGlFuncs.prepareToDrawWithTexture(
			texture != TextureEnableWithoutBinding ? static_cast<uint32_t>(texture) : 0,
			vertices,
			sizeof(Rml::Vertex),
			offsetof(Rml::Vertex, position),
			offsetof(Rml::Vertex, colour),
			offsetof(Rml::Vertex, tex_coord)
		);
	}
	else
	{
		gUiGlFuncs.prepareToDrawWithoutTexture(
			vertices,
			sizeof(Rml::Vertex),
			offsetof(Rml::Vertex, position),
			offsetof(Rml::Vertex, colour)
		);
	}

	gUiGlFuncs.drawElements(num_indices, indices);
	gUiGlFuncs.popMatrix();
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::EnableScissorRegion(bool enable)
{
#ifdef RMLUI_REFERENCE_CODE
	if ( enable )
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}
#else
	gUiGlFuncs.setScissorEnabled(enable);
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::SetScissorRegion(Rml::Rectanglei region)
{
#ifdef RMLUI_REFERENCE_CODE
	glScissor(region.Left(), viewport_height - region.Bottom(), region.Width(), region.Height());
#else
	gUiGlFuncs.setScissorRegion(region.Left(), m_ViewportHeight - region.Bottom(), region.Width(), region.Height());
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::EnableClipMask(bool enable)
{
#ifdef RMLUI_REFERENCE_CODE
	if ( enable )
	{
		glEnable(GL_STENCIL_TEST);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}
#else
	gUiGlFuncs.setStencilEnabled(enable);
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::RenderToClipMask(
	Rml::ClipMaskOperation operation,
	Rml::CompiledGeometryHandle geometry,
	Rml::Vector2f translation
)
{
#ifdef RMLUI_REFERENCE_CODE
	RMLUI_ASSERT(glIsEnabled(GL_STENCIL_TEST));
	using Rml::ClipMaskOperation;

	const bool clear_stencil = (operation == ClipMaskOperation::Set || operation == ClipMaskOperation::SetInverse);
	if ( clear_stencil )
	{
		// @performance Increment the reference value instead of clearing each time.
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	GLint stencil_test_value = 0;
	glGetIntegerv(GL_STENCIL_REF, &stencil_test_value);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, GLint(1), GLuint(-1));

	switch ( operation )
	{
		case ClipMaskOperation::Set:
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			stencil_test_value = 1;
		}
		break;
		case ClipMaskOperation::SetInverse:
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			stencil_test_value = 0;
		}
		break;
		case ClipMaskOperation::Intersect:
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			stencil_test_value += 1;
		}
		break;
	}

	RenderGeometry(geometry, translation, {});

	// Restore state
	// @performance Cache state so we don't toggle it unnecessarily.
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_EQUAL, stencil_test_value, GLuint(-1));
#else
	using Rml::ClipMaskOperation;

	const bool clear_stencil = (operation == ClipMaskOperation::Set || operation == ClipMaskOperation::SetInverse);
	int stencil_test_value = gUiGlFuncs.enableWritingToStencilMask(clear_stencil);

	switch ( operation )
	{
		case ClipMaskOperation::Set:
		{
			gUiGlFuncs.setStencilOpReplace();
			stencil_test_value = 1;
			break;
		}

		case ClipMaskOperation::SetInverse:
		{
			gUiGlFuncs.setStencilOpReplace();
			stencil_test_value = 0;
			break;
		}

		case ClipMaskOperation::Intersect:
		{
			gUiGlFuncs.setStencilOpIncrement();
			stencil_test_value += 1;
			break;
		}
	}

	RenderGeometry(geometry, translation, {});
	gUiGlFuncs.disableWritingToStencilMask(stencil_test_value);
#endif  // RMLUI_REFERENCE_CODE
}

#ifdef RMLUI_REFERENCE_CODE
// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader
{
	char idLength;
	char colourMapType;
	char dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char bitsPerPixel;
	char imageDescriptor;
};
// Restore packing
#pragma pack()
#endif  // RMLUI_REFERENCE_CODE

Rml::TextureHandle RenderInterfaceImpl::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
#ifdef RMLUI_REFERENCE_CODE
	Rml::FileInterface* file_interface = Rml::GetFileInterface();
	Rml::FileHandle file_handle = file_interface->Open(source);
	if ( !file_handle )
	{
		return false;
	}

	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);

	if ( buffer_size <= sizeof(TGAHeader) )
	{
		Rml::Log::Message(
			Rml::Log::LT_ERROR,
			"Texture file size is smaller than TGAHeader, file is not a valid TGA image."
		);
		file_interface->Close(file_handle);
		return false;
	}

	using Rml::byte;
	Rml::UniquePtr<byte[]> buffer(new byte[buffer_size]);
	file_interface->Read(buffer.get(), buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer.get(), sizeof(TGAHeader));

	int color_mode = header.bitsPerPixel / 8;
	const size_t image_size = header.width * header.height * 4;  // We always make 32bit textures

	if ( header.dataType != 2 )
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}

	// Ensure we have at least 3 colors
	if ( color_mode < 3 )
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Only 24 and 32bit textures are supported.");
		return false;
	}

	const byte* image_src = buffer.get() + sizeof(TGAHeader);
	Rml::UniquePtr<byte[]> image_dest_buffer(new byte[image_size]);
	byte* image_dest = image_dest_buffer.get();

	// Targa is BGR, swap to RGB, flip Y axis, and convert to premultiplied alpha.
	for ( long y = 0; y < header.height; y++ )
	{
		long read_index = y * header.width * color_mode;
		long write_index =
			((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * 4;
		for ( long x = 0; x < header.width; x++ )
		{
			image_dest[write_index] = image_src[read_index + 2];
			image_dest[write_index + 1] = image_src[read_index + 1];
			image_dest[write_index + 2] = image_src[read_index];
			if ( color_mode == 4 )
			{
				const byte alpha = image_src[read_index + 3];
				for ( size_t j = 0; j < 3; j++ )
					image_dest[write_index + j] = byte((image_dest[write_index + j] * alpha) / 255);
				image_dest[write_index + 3] = alpha;
			}
			else
				image_dest[write_index + 3] = 255;

			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;

	return GenerateTexture({image_dest, image_size}, texture_dimensions);
#else  // RMLUI_REFERENCE_CODE
	HIMAGE image = gEngfuncs.pfnPIC_Load(source.c_str(), nullptr, 0, 0);

	if ( !image )
	{
		return 0;
	}

	texture_dimensions.x = gEngfuncs.pfnPIC_Width(image);
	texture_dimensions.y = gEngfuncs.pfnPIC_Height(image);

	return static_cast<Rml::TextureHandle>(image);
#endif  // RMLUI_REFERENCE_CODE
}

Rml::TextureHandle
RenderInterfaceImpl::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions)
{
#ifdef RMLUI_REFERENCE_CODE
	RMLUI_ASSERT(source.data() && source.size() == size_t(source_dimensions.x * source_dimensions.y * 4));

	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if ( texture_id == 0 )
	{
		Rml::Log::Message(Rml::Log::LT_ERROR, "Failed to generate texture.");
		return {};
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		source_dimensions.x,
		source_dimensions.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		source.data()
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return (Rml::TextureHandle)texture_id;
#else
	(void)source_dimensions;

	char textureName[64];
	PlatformLib_SNPrintF(textureName, sizeof(textureName), "#RMLUI_TEX_GEN_%zu", ++m_GeneratedTextureCount);

	HIMAGE image = gUiGlFuncs.loadImageFromMemory(textureName, source.data(), source.size(), 0);
	return static_cast<Rml::TextureHandle>(image);
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::ReleaseTexture(Rml::TextureHandle texture_handle)
{
#ifdef RMLUI_REFERENCE_CODE
	glDeleteTextures(1, (GLuint*)&texture_handle);
#else
	gUiGlFuncs.freeImage(static_cast<HIMAGE>(texture_handle));
#endif  // RMLUI_REFERENCE_CODE
}

void RenderInterfaceImpl::SetTransform(const Rml::Matrix4f* transform)
{
#ifdef RMLUI_REFERENCE_CODE
	transform_enabled = (transform != nullptr);

	if ( transform )
	{
		if ( std::is_same<Rml::Matrix4f, Rml::ColumnMajorMatrix4f>::value )
		{
			glLoadMatrixf(transform->data());
		}
		else if ( std::is_same<Rml::Matrix4f, Rml::RowMajorMatrix4f>::value )
		{
			glLoadMatrixf(transform->Transpose().data());
		}
	}
	else
	{
		glLoadIdentity();
	}
#else
	m_TransformEnabled = transform != nullptr;

	if ( m_TransformEnabled )
	{
		if ( std::is_same<Rml::Matrix4f, Rml::ColumnMajorMatrix4f>::value )
		{
			gUiGlFuncs.setTransform(transform->data());
			return;
		}

		if ( std::is_same<Rml::Matrix4f, Rml::RowMajorMatrix4f>::value )
		{
			gUiGlFuncs.setTransform(transform->Transpose().data());
			return;
		}
	}

	gUiGlFuncs.setTransform(nullptr);
#endif  // RMLUI_REFERENCE_CODE
}
