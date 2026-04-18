#include "templatebindings/MenuFrameDataBinding.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

// These are the elements that we support tooltips on:
static constexpr const char* const TOOLTIP_SELECTOR = "bigbutton[tooltip], button[tooltip]";

MenuFrameDataBinding::MenuFrameDataBinding(BaseMenu* parentMenu) :
	DocumentObserver(parentMenu),
	m_Tooltip {"footerTooltip", ""},
	m_TooltipListener(this, &MenuFrameDataBinding::HandleMouseEvents)
{
}

bool MenuFrameDataBinding::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_Tooltip.name, &m_Tooltip.value) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void MenuFrameDataBinding::DocumentLoaded(Rml::ElementDocument* document)
{
	Rml::ElementList elements;
	document->QuerySelectorAll(elements, TOOLTIP_SELECTOR);

	for ( Rml::Element* element : elements )
	{
		element->AddEventListener(Rml::EventId::Mouseover, &m_TooltipListener);
		element->AddEventListener(Rml::EventId::Mouseout, &m_TooltipListener);
	}
}

void MenuFrameDataBinding::DocumentUnloaded(Rml::ElementDocument* document)
{
	Rml::ElementList elements;
	document->QuerySelectorAll(elements, TOOLTIP_SELECTOR);

	for ( Rml::Element* element : elements )
	{
		element->RemoveEventListener(Rml::EventId::Mouseover, &m_TooltipListener);
		element->RemoveEventListener(Rml::EventId::Mouseout, &m_TooltipListener);
	}
}

void MenuFrameDataBinding::HandleMouseEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Mouseover:
		{
			SetTooltip(event);
			event.StopPropagation();
			break;
		}

		case Rml::EventId::Mouseout:
		{
			ClearTooltip();
			event.StopPropagation();
			break;
		}

		default:
		{
			break;
		}
	}
}

void MenuFrameDataBinding::SetTooltip(Rml::Event& event)
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

	if ( tooltipAttr->GetInto(m_Tooltip.value) && m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(m_Tooltip.name);
	}
}

void MenuFrameDataBinding::ClearTooltip()
{
	if ( !m_Tooltip.value.empty() )
	{
		m_Tooltip.value.clear();

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(m_Tooltip.name);
		}
	}
}
