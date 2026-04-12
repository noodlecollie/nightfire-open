#include "menus/KeyBindingsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include "EnginePublicAPI/keydefs.h"
#include "rmlui/Utils.h"
#include "rmlui/RmlUiBackend.h"
#include "UIDebug.h"
#include "udll_int.h"

static constexpr const char* const PROP_SHOW_MODAL = "showModal";
static constexpr const char* const PROP_CURRENT_ROW = "currentRow";
static constexpr const char* const PROP_CURRENT_BINDING = "currentBinding";
static constexpr const char* const EVENT_REBIND_KEY = "rebindKey";
static constexpr const char* const EVENT_SELECT_BINDING = "selectBinding";
static constexpr const char* const EVENT_CLEAR_BINDING = "clearBinding";
static constexpr const char* const EVENT_RESET_BINDING = "resetBindingToDefault";
static constexpr const char* const EVENT_RESET_ALL_BINDINGS = "resetAllBindingsToDefaults";

static constexpr const char* const RML_MODAL_SET_BINDING =
	"<p>Press a key, or press <keyname>Escape</keyname> to cancel</p>";
static constexpr const char* const RML_MODAL_CONFIRM_RESET_TO_DEFAULTS =
	"<p>Reset all key bindings to default values?</p>";

enum ModalUserData
{
	SETTING_BINDING,
	RESETTING_BINDINGS
};

KeyBindingsMenu::KeyBindingsMenu() :
	MenuPage("keybindings_menu", "resource/rml/keybindings_menu.rml"),
	m_TabBarDataBinding(OptionsTabBarDataBinding::TAB_KEYS),
	m_Modal(this, "keybindings_modal"),
	m_ShowHideEventListener(this, &KeyBindingsMenu::ProcessShowHideEvents),
	m_KeyEventListener(this, &KeyBindingsMenu::ProcessKeyEvents)
{
	m_Modal.SetButtonClickCallback(
		[this](Rml::Event&, size_t buttonIndex, const Rml::Variant& userData)
		{
			if ( userData.Get<int>() == RESETTING_BINDINGS )
			{
				ResetAllBindingsResponse(buttonIndex == 1);
			}
		}
	);
}

void KeyBindingsMenu::Update(float currentTime)
{
	MenuPage::Update(currentTime);

	if ( m_PageModel.showModal && RmlUiBackend::StaticInstance().HasStoredKey() )
	{
		SetStoredKeyForCurrentRebinding();
	}

	if ( m_TabBarDataBinding.ActiveTabChanged() &&
		 m_TabBarDataBinding.ActiveTab() != OptionsTabBarDataBinding::TAB_KEYS )
	{
		// TODO: We need to know the name of the menu to swap in here.
		RequestPop();
	}
}

bool KeyBindingsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_TabBarDataBinding.SetUpDataBindings(constructor) || !m_KeyBindings.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(PROP_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.Bind(PROP_CURRENT_ROW, &m_PageModel.currentRow) ||
		 !constructor.Bind(PROP_CURRENT_BINDING, &m_PageModel.currentBinding) ||
		 !constructor.BindEventCallback(EVENT_REBIND_KEY, &KeyBindingsMenu::HandleRebindKeyEvent, this) ||
		 !constructor.BindEventCallback(EVENT_SELECT_BINDING, &KeyBindingsMenu::HandleSelectBindingEvent, this) ||
		 !constructor.BindEventCallback(EVENT_CLEAR_BINDING, &KeyBindingsMenu::HandleClearBinding, this) ||
		 !constructor.BindEventCallback(EVENT_RESET_BINDING, &KeyBindingsMenu::HandleResetBindingToDefault, this) ||
		 !constructor
			  .BindEventCallback(EVENT_RESET_ALL_BINDINGS, &KeyBindingsMenu::HandleResetAllBindingsToDefaults, this) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();

	return true;
}

void KeyBindingsMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Keydown, &m_KeyEventListener);
}

void KeyBindingsMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Keydown, &m_KeyEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void KeyBindingsMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			m_TabBarDataBinding.SetActiveTab(OptionsTabBarDataBinding::TAB_KEYS);
			m_KeyBindings.ReloadAndApplyBindings(true, true);
			ResetRebindingRow();
			break;
		}

		case Rml::EventId::Hide:
		{
			// Reset this from whatever the tabs might have selected.
			m_TabBarDataBinding.SetActiveTab(OptionsTabBarDataBinding::TAB_KEYS);

			ResetRebindingRow();
			m_KeyBindings.WriteBindings();
			break;
		}

		default:
		{
			break;
		}
	}
}

void KeyBindingsMenu::ProcessKeyEvents(Rml::Event& event)
{
	ASSERT(event.GetId() == Rml::EventId::Keydown);

	if ( !m_PageModel.showModal )
	{
		return;
	}

	if ( GetEventKeyId(event) == Rml::Input::KI_ESCAPE )
	{
		switch ( m_Modal.UserData().Get<int>() )
		{
			case SETTING_BINDING:
			{
				ResetRebindingRow();
				break;
			}

			case RESETTING_BINDINGS:
			{
				ResetAllBindingsResponse(false);
				break;
			}

			default:
			{
				break;
			}
		}
	}
}

void KeyBindingsMenu::HandleRebindKeyEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments)
{
	if ( arguments.size() < 2 )
	{
		ASSERT(false);
		return;
	}

	int row = INVALID_ROW;
	int bindIndex = INVALID_BINDING;

	if ( !arguments[0].GetInto(row) || !arguments[1].GetInto(bindIndex) )
	{
		ASSERT(false);
		return;
	}

	HandleRebindKeyEvent(row, bindIndex);
}

void KeyBindingsMenu::HandleSelectBindingEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments)
{
	if ( arguments.size() < 2 )
	{
		ASSERT(false);
		return;
	}

	int row = INVALID_ROW;
	int bindIndex = INVALID_BINDING;

	if ( !arguments[0].GetInto(row) || !arguments[1].GetInto(bindIndex) )
	{
		ASSERT(false);
		return;
	}

	HandleSelectBindingEvent(row, bindIndex);
}

void KeyBindingsMenu::HandleClearBinding(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_PageModel.currentRow < 0 || m_PageModel.currentBinding < 0 )
	{
		return;
	}

	m_KeyBindings.ClearBinding(static_cast<size_t>(m_PageModel.currentRow), m_PageModel.currentBinding == 0);
	m_KeyBindings.WriteBindings();
}

void KeyBindingsMenu::HandleResetBindingToDefault(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_PageModel.currentRow < 0 || m_PageModel.currentBinding < 0 )
	{
		return;
	}

	m_KeyBindings.ResetBindingToDefault(static_cast<size_t>(m_PageModel.currentRow));
	m_KeyBindings.WriteBindings();
}

void KeyBindingsMenu::HandleResetAllBindingsToDefaults(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	Rml::StringList buttons;
	buttons.push_back("Cancel");
	buttons.push_back("OK");

	m_Modal.SetTitle("Reset All Bindings");
	m_Modal.SetContentsRml(RML_MODAL_CONFIRM_RESET_TO_DEFAULTS);
	m_Modal.SetButtons(buttons);
	m_Modal.SetUserData(Rml::Variant(RESETTING_BINDINGS));

	ShowModal(true);
	SetRequestPopOnEscapeKey(false);
}

void KeyBindingsMenu::HandleRebindKeyEvent(int row, int bindIndex)
{
	if ( !HandleSelectBindingEvent(row, bindIndex) )
	{
		return;
	}

	m_Modal.SetTitle("Set Binding");
	m_Modal.SetContentsRml(RML_MODAL_SET_BINDING);
	m_Modal.SetButtons({});
	m_Modal.SetUserData(Rml::Variant(SETTING_BINDING));

	ShowModal(true);
	SetRequestPopOnEscapeKey(false);
	RmlUiBackend::StaticInstance().SetStoreNextKey(true);
}

bool KeyBindingsMenu::HandleSelectBindingEvent(int row, int bindIndex)
{
	if ( bindIndex != 0 && bindIndex != 1 )
	{
		ASSERT(false);
		ResetRebindingRow();
		return false;
	}

	if ( row < 0 || static_cast<size_t>(row) >= m_KeyBindings.Rows() )
	{
		ASSERT(false);
		ResetRebindingRow();
		return false;
	}

	if ( m_PageModel.currentRow != row )
	{
		m_PageModel.currentRow = row;
		m_ModelHandle.DirtyVariable(PROP_CURRENT_ROW);
	}

	if ( m_PageModel.currentBinding != bindIndex )
	{
		m_PageModel.currentBinding = bindIndex;
		m_ModelHandle.DirtyVariable(PROP_CURRENT_BINDING);
	}

	return true;
}

void KeyBindingsMenu::ResetRebindingRow()
{
	if ( m_PageModel.currentRow >= 0 )
	{
		m_PageModel.currentRow = INVALID_ROW;
		m_ModelHandle.DirtyVariable(PROP_CURRENT_ROW);
	}

	if ( m_PageModel.currentBinding >= 0 )
	{
		m_PageModel.currentBinding = INVALID_BINDING;
		m_ModelHandle.DirtyVariable(PROP_CURRENT_BINDING);
	}

	CloseModalAndStopListeningForKeys();
}

void KeyBindingsMenu::CloseModalAndStopListeningForKeys()
{
	ShowModal(false);
	SetRequestPopOnEscapeKey(true);
	RmlUiBackend::StaticInstance().ClearStoreNextKey();
}

void KeyBindingsMenu::ShowModal(bool show)
{
	if ( m_PageModel.showModal != show )
	{
		m_PageModel.showModal = show;
		m_ModelHandle.DirtyVariable(PROP_SHOW_MODAL);
	}
}

void KeyBindingsMenu::SetStoredKeyForCurrentRebinding()
{
	const RmlUiBackend::StoredKey storedKey = RmlUiBackend::StaticInstance().TakeStoredKey();

	// We shouldn't get these values
	ASSERT(storedKey.key != -1);
	ASSERT(storedKey.key != K_ESCAPE);
	ASSERT(m_PageModel.currentBinding == 0 || m_PageModel.currentBinding == 1);

	// Sanity:
	if ( storedKey.key == -1 || storedKey.key == K_ESCAPE )
	{
		ResetRebindingRow();
		return;
	}

	const char* keyStr = gEngfuncs.pfnKeynumToString(storedKey.key);
	ASSERT(keyStr && *keyStr);

	if ( !keyStr || !(*keyStr) )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Could not get key string for key %d", storedKey.key);
		CloseModalAndStopListeningForKeys();
		return;
	}

	m_KeyBindings.SetBinding(static_cast<size_t>(m_PageModel.currentRow), m_PageModel.currentBinding == 0, keyStr);
	m_KeyBindings.WriteBindings();
	CloseModalAndStopListeningForKeys();
}

void KeyBindingsMenu::ResetAllBindingsResponse(bool shouldReset)
{
	ShowModal(false);
	SetRequestPopOnEscapeKey(true);

	if ( shouldReset )
	{
		m_KeyBindings.ResetAllBindingsToDefaults();
		m_KeyBindings.WriteBindings();
	}
}
