#pragma once

#include <RmlUi/Core/RenderInterface.h>

class RmlUiBackend;

// Based on https://github.com/mikke89/RmlUi/blob/master/Backends/RmlUi_Renderer_GL2.cpp
class RenderInterfaceImpl : public Rml::RenderInterface
{
public:
	// Can be passed to RenderGeometry() to enable texture rendering without changing the bound texture.
	static constexpr Rml::TextureHandle TextureEnableWithoutBinding = Rml::TextureHandle(-1);

	RenderInterfaceImpl(RmlUiBackend* backend);

	// The viewport should be updated whenever the window size changes.
	int ViewportWidth() const;
	int ViewportHeight() const;
	void SetViewport(int viewport_width, int viewport_height);

	// Sets up OpenGL states for taking rendering commands from RmlUi.
	void BeginFrame();
	void EndFrame();

	// Optional, can be used to clear the framebuffer.
	void Clear();

	// -- Inherited from Rml::RenderInterface --

	Rml::CompiledGeometryHandle
	CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;
	void ReleaseGeometry(Rml::CompiledGeometryHandle geometry) override;
	void
	RenderGeometry(Rml::CompiledGeometryHandle handle, Rml::Vector2f translation, Rml::TextureHandle texture) override;

	Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) override;
	Rml::TextureHandle GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) override;

	void ReleaseTexture(Rml::TextureHandle texture_handle) override;

	void EnableScissorRegion(bool enable) override;
	void SetScissorRegion(Rml::Rectanglei region) override;

	void EnableClipMask(bool enable) override;
	void RenderToClipMask(
		Rml::ClipMaskOperation operation,
		Rml::CompiledGeometryHandle geometry,
		Rml::Vector2f translation
	) override;

	void SetTransform(const Rml::Matrix4f* transform) override;

	void RenderDebugTriangle();

private:
	struct GeometryView
	{
		Rml::Span<const Rml::Vertex> vertices;
		Rml::Span<const int> indices;
	};

	RmlUiBackend* m_Backend;
	int m_ViewportWidth = 0;
	int m_ViewportHeight = 0;
	bool m_TransformEnabled = false;
	size_t m_GeneratedTextureCount = 0;
};
