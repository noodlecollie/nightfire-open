#include "templatebindings/MenuFrameDataBinding.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

// These are the elements that we support tooltips on:
static constexpr const char* const TOOLTIP_SELECTOR = "bigbutton[tooltip], button[tooltip], label[tooltip]";

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
	document->AddEventListener(Rml::EventId::Hide, &m_TooltipListener);

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

	document->RemoveEventListener(Rml::EventId::Hide, &m_TooltipListener);
}

void MenuFrameDataBinding::HandleMouseEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Mouseover:
		{
			SetTooltip(event);
			break;
		}

		case Rml::EventId::Mouseout:
		{
			Rml::Element* element = event.GetTargetElement();

			if ( element && element == m_CurrentTooltipElement )
			{
				ClearTooltip();
			}

			break;
		}

		case Rml::EventId::Hide:
		{
			// The document is being hidden, so forcibly clear the tooltip.
			ClearTooltip();
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
	if ( m_CurrentTooltipElement )
	{
		// We moused over another element inside the current one.
		// Don't allow setting the tooltip until the current
		// element clears it.
		return;
	}

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
		m_CurrentTooltipElement = element;
	}
}

void MenuFrameDataBinding::ClearTooltip()
{
	m_CurrentTooltipElement = nullptr;

	if ( !m_Tooltip.value.empty() )
	{
		m_Tooltip.value.clear();

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(m_Tooltip.name);
		}
	}
}
