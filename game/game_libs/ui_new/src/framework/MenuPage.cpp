#include "framework/MenuPage.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>
#include "rmlui/Utils.h"

static constexpr const char* const EVENT_PUSH_MENU = "pushMenu";
static constexpr const char* const EVENT_POP_MENU = "popMenu";
static constexpr const char* const EVENT_CUT_STACK = "cutStack";

MenuPage::MenuPage(const char* name, const char* rmlFilePath) :
	BaseMenu(name, rmlFilePath),
	m_KeyEventListener(this, &MenuPage::ProcessKeyEvent)
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

void MenuPage::ProcessKeyEvent(Rml::Event& event)
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

	Rml::Dictionary options;

	if ( !menuToSwapIn.empty() )
	{
		options.insert({PopMenuRequest::OPTION_NEW_MENU, Rml::Variant(menuToSwapIn)});
	}

	SetCurrentRequest(PopMenuRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::RequestCutStack(size_t newSize, Rml::String menuToSwapIn)
{
	if ( !ShouldPop(menuToSwapIn) )
	{
		return;
	}

	Rml::Dictionary options;
	options.insert({CutStackRequest::OPTION_NEW_SIZE, Rml::Variant(newSize)});

	if ( !menuToSwapIn.empty() )
	{
		options.insert({CutStackRequest::OPTION_NEW_MENU, Rml::Variant(menuToSwapIn)});
	}

	SetCurrentRequest(CutStackRequest::REQUEST_TYPE, std::move(options));
}

bool MenuPage::ShouldPop(const Rml::String&) const
{
	return true;
}

void MenuPage::RequestSwitchFocus(Rml::String target, Rml::String newMenu)
{
	Rml::Dictionary options;
	options.insert({SwitchFocusRequest::OPTION_TARGET, Rml::Variant(std::move(target))});

	if ( !newMenu.empty() )
	{
		options.insert({SwitchFocusRequest::OPTION_NEW_MENU, Rml::Variant(std::move(newMenu))});
	}

	SetCurrentRequest(SwitchFocusRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Ignoring push menu data event which has no arguments provided");
		return;
	}

	Rml::Dictionary options;
	options.insert({PushMenuRequest::OPTION_MENU, args[0]});

	SetCurrentRequest(PushMenuRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	Rml::Dictionary options;

	if ( !args.empty() )
	{
		options.insert({PopMenuRequest::OPTION_NEW_MENU, args[0]});
	}

	SetCurrentRequest(PopMenuRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::HandleCutStack(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Ignoring cut stack data event which has no arguments provided");
		return;
	}

	Rml::Dictionary options;
	options.insert({CutStackRequest::OPTION_NEW_SIZE, args[0]});

	if ( args.size() > 1 )
	{
		options.insert({CutStackRequest::OPTION_NEW_MENU, args[1]});
	}

	SetCurrentRequest(CutStackRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::HandleSwitchFocus(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Ignoring switch focus event which has no arguments provided");
		return;
	}

	Rml::Dictionary options;
	options.insert({SwitchFocusRequest::OPTION_TARGET, args[0]});

	if ( args.size() > 1 )
	{
		options.insert({SwitchFocusRequest::OPTION_NEW_MENU, args[1]});
	}

	SetCurrentRequest(SwitchFocusRequest::REQUEST_TYPE, std::move(options));
}
