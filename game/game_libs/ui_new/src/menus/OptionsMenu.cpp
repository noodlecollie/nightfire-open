#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include "EnginePublicAPI/keydefs.h"
#include "rmlui/Utils.h"
#include "rmlui/RmlUiBackend.h"
#include "UIDebug.h"
#include "udll_int.h"

static constexpr const char* const PROP_ACTIVE_TAB = "activeTab";
static constexpr const char* const PROP_SHOW_MODAL = "showModal";
static constexpr const char* const PROP_CURRENT_ROW = "currentRow";
static constexpr const char* const PROP_CURRENT_BINDING = "currentBinding";
static constexpr const char* const EVENT_REBIND_KEY = "rebindKey";
static constexpr const char* const EVENT_SELECT_BINDING = "selectBinding";
static constexpr const char* const EVENT_CLEAR_BINDING = "clearBinding";
static constexpr const char* const EVENT_RESET_BINDING = "resetBindingToDefault";
static constexpr const char* const EVENT_RESET_ALL_BINDINGS = "resetAllBindingsToDefaults";

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml"),
	m_Modal(this, "options_modal"),
	m_ShowHideEventListener(this, &OptionsMenu::ProcessShowHideEvents),
	m_KeyEventListener(this, &OptionsMenu::ProcessKeyEvents)
{
}

void OptionsMenu::Update(float currentTime)
{
	MenuPage::Update(currentTime);

	if ( m_PageModel.showModal && RmlUiBackend::StaticInstance().HasStoredKey() )
	{
		SetStoredKeyForCurrentRebinding();
	}
}

bool OptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_KeyBindings.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(PROP_ACTIVE_TAB, &m_PageModel.activeTab) ||
		 !constructor.Bind(PROP_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.Bind(PROP_CURRENT_ROW, &m_PageModel.currentRow) ||
		 !constructor.Bind(PROP_CURRENT_BINDING, &m_PageModel.currentBinding) ||
		 !constructor.BindEventCallback(EVENT_REBIND_KEY, &OptionsMenu::HandleRebindKeyEvent, this) ||
		 !constructor.BindEventCallback(EVENT_SELECT_BINDING, &OptionsMenu::HandleSelectBindingEvent, this) ||
		 !constructor.BindEventCallback(EVENT_CLEAR_BINDING, &OptionsMenu::HandleClearBinding, this) ||
		 !constructor.BindEventCallback(EVENT_RESET_BINDING, &OptionsMenu::HandleResetBindingToDefault, this) ||
		 !constructor
			  .BindEventCallback(EVENT_RESET_ALL_BINDINGS, &OptionsMenu::HandleResetAllBindingsToDefaults, this) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();

	return true;
}

void OptionsMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Keydown, &m_KeyEventListener);
}

void OptionsMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Keydown, &m_KeyEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void OptionsMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			m_KeyBindings.ReloadAndApplyBindings(true, true);
			ResetRebindingRow();
			break;
		}

		case Rml::EventId::Hide:
		{
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

void OptionsMenu::ProcessKeyEvents(Rml::Event& event)
{
	ASSERT(event.GetId() == Rml::EventId::Keydown);

	if ( !m_PageModel.showModal )
	{
		return;
	}

	if ( GetEventKeyId(event) == Rml::Input::KI_ESCAPE )
	{
		ResetRebindingRow();
	}
}

void OptionsMenu::HandleRebindKeyEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments)
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

void OptionsMenu::HandleSelectBindingEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments)
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

void OptionsMenu::HandleClearBinding(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_PageModel.currentRow < 0 || m_PageModel.currentBinding < 0 )
	{
		return;
	}

	m_KeyBindings.ClearBinding(static_cast<size_t>(m_PageModel.currentRow), m_PageModel.currentBinding == 0);
}

void OptionsMenu::HandleResetBindingToDefault(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_PageModel.currentRow < 0 || m_PageModel.currentBinding < 0 )
	{
		return;
	}

	m_KeyBindings.ResetBindingToDefault(static_cast<size_t>(m_PageModel.currentRow));
}

void OptionsMenu::HandleResetAllBindingsToDefaults(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	// TODO: Need the modal added here
	m_KeyBindings.ResetAllBindingsToDefaults();
}

void OptionsMenu::HandleRebindKeyEvent(int row, int bindIndex)
{
	if ( !HandleSelectBindingEvent(row, bindIndex) )
	{
		return;
	}

	ShowModal(true);
	SetRequestPopOnEscapeKey(false);
	RmlUiBackend::StaticInstance().SetStoreNextKey(true);
}

bool OptionsMenu::HandleSelectBindingEvent(int row, int bindIndex)
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

void OptionsMenu::ResetRebindingRow()
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

void OptionsMenu::CloseModalAndStopListeningForKeys()
{
	ShowModal(false);
	SetRequestPopOnEscapeKey(true);
	RmlUiBackend::StaticInstance().ClearStoreNextKey();
}

void OptionsMenu::ShowModal(bool show)
{
	if ( m_PageModel.showModal != show )
	{
		m_PageModel.showModal = show;
		m_ModelHandle.DirtyVariable(PROP_SHOW_MODAL);
	}
}

void OptionsMenu::SetStoredKeyForCurrentRebinding()
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
