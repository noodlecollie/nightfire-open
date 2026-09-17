#include "templatebindings/MenuFrameDataBinding.h"
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

MenuFrameDataBinding::MenuFrameDataBinding(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_HintText {"menuHintText", ""},
	m_DocumentListener(parentMenu, this, &MenuFrameDataBinding::HandleDocumentHide, {Rml::EventId::Hide}),
	m_HintListener(
		parentMenu,
		this,
		&MenuFrameDataBinding::HandleMouseEvents,
		"bigbutton[tooltip], button[tooltip], label[tooltip], .hover-tooltip[tooltip]",
		{Rml::EventId::Mouseover, Rml::EventId::Mouseout}
	)
{
}

void MenuFrameDataBinding::SetHintInnerRml(const Rml::String& rml)
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

Rml::String MenuFrameDataBinding::DefaultHintText() const
{
	return m_DefaultHintText;
}

void MenuFrameDataBinding::SetDefaultHintText(Rml::String text)
{
	m_DefaultHintText = std::move(text);

	if ( !m_CurrentHintElement )
	{
		// Call this if we're not currently showing a tooltip
		ResetHint();
	}
}

void MenuFrameDataBinding::DocumentLoaded(Rml::ElementDocument* document)
{
	static constexpr const char* const HINT_AREA_ID = "#main_menu_hint_area";

	m_HintDisplayElement = document->QuerySelector(HINT_AREA_ID);

	if ( !m_HintDisplayElement )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Could not find menu hint element with ID %s", HINT_AREA_ID);
	}
}

void MenuFrameDataBinding::DocumentUnloaded(Rml::ElementDocument*)
{
	m_HintDisplayElement = nullptr;
}

bool MenuFrameDataBinding::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_HintText.name, &m_HintText.value) )
	{
		return false;
	}

	return true;
}

void MenuFrameDataBinding::HandleDocumentHide(Rml::Event&)
{
	// The document is being hidden, so forcibly clear the tooltip.
	ResetHint();
}

void MenuFrameDataBinding::HandleMouseEvents(Rml::Event& event)
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

void MenuFrameDataBinding::SetHint(Rml::Event& event)
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

void MenuFrameDataBinding::ResetHint()
{
	m_CurrentHintElement = nullptr;

	if ( m_HintText.value != m_DefaultHintText )
	{
		m_HintText.value = m_DefaultHintText;
		DirtyVariable(m_HintText.name);
	}
}
