#include "components/ModalComponent.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/StringUtilities.h>
#include "framework/ElementFinder.h"

static constexpr const char* const PARAM_TITLE = "title";
static constexpr const char* const PARAM_BUTTONS = "buttons";

ModalComponent::ModalComponent(BaseMenu* parentMenu, Rml::String id) :
	BaseComponent(parentMenu, std::move(id)),
	m_ButtonEventListener(this, &ModalComponent::HandleButtonEvent)
{
	AddParamSpec(PARAM_TITLE, Rml::Variant(""));
	AddParamSpec(PARAM_BUTTONS, Rml::Variant(""));
}

void ModalComponent::SetTitle(const Rml::String& title)
{
	ASSERTSZ(m_Elems.modalHeader, "SetTitle called before modal elements were loaded");

	if ( !m_Elems.modalHeader )
	{
		return;
	}

	if ( !title.empty() )
	{
		m_Elems.modalHeader->SetInnerRML("<h2>" + Rml::StringUtilities::EncodeRml(title) + "</h2>");
	}
	else
	{
		m_Elems.modalHeader->SetInnerRML("");
	}
}

void ModalComponent::SetContentsRml(const Rml::String& rml)
{
	ASSERTSZ(m_Elems.modalBody, "SetContentsRml called before modal elements were loaded");

	if ( !m_Elems.modalBody )
	{
		return;
	}

	m_Elems.modalBody->SetInnerRML(rml);
}

void ModalComponent::SetButtons(const Rml::StringList& buttons)
{
	ASSERTSZ(m_Elems.modalFooter, "SetButtons called before modal elements were loaded");

	if ( !m_Elems.modalFooter )
	{
		return;
	}

	m_Elems.buttons.clear();

	while ( m_Elems.modalFooter->HasChildNodes() )
	{
		m_Elems.modalFooter->RemoveChild(m_Elems.modalFooter->GetFirstChild());
	}

	Rml::String rmlString = "";

	for ( const Rml::String& button : buttons )
	{
		rmlString += "<button class=\"primary\">" + Rml::StringUtilities::EncodeRml(button) + "</button>";
	}

	m_Elems.modalFooter->SetInnerRML(rmlString);
	m_Elems.buttons.reserve(m_Elems.modalFooter->GetNumChildren());

	for ( Rml::Element* child = m_Elems.modalFooter->GetFirstChild(); child; child = child->GetNextSibling() )
	{
		m_Elems.buttons.push_back(child);
		child->AddEventListener(Rml::EventId::Click, &m_ButtonEventListener);
		child->AddEventListener(Rml::EventId::Mouseup, &m_ButtonEventListener);
	}
}

void ModalComponent::SetButtonClickCallback(ButtonClickCallback callback)
{
	m_ButtonClickCallback = std::move(callback);
}

const Rml::Variant& ModalComponent::UserData() const
{
	return m_UserData;
}

void ModalComponent::SetUserData(Rml::Variant data)
{
	m_UserData = std::move(data);
}

bool ModalComponent::OnLoadFromDocument(Rml::ElementDocument*)
{
	ElementFinder finder;

	finder.Add(ComponentElementPtrPtr(), ".modal-shade", &m_Elems.shade);
	finder.Add(&m_Elems.shade, "modal", &m_Elems.modal);
	finder.Add(&m_Elems.modal, ".modal-header", &m_Elems.modalHeader);
	finder.Add(&m_Elems.modal, ".modal-body", &m_Elems.modalBody);
	finder.Add(&m_Elems.modal, ".modal-footer", &m_Elems.modalFooter);

	if ( !finder.FindAll() )
	{
		return false;
	}

	LoadParams();
	return true;
}

void ModalComponent::OnUnload()
{
	for ( Rml::Element* button : m_Elems.buttons )
	{
		button->RemoveEventListener(Rml::EventId::Click, &m_ButtonEventListener);
		button->RemoveEventListener(Rml::EventId::Mouseup, &m_ButtonEventListener);
	}

	m_Elems = Elements {};
}

void ModalComponent::LoadParams()
{
	SetTitle(GetParam(PARAM_TITLE).Get<Rml::String>());

	const Rml::String buttons = GetParam(PARAM_BUTTONS).Get<Rml::String>();

	if ( !buttons.empty() )
	{
		m_Elems.buttons.clear();

		Rml::StringList buttonsList;
		Rml::StringUtilities::ExpandString(buttonsList, buttons, ';');
		SetButtons(buttonsList);
	}
}

void ModalComponent::HandleButtonEvent(Rml::Event& event)
{
	if ( !m_ButtonClickCallback )
	{
		return;
	}

	Rml::Element* button = event.GetTargetElement();
	const auto buttonIt = std::find(m_Elems.buttons.begin(), m_Elems.buttons.end(), button);

	// We should only get events for buttons we know about.
	ASSERT(buttonIt != m_Elems.buttons.end());

	if ( buttonIt == m_Elems.buttons.end() )
	{
		return;
	}

	event.StopPropagation();
	m_ButtonClickCallback(event, buttonIt - m_Elems.buttons.begin(), m_UserData);
}
