#include "menus/StyleGuide.h"

StyleGuide::StyleGuide() :
	MenuPage("style_guide", "resource/rml/style_guide.rml"),
	m_MenuFrameDataBinding(this)
{
}

void StyleGuide::OnDocumentLoaded()
{
	m_MenuFrameDataBinding.SetDefaultTooltipText("None");
	m_MenuFrameDataBinding.SetTooltipInnerRml("Colour constant: {{footerTooltip}}");
	m_MenuFrameDataBinding.SetTitle("Style Guide");
}
