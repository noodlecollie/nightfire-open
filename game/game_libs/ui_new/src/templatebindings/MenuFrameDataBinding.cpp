#include "templatebindings/MenuFrameDataBinding.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

MenuFrameDataBinding::MenuFrameDataBinding() :
	m_Tooltip("footer_tooltip", "")
{
}

bool MenuFrameDataBinding::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	constructor.Bind(m_Tooltip.Name(), &m_Tooltip.Value());
	constructor.BindEventCallback("set_tooltip", &MenuFrameDataBinding::SetTooltip, this);
	constructor.BindEventCallback("clear_tooltip", &MenuFrameDataBinding::ClearTooltip, this);

	return true;
}

void MenuFrameDataBinding::SetTooltip(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList&)
{
	Rml::Element* element = event.GetTargetElement();

	if ( !element )
	{
		return;
	}

	Rml::Variant* tooltipAttr = element->GetAttribute("tooltip");

	if ( !tooltipAttr )
	{
		return;
	}

	if ( tooltipAttr->GetInto(m_Tooltip.Value()) )
	{
		handle.DirtyVariable(m_Tooltip.Name());
	}
}

void MenuFrameDataBinding::ClearTooltip(Rml::DataModelHandle handle, Rml::Event&, const Rml::VariantList&)
{
	if ( !m_Tooltip.Value().empty() )
	{
		m_Tooltip.Value().clear();
		handle.DirtyVariable(m_Tooltip.Name());
	}
}
