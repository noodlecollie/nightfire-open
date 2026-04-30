#include "menus/MultiplayerMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "rmlui/RmlUiBackend.h"
#include "udll_int.h"

static constexpr const char* const NAME_SORT_TYPE = "sortType";
static constexpr const char* const NAME_SELECTED_ROW = "selectedRow";
static constexpr const char* const EVENT_SORT = "sort";
static constexpr const char* const EVENT_SELECT_SERVER = "selectServer";
static constexpr const char* const EVENT_CONNECT = "connectToSelectedServer";

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml"),
	m_MenuFrameDataBinding(this),
	m_ShowHideEventListener(this, &MultiplayerMenu::ProcessShowHideEvents)
{
	ReSortServerModel();
}

void MultiplayerMenu::Update(float /* currentTime */)
{
	// TODO: Periodic refresh of servers in the model
}

bool MultiplayerMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_ServerModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_SORT_TYPE, &m_PageModel.sortType) ||
		 !constructor.Bind(NAME_SELECTED_ROW, &m_PageModel.selectedRow) ||
		 !constructor.BindEventCallback(EVENT_SORT, &MultiplayerMenu::HandleColumnSortRequested, this) ||
		 !constructor.BindEventCallback(EVENT_SELECT_SERVER, &MultiplayerMenu::HandleSelectServer, this) ||
		 !constructor.BindEventCallback(EVENT_CONNECT, &MultiplayerMenu::HandleConnectToSelectedServer, this) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void MultiplayerMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	RmlUiBackend::StaticInstance().SetDiscoveredServerCallback(
		[this](const netadr_t& address, Rml::String&& info)
		{
			AddServerToList(address, std::move(info));
		}
	);

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
}

void MultiplayerMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void MultiplayerMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			RmlUiBackend::StaticInstance().SetDiscoveredServerCallback(
				[this](const netadr_t& address, Rml::String&& info)
				{
					AddServerToList(address, std::move(info));
				}
			);

			// TODO: Do we want an option for internet servers?
			// It'd be good to implement the logic at least, even if
			// we don't allow the user to query them yet.
			RefreshServersLocal();
			break;
		};

		case Rml::EventId::Hide:
		{
			RmlUiBackend::StaticInstance().ClearDiscoveredServerCallback();

			m_PageModel.selectedRow = INVALID_ROW;

			if ( m_ModelHandle )
			{
				m_ModelHandle.DirtyVariable(NAME_SELECTED_ROW);
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void MultiplayerMenu::AddServerToList(const netadr_t& address, Rml::String&& info)
{
	if ( m_ServerModel.Add(address, std::move(info)) )
	{
		ReSortServerModel();
	}
}

void MultiplayerMenu::HandleColumnSortRequested(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		return;
	}

	const Rml::Variant& sortTypeArg = args[0];
	Rml::String sortType;

	if ( !sortTypeArg.GetInto(sortType) )
	{
		return;
	}

	ReSortServerModel(sortType);
}

void MultiplayerMenu::HandleSelectServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		return;
	}

	const Rml::Variant& indexArg = args[0];
	int index;

	if ( !indexArg.GetInto(index) )
	{
		return;
	}

	if ( m_PageModel.selectedRow != index )
	{
		m_PageModel.selectedRow = index;

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(NAME_SELECTED_ROW);
		}
	}
}

void MultiplayerMenu::HandleConnectToSelectedServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_PageModel.selectedRow == INVALID_ROW )
	{
		return;
	}

	const ServerModel::Entry* entry = m_ServerModel.GetEntry(m_PageModel.selectedRow);

	if ( !entry )
	{
		ASSERT(false);
		return;
	}

	if ( entry->hasPassword )
	{
		// TODO: Modal to enter password
		ASSERT(false);
		return;
	}

	// No password required.
	gEngfuncs.pfnCvarSetString("password", "");

	// TODO: Do we need to cater for legacy servers here?
	// I don't think so at the moment.
	Rml::String connectCmd;
	Rml::FormatString(connectCmd, "connect %s", gTextfuncs.pfnAdrToString(entry->address));

	gEngfuncs.pfnClientCmd(0, connectCmd.c_str());

	// TODO: Init up the connection progress dialogue and pop this menu.
}

void MultiplayerMenu::ReSortServerModel(const Rml::String& sortTypeStr)
{
	if ( !sortTypeStr.empty() )
	{
		ServerModel::SortType newSortBy = ServerModel::SortType::PING;

		if ( !ServerModel::SortTypeFromString(sortTypeStr, newSortBy) )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_WARNING,
				"MultiplayerMenu::ReSortServerModel: Unrecognised sort type \"%s\"",
				sortTypeStr.c_str()
			);

			return;
		}

		// If the sort type has changed, re-sort ascending with the new type.
		// If it hasn't changed, re-sort with the inverse direction.

		if ( m_PageModel.sortType.empty() || m_PageModel.sortBy != newSortBy )
		{
			m_PageModel.sortAscending = true;
			m_PageModel.sortBy = newSortBy;
		}
		else
		{
			m_PageModel.sortAscending = !m_PageModel.sortAscending;
		}
	}

	bool reselectRow = false;
	netadr_t selectedAddress = {};

	if ( m_PageModel.selectedRow >= 0 )
	{
		const ServerModel::Entry* entry = m_ServerModel.GetEntry(m_PageModel.selectedRow);

		if ( entry )
		{
			selectedAddress = entry->address;
			reselectRow = true;
		}
	}

	m_ServerModel.Sort(m_PageModel.sortBy, m_PageModel.sortAscending);
	UpdateSortTypeVariable();

	if ( reselectRow )
	{
		size_t row = 0;
		m_PageModel.selectedRow =
			m_ServerModel.GetRowForAddress(selectedAddress, row) ? static_cast<int>(row) : INVALID_ROW;

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(NAME_SELECTED_ROW);
		}
	}
}

void MultiplayerMenu::UpdateSortTypeVariable()
{
	Rml::String newStr;

	if ( !ServerModel::SortTypeToString(m_PageModel.sortBy, m_PageModel.sortAscending, newStr) )
	{
		ASSERT(false);
		return;
	}

	m_PageModel.sortType = newStr;

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_SORT_TYPE);
	}
}

void MultiplayerMenu::RefreshServersLocal()
{
	m_ServerModel.Clear();
	m_PageModel.selectedRow = INVALID_ROW;

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_SELECTED_ROW);
	}

	gEngfuncs.pfnClientCmd(false, "localservers");
}
