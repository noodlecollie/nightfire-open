#include "framework/MenuPage.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>

MenuPage::MenuPage(const char* name, const char* rmlFilePath) :
	BaseMenu(name, rmlFilePath),
	m_KeyEventListener(this, &MenuPage::ProcessEvent)
{
}

bool MenuPage::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseMenu::OnSetUpDataModelBindings(constructor) &&
		constructor.BindEventCallback("push_menu", &MenuPage::HandlePushMenu, this) &&
		constructor.BindEventCallback("pop_menu", &MenuPage::HandlePopMenu, this);
}

void MenuPage::ProcessEvent(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Keydown:
		{
			const int keyId = event.GetParameter<int>("key_identifier", 0);

			if ( keyId == Rml::Input::KI_ESCAPE )
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
