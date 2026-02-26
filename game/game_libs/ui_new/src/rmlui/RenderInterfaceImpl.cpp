#include "rmlui/RenderInterfaceImpl.h"
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Platform.h>
#include <string.h>
#include "udll_int.h"
#include "PlatformLib/String.h"
#include "UIDebug.h"

RenderInterfaceImpl::RenderInterfaceImpl(RmlUiBackend* backend) :
	m_Backend(backend)
{
}

int RenderInterfaceImpl::ViewportWidth() const
{
	return m_ViewportWidth;
}

int RenderInterfaceImpl::ViewportHeight() const
{
	return m_ViewportHeight;
}

void RenderInterfaceImpl::SetViewport(int in_viewport_width, int in_viewport_height)
{
	ASSERT(in_viewport_width > 0);
	ASSERT(in_viewport_height > 0);

	m_ViewportWidth = in_viewport_width;
	m_ViewportHeight = in_viewport_height;
}

void RenderInterfaceImpl::BeginFrame()
{
	ASSERT(m_ViewportWidth > 0 && m_ViewportHeight > 0);
	gUiGlFuncs.renderer.beginFrame(0, 0, m_ViewportWidth, m_ViewportHeight);
}

void RenderInterfaceImpl::EndFrame()
{
	gUiGlFuncs.renderer.endFrame();
}

void RenderInterfaceImpl::Clear()
{
	gUiGlFuncs.renderer.clear(0x00000000, 0);
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

	gUiGlFuncs.renderer.pushProjectionMatrixTranslation(translation.x, translation.y, 0.0f);

	if ( texture )
	{
		gUiGlFuncs.renderer.prepareToDrawWithTexture(
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
		gUiGlFuncs.renderer.prepareToDrawWithoutTexture(
			vertices,
			sizeof(Rml::Vertex),
			offsetof(Rml::Vertex, position),
			offsetof(Rml::Vertex, colour)
		);
	}

	gUiGlFuncs.renderer.drawElements(num_indices, indices);
	gUiGlFuncs.renderer.popProjectionMatrix();
}

void RenderInterfaceImpl::EnableScissorRegion(bool enable)
{
	gUiGlFuncs.renderer.setScissorEnabled(enable);
}

void RenderInterfaceImpl::SetScissorRegion(Rml::Rectanglei region)
{
	gUiGlFuncs.renderer
		.setScissorRegion(region.Left(), m_ViewportHeight - region.Bottom(), region.Width(), region.Height());
}

void RenderInterfaceImpl::EnableClipMask(bool enable)
{
	gUiGlFuncs.renderer.setStencilEnabled(enable);
}

void RenderInterfaceImpl::RenderToClipMask(
	Rml::ClipMaskOperation operation,
	Rml::CompiledGeometryHandle geometry,
	Rml::Vector2f translation
)
{
	using Rml::ClipMaskOperation;

	const bool clear_stencil = (operation == ClipMaskOperation::Set || operation == ClipMaskOperation::SetInverse);
	int stencil_test_value = gUiGlFuncs.renderer.enableWritingToStencilMask(clear_stencil);

	switch ( operation )
	{
		case ClipMaskOperation::Set:
		{
			gUiGlFuncs.renderer.setStencilOpReplace();
			stencil_test_value = 1;
			break;
		}

		case ClipMaskOperation::SetInverse:
		{
			gUiGlFuncs.renderer.setStencilOpReplace();
			stencil_test_value = 0;
			break;
		}

		case ClipMaskOperation::Intersect:
		{
			gUiGlFuncs.renderer.setStencilOpIncrement();
			stencil_test_value += 1;
			break;
		}
	}

	RenderGeometry(geometry, translation, {});
	gUiGlFuncs.renderer.disableWritingToStencilMask(stencil_test_value);
}

Rml::TextureHandle RenderInterfaceImpl::LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
	HIMAGE image = gEngfuncs.pfnPIC_Load(source.c_str(), nullptr, 0, 0);

	ASSERTSZ(image != 0, "Failed to load texture");

	if ( !image )
	{
		return 0;
	}

	texture_dimensions.x = gEngfuncs.pfnPIC_Width(image);
	texture_dimensions.y = gEngfuncs.pfnPIC_Height(image);

	return static_cast<Rml::TextureHandle>(image);
}

Rml::TextureHandle
RenderInterfaceImpl::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions)
{
	char textureName[64];
	PlatformLib_SNPrintF(textureName, sizeof(textureName), "#RMLUI_TEX_GEN_%zu", m_GeneratedTextureCount + 1);

	HIMAGE image = gUiGlFuncs.renderer.loadRGBAImageFromMemory(
		textureName,
		source_dimensions.x,
		source_dimensions.y,
		source.data(),
		source.size(),
		0
	);

	ASSERTSZ(image != 0, "Failed to load texture");

	if ( image == 0 )
	{
		return 0;
	}

	++m_GeneratedTextureCount;
	return static_cast<Rml::TextureHandle>(image);
}

void RenderInterfaceImpl::ReleaseTexture(Rml::TextureHandle texture_handle)
{
	gUiGlFuncs.renderer.freeImage(static_cast<HIMAGE>(texture_handle));
}

void RenderInterfaceImpl::SetTransform(const Rml::Matrix4f* transform)
{
	m_TransformEnabled = transform != nullptr;

	if ( m_TransformEnabled )
	{
		if ( std::is_same<Rml::Matrix4f, Rml::ColumnMajorMatrix4f>::value )
		{
			gUiGlFuncs.renderer.setTransform(transform->data());
			return;
		}

		if ( std::is_same<Rml::Matrix4f, Rml::RowMajorMatrix4f>::value )
		{
			gUiGlFuncs.renderer.setTransform(transform->Transpose().data());
			return;
		}

		ASSERTSZ(false, "Unrecognised transform type");
	}

	gUiGlFuncs.renderer.setTransform(nullptr);
}

void RenderInterfaceImpl::RenderDebugTriangle()
{
	static Rml::CompiledGeometryHandle geom = 0;
	static Rml::TextureHandle texture = 0;

	if ( !geom )
	{
		static const float extent = 200.0f;

		static const Rml::Vertex verticesArray[] = {
			{{0, 0}, Rml::ColourbPremultiplied {255, 0, 0, 255}, {0, 0}},
			{{extent, 0}, Rml::ColourbPremultiplied {255, 0, 0, 255}, {1, 0}},
			{{extent / 2.0f, extent}, Rml::ColourbPremultiplied {255, 0, 0, 255}, {0.5f, 1}},
		};

		static const int indicesArray[] = {
			0,
			1,
			2,
		};

		Rml::Span<const Rml::Vertex> vertices(verticesArray, 3);
		Rml::Span<const int> indices(indicesArray, 3);
		geom = CompileGeometry(vertices, indices);
	}

	if ( !texture )
	{
		static constexpr int WIDTH = 16;
		static constexpr int HEIGHT = 16;

		uint32_t texDataRGBA[WIDTH * HEIGHT];
		memset(&texDataRGBA, 0xFF, sizeof(texDataRGBA));

		Rml::Span<const Rml::byte> texData(reinterpret_cast<const Rml::byte*>(texDataRGBA), sizeof(texDataRGBA));
		texture = GenerateTexture(texData, Rml::Vector2i(WIDTH, HEIGHT));
	}

	gUiGlFuncs.renderer.beginFrame(0, 0, m_ViewportWidth, m_ViewportHeight);
	gUiGlFuncs.renderer.clear(0x000000FF, 0);

	gUiGlFuncs.renderer.pushProjectionMatrixTranslation(20, 50, 0);

	const GeometryView* geometry = reinterpret_cast<GeometryView*>(geom);
	const Rml::Vertex* vertices = geometry->vertices.data();
	const int* indices = geometry->indices.data();
	const int num_indices = static_cast<int>(geometry->indices.size());

	gUiGlFuncs.renderer.prepareToDrawWithTexture(
		static_cast<uint32_t>(texture),
		vertices,
		sizeof(Rml::Vertex),
		offsetof(Rml::Vertex, position),
		offsetof(Rml::Vertex, colour),
		offsetof(Rml::Vertex, tex_coord)
	);

	gUiGlFuncs.renderer.drawElements(num_indices, indices);
	gUiGlFuncs.renderer.popProjectionMatrix();
	gUiGlFuncs.renderer.endFrame();
}
