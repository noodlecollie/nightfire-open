#include "menus/temp_new/NewMultiplayerMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "rmlui/RmlUiBackend.h"
#include "menus/ServerConnectionScreen.h"
#include "udll_int.h"

static constexpr const char* const NAME_SORT_TYPE = "sortType";
static constexpr const char* const NAME_SELECTED_ROW = "selectedRow";
static constexpr const char* const EVENT_SORT = "sort";
static constexpr const char* const EVENT_SELECT_SERVER = "selectServer";
static constexpr const char* const EVENT_CONNECT = "connectToSelectedServer";

NewMultiplayerMenu::NewMultiplayerMenu() :
	MenuPage("new_multiplayer_menu", "multiplayer_menu.rml"),
	m_ShowHideEventListener(this, &NewMultiplayerMenu::ProcessShowHideEvents, {Rml::EventId::Show, Rml::EventId::Hide}),
	m_ServerModel(this)
{
	ReSortServerModel();
}

void NewMultiplayerMenu::Update(float currentTime)
{
	MenuPage::Update(currentTime);

	if ( m_NextRefreshTime != REFRESH_NEVER && m_NextRefreshTime <= currentTime )
	{
		RefreshServersLocal();
	}
}

bool NewMultiplayerMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_SORT_TYPE, &m_PageModel.sortType) ||
		 !constructor.Bind(NAME_SELECTED_ROW, &m_PageModel.selectedRow) ||
		 !constructor.BindEventCallback(EVENT_SORT, &NewMultiplayerMenu::HandleColumnSortRequested, this) ||
		 !constructor.BindEventCallback(EVENT_SELECT_SERVER, &NewMultiplayerMenu::HandleSelectServer, this) ||
		 !constructor.BindEventCallback(EVENT_CONNECT, &NewMultiplayerMenu::HandleConnectToSelectedServer, this) )
	{
		return false;
	}

	return true;
}

void NewMultiplayerMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			// Stop demos to allow network sockets to open
			if ( gpGlobals->demoplayback && gEngfuncs.pfnGetCvarFloat("cl_background") )
			{
				gEngfuncs.pfnClientCmd(false, "stop");
			}

			RmlUiBackend::StaticInstance().SetDiscoveredServerCallback(
				[this](const netadr_t& address, Rml::String&& info)
				{
					AddServerToList(address, std::move(info));
				}
			);

			RefreshServersLocal();
			break;
		};

		case Rml::EventId::Hide:
		{
			RmlUiBackend::StaticInstance().ClearDiscoveredServerCallback();
			m_NextRefreshTime = REFRESH_NEVER;
			SetSelectedRow(INVALID_ROW);
			break;
		}

		default:
		{
			break;
		}
	}
}

void NewMultiplayerMenu::AddServerToList(const netadr_t& address, Rml::String&& info)
{
	if ( m_ServerModel.Add(address, std::move(info)) )
	{
		ReSortServerModel();
	}
}

void NewMultiplayerMenu::HandleColumnSortRequested(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
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

void NewMultiplayerMenu::HandleSelectServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
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

	SetSelectedRow(index);
}

void NewMultiplayerMenu::HandleConnectToSelectedServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
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

	m_NextRefreshTime = REFRESH_NEVER;

	// No password required.
	gEngfuncs.pfnCvarSetString("password", "");

	// TODO: Do we need to cater for legacy servers here?
	// I don't think so at the moment.
	gEngfuncs.pfnClientCmd(false, Rml::CreateString("connect %s", gTextfuncs.pfnAdrToString(entry->address)).c_str());

	RequestCutStack(0, ServerConnectionScreen::NAME);
}

void NewMultiplayerMenu::ReSortServerModel(const Rml::String& sortTypeStr)
{
	if ( !sortTypeStr.empty() )
	{
		ServerModel::SortType newSortBy = ServerModel::SortType::PING;

		if ( !ServerModel::SortTypeFromString(sortTypeStr, newSortBy) )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_WARNING,
				"NewMultiplayerMenu::ReSortServerModel: Unrecognised sort type \"%s\"",
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
		SetSelectedRow(m_ServerModel.GetRowForAddress(selectedAddress, row) ? static_cast<int>(row) : INVALID_ROW);
	}
}

void NewMultiplayerMenu::UpdateSortTypeVariable()
{
	Rml::String newStr;

	if ( !ServerModel::SortTypeToString(m_PageModel.sortBy, m_PageModel.sortAscending, newStr) )
	{
		ASSERT(false);
		return;
	}

	m_PageModel.sortType = newStr;
	DirtyVariable(NAME_SORT_TYPE);
}

void NewMultiplayerMenu::RefreshServersLocal()
{
	m_ServerModel.Clear();
	SetSelectedRow(INVALID_ROW);
	gEngfuncs.pfnClientCmd(false, "localservers");
	m_NextRefreshTime = gpGlobals->time + REFRESH_INTERVAL_SECS;
}

void NewMultiplayerMenu::SetSelectedRow(int index)
{
	if ( m_PageModel.selectedRow != index )
	{
		m_PageModel.selectedRow = index;
		DirtyVariable(NAME_SELECTED_ROW);
	}
}
