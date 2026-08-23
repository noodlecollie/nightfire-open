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

void RenderInterfaceImpl::SetViewport(Rml::Vector2i windowSize, Rml::Rectanglei viewport)
{
	ASSERT(windowSize.x > 0 && windowSize.y > 0);
	ASSERT(viewport.Width() > 0 && viewport.Height() > 0);

	m_WindowSize = windowSize;
	m_Viewport = viewport;
}

void RenderInterfaceImpl::BeginFrame()
{
	ASSERT(m_Viewport.Width() > 0 && m_Viewport.Height() > 0);

	if ( m_Viewport.Top() > 0 || m_Viewport.Left() > 0 )
	{
		gUiGlFuncs.renderer.clear(0x00000000, 0);
	}

	// The offset from (0,0) represents the pillarboxing/letterboxing
	// that we want to apply. If we translate the entire window up and
	// left by this value, this adds the margins that we need. The
	// viewport itself will be scaled to be smaller than the window,
	// which will result in the margins at the other side.

	gUiGlFuncs.renderer.beginFrame(
		-m_Viewport.Left(),
		-m_Viewport.Top(),
		m_WindowSize.x - m_Viewport.Left(),
		m_WindowSize.y - m_Viewport.Top()
	);
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
	gUiGlFuncs.renderer.setScissorRegion(
		region.Left() + m_Viewport.Left(),
		m_Viewport.Height() - region.Bottom() + m_Viewport.Top(),
		region.Width(),
		region.Height()
	);
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
