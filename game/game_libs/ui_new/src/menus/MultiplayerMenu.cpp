#include "menus/MultiplayerMenu.h"
#include "rmlui/RmlUiBackend.h"

static constexpr const char* const NAME_SORT_COLUMN = "sortColumn";
static constexpr const char* const SORT_NAME_DESC = "name-desc";
static constexpr const char* const SORT_NAME_ASC = "name-asc";
static constexpr const char* const SORT_MAP_DESC = "map-desc";
static constexpr const char* const SORT_MAP_ASC = "map-asc";
static constexpr const char* const SORT_PLAYERS_DESC = "players-desc";
static constexpr const char* const SORT_PLAYERS_ASC = "players-asc";
static constexpr const char* const SORT_PING_DESC = "ping-desc";
static constexpr const char* const SORT_PING_ASC = "ping-asc";

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml"),
	m_MenuFrameDataBinding(this)
{
}

bool MultiplayerMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_ServerModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_SORT_COLUMN, &m_PageModel.sortColumn) )
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
}

void MultiplayerMenu::OnBeginDocumentUnloaded()
{
	RmlUiBackend::StaticInstance().ClearDiscoveredServerCallback();
	MenuPage::OnBeginDocumentUnloaded();
}

void MultiplayerMenu::AddServerToList(const netadr_t& /* address */, Rml::String&& /* info */)
{
	// TODO
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

void MultiplayerMenu::ReSortServerModel(const Rml::String& sortTypeStr)
{
	bool didSort = true;

	if ( sortTypeStr == SORT_NAME_ASC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::SERVER_NAME, true);
	}
	else if ( sortTypeStr == SORT_NAME_DESC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::SERVER_NAME, false);
	}
	else if ( sortTypeStr == SORT_MAP_ASC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::MAP_NAME, true);
	}
	else if ( sortTypeStr == SORT_MAP_DESC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::MAP_NAME, false);
	}
	else if ( sortTypeStr == SORT_PLAYERS_ASC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::NUM_CLIENTS, true);
	}
	else if ( sortTypeStr == SORT_PLAYERS_DESC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::NUM_CLIENTS, false);
	}
	else if ( sortTypeStr == SORT_PING_ASC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::PING, true);
	}
	else if ( sortTypeStr == SORT_PING_DESC )
	{
		m_ServerModel.Sort(ServerModel::SortBy::PING, false);
	}
	else
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"MultiplayerMenu::ReSortServerModel: Unrecognised sort type \"%s\"",
			sortTypeStr.c_str()
		);

		didSort = false;
	}

	if ( didSort )
	{
		m_PageModel.sortColumn = sortTypeStr;

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(NAME_SORT_COLUMN);
		}
	}
}
