#include "rmlui/FontEngineInterfaceImpl.h"
#include <RmlUi/Core/StringUtilities.h>

void FontEngineInterfaceImpl::Initialize()
{
	// TODO
}

void FontEngineInterfaceImpl::Shutdown()
{
	// TODOs
}

bool FontEngineInterfaceImpl::LoadFontFace(
	const Rml::String& /* file_name */,
	int /* face_index */,
	bool /* fallback_face */,
	Rml::Style::FontWeight /* weight */
)
{
	// TODO
	return false;
}

bool FontEngineInterfaceImpl::LoadFontFace(
	Rml::Span<const Rml::byte> /* data */,
	int /* face_index */,
	const Rml::String& /* family */,
	Rml::Style::FontStyle /* style */,
	Rml::Style::FontWeight /* weight */,
	bool /* fallback_face */
)
{
	// TODO
	return false;
}

Rml::FontFaceHandle FontEngineInterfaceImpl::GetFontFaceHandle(
	const Rml::String& /* family */,
	Rml::Style::FontStyle /* style */,
	Rml::Style::FontWeight /* weight */,
	int /* size */
)
{
	// TODO
	return false;
}

Rml::FontEffectsHandle FontEngineInterfaceImpl::
	PrepareFontEffects(Rml::FontFaceHandle /* handle */, const Rml::FontEffectList& /* font_effects */)
{
	// TODO
	return (Rml::FontEffectsHandle)-1;
}

const Rml::FontMetrics& FontEngineInterfaceImpl::GetFontMetrics(Rml::FontFaceHandle /* handle */)
{
	// TODO
	static Rml::FontMetrics metrics {};
	return metrics;
}

int FontEngineInterfaceImpl::GetStringWidth(
	Rml::FontFaceHandle /* handle */,
	Rml::StringView /* string */,
	const Rml::TextShapingContext& /* text_shaping_context */,
	Rml::Character /* prior_character = Rml::Character::Null */
)
{
	// TODO
	return 0;
}

int FontEngineInterfaceImpl::GenerateString(
	Rml::RenderManager& /* render_manager */,
	Rml::FontFaceHandle /* face_handle */,
	Rml::FontEffectsHandle /* font_effects_handle */,
	Rml::StringView /* string */,
	Rml::Vector2f /* position */,
	Rml::ColourbPremultiplied /* colour */,
	float /* opacity */,
	const Rml::TextShapingContext& /* text_shaping_context */,
	Rml::TexturedMeshList& /* mesh_list */
)
{
	// TODO
	return 0;
}

int FontEngineInterfaceImpl::GetVersion(Rml::FontFaceHandle /* handle */)
{
	// TODO
	return 0;
}

void FontEngineInterfaceImpl::ReleaseFontResources()
{
	// TODO
}
