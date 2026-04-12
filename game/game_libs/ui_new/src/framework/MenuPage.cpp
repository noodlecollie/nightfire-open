#include "framework/MenuPage.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>
#include "rmlui/Utils.h"

MenuPage::MenuPage(const char* name, const char* rmlFilePath) :
	BaseMenu(name, rmlFilePath),
	m_KeyEventListener(this, &MenuPage::ProcessEvent)
{
}

bool MenuPage::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseMenu::OnSetUpDataModelBindings(constructor) &&
		constructor.BindEventCallback("pushMenu", &MenuPage::HandlePushMenu, this) &&
		constructor.BindEventCallback("popMenu", &MenuPage::HandlePopMenu, this);
}

bool MenuPage::RequestPopOnEscapeKey() const
{
	return m_RequestPopOnEscapeKey;
}

void MenuPage::SetRequestPopOnEscapeKey(bool enable)
{
	m_RequestPopOnEscapeKey = enable;
}

void MenuPage::ProcessEvent(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Keydown:
		{
			const int keyId = GetEventKeyId(event);

			if ( keyId == Rml::Input::KI_ESCAPE && m_RequestPopOnEscapeKey )
			{
				event.StopPropagation();
				SetCurrentRequest(MenuRequestType::PopMenu);
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void MenuPage::OnEndDocumentLoaded()
{
	BaseMenu::OnEndDocumentLoaded();

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Keydown, &m_KeyEventListener);
}

void MenuPage::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Keydown, &m_KeyEventListener);

	BaseMenu::OnBeginDocumentUnloaded();
}

void MenuPage::HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PushMenu, args);
}

void MenuPage::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PopMenu, args);
}
