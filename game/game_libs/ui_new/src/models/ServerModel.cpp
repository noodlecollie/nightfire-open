#include "models/ServerModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataStructHandle.h>
#include "PlatformLib/String.h"
#include "UIDebug.h"
#include "udll_int.h"

static constexpr const char* const NAME_SERVER_LIST = "serverList";
static constexpr const char* const PROP_PING = "ping";
static constexpr const char* const PROP_NUM_CLIENTS = "numClients";
static constexpr const char* const PROP_MAX_CLIENTS = "maxClients";
static constexpr const char* const PROP_SERVER_NAME = "serverName";
static constexpr const char* const PROP_MAP_NAME = "mapName";
static constexpr const char* const PROP_HAS_PASSWORD = "hasPassword";

ServerModel::ServerModel()
{
	// REMOVE ME
	Entry* entry = new Entry {};
	entry->serverName = "Test Server";
	entry->mapName = "dm_island";
	entry->hasPassword = true;
	entry->maxClients = 16;
	entry->numClients = 4;
	entry->ping = 25;

	m_Entries.push_back(EntryPtr {std::unique_ptr<Entry>(entry)});

	entry = new Entry {};
	entry->serverName = "AAA Test Server";
	entry->mapName = "dm_power";
	entry->hasPassword = true;
	entry->maxClients = 20;
	entry->numClients = 1;
	entry->ping = 40;

	m_Entries.push_back(EntryPtr {std::unique_ptr<Entry>(entry)});
}

bool ServerModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<EntryPtr> entryType = constructor.RegisterStruct<EntryPtr>();

	if ( !entryType )
	{
		return false;
	}

	if ( !entryType.RegisterMember(PROP_PING, &EntryPtr::GetPing) ||
		 !entryType.RegisterMember(PROP_NUM_CLIENTS, &EntryPtr::GetNumClients) ||
		 !entryType.RegisterMember(PROP_MAX_CLIENTS, &EntryPtr::GetMaxClients) ||
		 !entryType.RegisterMember(PROP_SERVER_NAME, &EntryPtr::GetServerName) ||
		 !entryType.RegisterMember(PROP_MAP_NAME, &EntryPtr::GetMapName) ||
		 !entryType.RegisterMember(PROP_HAS_PASSWORD, &EntryPtr::GetHasPassword) )
	{
		return false;
	}

	if ( !constructor.RegisterArray<std::vector<EntryPtr>>() || !constructor.Bind(NAME_SERVER_LIST, &m_Entries) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

size_t ServerModel::Rows() const
{
	return m_Entries.size();
}

void ServerModel::Sort(SortBy sortBy)
{
	std::sort(m_Entries.begin(), m_Entries.end(), GetSortFunction(sortBy));
}

std::function<bool(const ServerModel::EntryPtr&, const ServerModel::EntryPtr&)> ServerModel::GetSortFunction(
	SortBy sortBy
)
{
	switch ( sortBy )
	{
		case SortBy::PING:
		{
			return [](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return a.inner->ping < b.inner->ping;
			};
		}

		case SortBy::SERVER_NAME:
		{
			return [](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return PlatformLib_StrCaseCmp(a.inner->serverName.c_str(), b.inner->serverName.c_str()) < 0;
			};
		}

		case SortBy::NUM_CLIENTS:
		{
			return [](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return a.inner->numClients < b.inner->numClients;
			};
		}

		case SortBy::ADDRESS:
		{
			return [](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return gTextfuncs.pfnCompareAdr(&a.inner->address, &b.inner->address) < 0;
			};
		}

		case SortBy::MAP_NAME:
		default:
		{
			return [](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return PlatformLib_StrCaseCmp(a.inner->mapName.c_str(), b.inner->mapName.c_str()) < 0;
			};
		}
	}
}
