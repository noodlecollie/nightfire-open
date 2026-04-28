#include "menus/MultiplayerMenu.h"
#include "rmlui/RmlUiBackend.h"

static constexpr const char* const NAME_SORT_TYPE = "sortType";
static constexpr const char* const EVENT_SORT = "sort";
static constexpr const char* const EVENT_CONNECT = "connectToSelectedServer";

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml"),
	m_MenuFrameDataBinding(this)
{
	ReSortServerModel();
}

bool MultiplayerMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_ServerModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_SORT_TYPE, &m_PageModel.sortType) ||
		 !constructor.BindEventCallback(EVENT_SORT, &MultiplayerMenu::HandleColumnSortRequested, this) )
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

void MultiplayerMenu::AddServerToList(const netadr_t& address, Rml::String&& info)
{
	m_ServerModel.Add(address, std::move(info));
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

void MultiplayerMenu::HandleConnectToSelectedServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	// TODO
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

	m_ServerModel.Sort(m_PageModel.sortBy, m_PageModel.sortAscending);
	UpdateSortTypeVariable();
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
