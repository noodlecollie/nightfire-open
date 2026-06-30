#include "templatebindings/MenuFrameDataBinding.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

MenuFrameDataBinding::MenuFrameDataBinding(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_Tooltip {"mainMenuTooltip", ""},
	m_Title {"frameTitle", ""},
	m_DocumentListener(parentMenu, this, &MenuFrameDataBinding::HandleDocumentHide, {Rml::EventId::Hide}),
	m_TooltipListener(
		parentMenu,
		this,
		&MenuFrameDataBinding::HandleMouseEvents,
		"bigbutton[tooltip], button[tooltip], label[tooltip], .hover-tooltip[tooltip]",
		{Rml::EventId::Mouseover, Rml::EventId::Mouseout}
	)
{
}

void MenuFrameDataBinding::SetTooltipInnerRml(const Rml::String& rml)
{
	if ( m_TooltipDisplayElement )
	{
		m_TooltipDisplayElement->SetInnerRML(rml);
	}
	else
	{
		ASSERT(false);
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "SetTooltipInnerRml: Tooltip display element was null");
	}
}

Rml::String MenuFrameDataBinding::DefaultTooltipText() const
{
	return m_DefaultTooltipText;
}

void MenuFrameDataBinding::SetDefaultTooltipText(Rml::String text)
{
	m_DefaultTooltipText = std::move(text);

	if ( !m_CurrentTooltipElement )
	{
		// Call this if we're not currently showing a tooltip
		ResetTooltip();
	}
}

Rml::String MenuFrameDataBinding::Title() const
{
	return m_Title.value;
}

void MenuFrameDataBinding::SetTitle(Rml::String title)
{
	if ( m_Title.value != title )
	{
		m_Title.value = title;
		DirtyVariable(m_Title.name);
	}
}

void MenuFrameDataBinding::DocumentLoaded(Rml::ElementDocument* document)
{
	m_TooltipDisplayElement = document->QuerySelector("#main_menu_tooltip");

	if ( !m_TooltipDisplayElement )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Could not find menu frame tooltip element with ID #main_menu_tooltip"
		);
	}
}

void MenuFrameDataBinding::DocumentUnloaded(Rml::ElementDocument*)
{
	m_TooltipDisplayElement = nullptr;
}

bool MenuFrameDataBinding::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_Tooltip.name, &m_Tooltip.value) || !constructor.Bind(m_Title.name, &m_Title.value) )
	{
		return false;
	}

	return true;
}

void MenuFrameDataBinding::HandleDocumentHide(Rml::Event&)
{
	// The document is being hidden, so forcibly clear the tooltip.
	ResetTooltip();
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
				ResetTooltip();
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

	if ( tooltipAttr->GetInto(m_Tooltip.value) && IsModelLoaded() )
	{
		DirtyVariable(m_Tooltip.name);
		m_CurrentTooltipElement = element;
	}
}

void MenuFrameDataBinding::ResetTooltip()
{
	m_CurrentTooltipElement = nullptr;

	if ( m_Tooltip.value != m_DefaultTooltipText )
	{
		m_Tooltip.value = m_DefaultTooltipText;
		DirtyVariable(m_Tooltip.name);
	}
}
