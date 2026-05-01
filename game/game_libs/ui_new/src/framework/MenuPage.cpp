#include "framework/MenuPage.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>
#include "rmlui/Utils.h"

static constexpr const char* const EVENT_PUSH_MENU = "pushMenu";
static constexpr const char* const EVENT_POP_MENU = "popMenu";
static constexpr const char* const EVENT_CUT_STACK = "cutStack";

MenuPage::MenuPage(const char* name, const char* rmlFilePath) :
	BaseMenu(name, rmlFilePath),
	m_KeyEventListener(this, &MenuPage::ProcessEvent)
{
}

bool MenuPage::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseMenu::OnSetUpDataModelBindings(constructor) &&
		constructor.BindEventCallback(EVENT_PUSH_MENU, &MenuPage::HandlePushMenu, this) &&
		constructor.BindEventCallback(EVENT_POP_MENU, &MenuPage::HandlePopMenu, this) &&
		constructor.BindEventCallback(EVENT_CUT_STACK, &MenuPage::HandleCutStack, this);
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

			if ( keyId == Rml::Input::KI_ESCAPE && m_RequestPopOnEscapeKey && ShouldPop("") )
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

void MenuPage::RequestPop(Rml::String menuToSwapIn)
{
	if ( !ShouldPop(menuToSwapIn) )
	{
		return;
	}

	Rml::VariantList args;

	if ( !menuToSwapIn.empty() )
	{
		args.push_back(Rml::Variant(menuToSwapIn));
	}

	SetCurrentRequest(MenuRequestType::PopMenu, args);
}

void MenuPage::RequestCutStack(size_t newSize, Rml::String menuToSwapIn)
{
	if ( !ShouldPop(menuToSwapIn) )
	{
		return;
	}

	Rml::VariantList args;

	args.push_back(Rml::Variant(newSize));

	if ( !menuToSwapIn.empty() )
	{
		args.push_back(Rml::Variant(menuToSwapIn));
	}

	SetCurrentRequest(MenuRequestType::CutStack, args);
}

bool MenuPage::ShouldPop(const Rml::String&) const
{
	return true;
}

void MenuPage::HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PushMenu, args);
}

void MenuPage::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PopMenu, args);
}

void MenuPage::HandleCutStack(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::CutStack, args);
}
