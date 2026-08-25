#include "templatebindings/HintDisplay.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

HintDisplay::HintDisplay(BaseMenu* parentMenu, Rml::String displayElementID, Rml::String dataVarName) :
	BaseMenuObserver(parentMenu),
	m_HintText {std::move(dataVarName), ""},
	m_DocumentListener(parentMenu, this, &HintDisplay::HandleDocumentHide, {Rml::EventId::Hide}),
	m_HintListener(
		parentMenu,
		this,
		&HintDisplay::HandleMouseEvents,
		"bigbutton[tooltip], button[tooltip], label[tooltip], .hover-tooltip[tooltip]",
		{Rml::EventId::Mouseover, Rml::EventId::Mouseout}
	),
	m_DisplayElementID(std::move(displayElementID))
{
}

void HintDisplay::SetHintInnerRml(const Rml::String& rml)
{
	if ( m_HintDisplayElement )
	{
		m_HintDisplayElement->SetInnerRML(rml);
	}
	else
	{
		ASSERT(false);
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "SetHintInnerRml: Hint display element was null");
	}
}

Rml::String HintDisplay::DefaultHintText() const
{
	return m_DefaultHintText;
}

void HintDisplay::SetDefaultHintText(Rml::String text)
{
	m_DefaultHintText = std::move(text);

	if ( !m_CurrentHintElement )
	{
		// Call this if we're not currently showing a tooltip
		ResetHint();
	}
}

void HintDisplay::DocumentLoaded(Rml::ElementDocument* document)
{
	m_HintDisplayElement = document->QuerySelector("#" + m_DisplayElementID);

	if ( !m_HintDisplayElement )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Could not find menu hint element with ID #%s",
			m_DisplayElementID.c_str()
		);
	}
}

void HintDisplay::DocumentUnloaded(Rml::ElementDocument*)
{
	m_HintDisplayElement = nullptr;
}

bool HintDisplay::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_HintText.name, &m_HintText.value) )
	{
		return false;
	}

	return true;
}

void HintDisplay::HandleDocumentHide(Rml::Event&)
{
	// The document is being hidden, so forcibly clear the tooltip.
	ResetHint();
}

void HintDisplay::HandleMouseEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Mouseover:
		{
			SetHint(event);
			break;
		}

		case Rml::EventId::Mouseout:
		{
			Rml::Element* element = event.GetTargetElement();

			if ( element && element == m_CurrentHintElement )
			{
				ResetHint();
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void HintDisplay::SetHint(Rml::Event& event)
{
	if ( m_CurrentHintElement )
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

	if ( tooltipAttr->GetInto(m_HintText.value) && IsModelLoaded() )
	{
		DirtyVariable(m_HintText.name);
		m_CurrentHintElement = element;
	}
}

void HintDisplay::ResetHint()
{
	m_CurrentHintElement = nullptr;

	if ( m_HintText.value != m_DefaultHintText )
	{
		m_HintText.value = m_DefaultHintText;
		DirtyVariable(m_HintText.name);
	}
}
