#include "framework/MenuPage.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>

MenuPage::MenuPage(const char* name, const char* rmlFilePath) :
	BaseMenu(name, rmlFilePath)
{
}

bool MenuPage::SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseMenu::SetUpDefaultDataModelBindings(constructor) &&
		constructor.BindEventCallback("push_menu", &MenuPage::HandlePushMenu, this) &&
		constructor.BindEventCallback("pop_menu", &MenuPage::HandlePopMenu, this);
}

void MenuPage::DocumentLoaded(Rml::ElementDocument* document)
{
	BaseMenu::DocumentLoaded(document);

	document->AddEventListener(Rml::EventId::Keydown, this);
	document->AddEventListener(Rml::EventId::Keyup, this);
}

void MenuPage::DocumentUnloaded(Rml::ElementDocument* document)
{
	document->RemoveEventListener(Rml::EventId::Keydown, this);
	document->RemoveEventListener(Rml::EventId::Keyup, this);

	BaseMenu::DocumentUnloaded(document);
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

void MenuPage::HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PushMenu, args);
}

void MenuPage::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PopMenu, args);
}
