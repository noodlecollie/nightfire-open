#include "components/TooltipComponent.h"
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

TooltipComponent::TooltipComponent(BaseMenu* parentMenu, Rml::String displayElementID, Rml::String dataVarName) :
	BaseComponent(parentMenu, displayElementID),
	m_TooltipText {std::move(dataVarName), ""},
	m_DocumentListener(parentMenu, this, &TooltipComponent::HandleDocumentHide, {Rml::EventId::Hide}),
	m_TooltipListener(
		parentMenu,
		this,
		&TooltipComponent::HandleMouseEvents,
		"bigbutton[tooltip], button[tooltip], label[tooltip], .hover-tooltip[tooltip]",
		{Rml::EventId::Mouseover, Rml::EventId::Mouseout}
	),
	m_DisplayElementID(std::move(displayElementID))
{
}

void TooltipComponent::SetHintInnerRml(const Rml::String& rml)
{
	if ( m_TooltipDisplayElement )
	{
		m_TooltipDisplayElement->SetInnerRML(rml);
	}
	else
	{
		ASSERT(false);
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "SetHintInnerRml: Hint display element was null");
	}
}

Rml::String TooltipComponent::DefaultTooltipText() const
{
	return m_DefaultTooltipText;
}

void TooltipComponent::SetDefaultTooltipText(Rml::String text)
{
	m_DefaultTooltipText = std::move(text);

	if ( !m_CurrentTooltipSourceElement )
	{
		// Call this if we're not currently showing a tooltip
		ResetTooltip();
	}
}

bool TooltipComponent::ComponentLoadFromDocument(Rml::ElementDocument* document)
{
	m_TooltipDisplayElement = document->QuerySelector("#" + m_DisplayElementID);

	if ( !m_TooltipDisplayElement )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Could not find tooltip element with ID #%s",
			m_DisplayElementID.c_str()
		);

		return false;
	}

	return true;
}

void TooltipComponent::ComponentUnload()
{
	m_TooltipDisplayElement = nullptr;
}

bool TooltipComponent::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_TooltipText.name, &m_TooltipText.value) )
	{
		return false;
	}

	return true;
}

void TooltipComponent::HandleDocumentHide(Rml::Event&)
{
	// The document is being hidden, so forcibly clear the tooltip.
	ResetTooltip();
}

void TooltipComponent::HandleMouseEvents(Rml::Event& event)
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

			if ( element && element == m_CurrentTooltipSourceElement )
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

void TooltipComponent::SetTooltip(Rml::Event& event)
{
	if ( m_CurrentTooltipSourceElement )
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

	if ( tooltipAttr->GetInto(m_TooltipText.value) && IsModelLoaded() )
	{
		DirtyVariable(m_TooltipText.name);
		m_CurrentTooltipSourceElement = element;
	}
}

void TooltipComponent::ResetTooltip()
{
	m_CurrentTooltipSourceElement = nullptr;

	if ( m_TooltipText.value != m_DefaultTooltipText )
	{
		m_TooltipText.value = m_DefaultTooltipText;
		DirtyVariable(m_TooltipText.name);
	}
}
