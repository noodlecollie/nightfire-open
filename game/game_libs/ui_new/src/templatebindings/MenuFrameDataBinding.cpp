#include "templatebindings/MenuFrameDataBinding.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

MenuFrameDataBinding::MenuFrameDataBinding(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_Tooltip {"footerTooltip", ""},
	m_DocumentListener(parentMenu, this, &MenuFrameDataBinding::HandleDocumentHide, {Rml::EventId::Hide}),
	m_TooltipListener(
		parentMenu,
		this,
		&MenuFrameDataBinding::HandleMouseEvents,
		"bigbutton[tooltip], button[tooltip], label[tooltip]",
		{Rml::EventId::Mouseover, Rml::EventId::Mouseout}
	)
{
}

bool MenuFrameDataBinding::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_Tooltip.name, &m_Tooltip.value) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void MenuFrameDataBinding::HandleDocumentHide(Rml::Event&)
{
	// The document is being hidden, so forcibly clear the tooltip.
	ClearTooltip();
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
