#include "framework/MenuPage.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>
#include "rmlui/Utils.h"
#include "udll_int.h"

static constexpr const char* const EVENT_PUSH_MENU = "pushMenu";
static constexpr const char* const EVENT_POP_MENU = "popMenu";
static constexpr const char* const EVENT_CUT_STACK = "cutStack";

MenuPage::MenuPage(const char* name, const char* rmlFilePath) :
	BaseMenu(name, rmlFilePath),
	m_KeyEventListener(this, &MenuPage::ProcessKeyEvent, {Rml::EventId::Keydown, Rml::EventId::Keyup})
{
}

bool MenuPage::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseMenu::OnSetUpDataModelBindings(constructor) &&
		constructor.BindEventCallback(EVENT_PUSH_MENU, &MenuPage::HandlePushMenu, this) &&
		constructor.BindEventCallback(EVENT_POP_MENU, &MenuPage::HandlePopMenu, this) &&
		constructor.BindEventCallback(EVENT_CUT_STACK, &MenuPage::HandleCutStack, this);
}

MenuPage::ActionOnEscape MenuPage::GetActionOnEscapeKey() const
{
	return m_ActionOnEscape;
}

void MenuPage::SetActionOnEscapeKey(ActionOnEscape action)
{
	m_ActionOnEscape = action;
}

void MenuPage::ProcessKeyEvent(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Keydown:
		{
			const int keyId = GetEventKeyId(event);

			if ( keyId == Rml::Input::KI_ESCAPE && m_ActionOnEscape != ActionOnEscape::NONE )
			{
				Rml::String targetToSwitchTo;

				if ( m_ActionOnEscape == ActionOnEscape::SWITCH_FOCUS )
				{
					targetToSwitchTo = gEngfuncs.pfnClientInGame() ? "game" : "console";
				}

				if ( ShouldAllowMenuRequest(targetToSwitchTo) )
				{
					event.StopPropagation();
					ProcessActionOnEscape(targetToSwitchTo);
				}
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void MenuPage::RequestPush(Rml::String menuToPush)
{
	Rml::Dictionary options;
	options.insert({PushMenuRequest::OPTION_MENU, Rml::Variant(std::move(menuToPush))});

	SetCurrentRequest(PushMenuRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::RequestPop(Rml::String menuToSwapIn)
{
	if ( !ShouldAllowMenuRequest(menuToSwapIn) )
	{
		return;
	}

	Rml::Dictionary options;

	if ( !menuToSwapIn.empty() )
	{
		options.insert({PopMenuRequest::OPTION_NEW_MENU, Rml::Variant(std::move(menuToSwapIn))});
	}

	SetCurrentRequest(PopMenuRequest::REQUEST_TYPE, std::move(options));
}

void MenuPage::RequestCutStack(size_t newSize, Rml::String menuToSwapIn)
{
	if ( !ShouldAllowMenuRequest(menuToSwapIn) )
	{
		return;
	}

	Rml::Dictionary options;
	options.insert({CutStackRequest::OPTION_NEW_SIZE, Rml::Variant(newSize)});

	if ( !menuToSwapIn.empty() )
	{
		options.insert({CutStackRequest::OPTION_NEW_MENU, Rml::Variant(std::move(menuToSwapIn))});
	}

	SetCurrentRequest(CutStackRequest::REQUEST_TYPE, std::move(options));
}

bool MenuPage::ShouldAllowMenuRequest(const Rml::String&)
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

	RequestPush(args[0].Get<Rml::String>());
}

void MenuPage::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	RequestPop(!args.empty() ? args[0].Get<Rml::String>() : "");
}

void MenuPage::HandleCutStack(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Ignoring cut stack data event which has no arguments provided");
		return;
	}

	size_t newSize = 0;
	if ( !args[0].GetInto(newSize) )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Ignoring cut stack data event with unparseable stack size (must be >= 0)"
		);

		return;
	}

	RequestCutStack(newSize, args.size() > 1 ? args[1].Get<Rml::String>() : "");
}

void MenuPage::HandleSwitchFocus(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Ignoring switch focus event which has no arguments provided");
		return;
	}

	RequestSwitchFocus(args[0].Get<Rml::String>(), args.size() > 1 ? args[1].Get<Rml::String>() : "");
}

void MenuPage::ProcessActionOnEscape(const Rml::String& targetToSwitchTo)
{
	switch ( m_ActionOnEscape )
	{
		case ActionOnEscape::NONE:
		{
			// Do nothing.
			break;
		}

		case ActionOnEscape::POP:
		{
			RequestPop(targetToSwitchTo);
			break;
		}

		case ActionOnEscape::SWITCH_FOCUS:
		{
			RequestSwitchFocus(targetToSwitchTo);
			break;
		}

		default:
		{
			ASSERT(false);
			break;
		}
	}
}
