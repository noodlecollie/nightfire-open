#include "models/ServerModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataStructHandle.h>
#include <RmlUi/Core/StringUtilities.h>
#include "PlatformLib/String.h"
#include "BuildPlatform/Utils.h"
#include "utils/InfoBuffer.h"
#include "udll_int.h"

static constexpr const char* const NAME_SERVER_LIST = "serverList";
static constexpr const char* const PROP_INDEX = "index";
static constexpr const char* const PROP_PING = "ping";
static constexpr const char* const PROP_NUM_CLIENTS = "numClients";
static constexpr const char* const PROP_MAX_CLIENTS = "maxClients";
static constexpr const char* const PROP_SERVER_NAME = "serverName";
static constexpr const char* const PROP_MAP_NAME = "mapName";
static constexpr const char* const PROP_HAS_PASSWORD = "hasPassword";
static constexpr const char* const SORT_PING = "ping";
static constexpr const char* const SORT_ADDRESS = "address";
static constexpr const char* const SORT_MAP_NAME = "map";
static constexpr const char* const SORT_SERVER_NAME = "name";
static constexpr const char* const SORT_NUM_CLIENTS = "players";

static const std::pair<ServerModel::SortType, Rml::String> SORT_TYPE_STRINGS[] = {
	{ServerModel::SortType::PING, SORT_PING},
	{ServerModel::SortType::ADDRESS, SORT_ADDRESS},
	{ServerModel::SortType::MAP_NAME, SORT_MAP_NAME},
	{ServerModel::SortType::SERVER_NAME, SORT_SERVER_NAME},
	{ServerModel::SortType::NUM_CLIENTS, SORT_NUM_CLIENTS},
};

bool ServerModel::SortTypeToString(SortType sortType, Rml::String& out)
{
	for ( size_t index = 0; index < SIZE_OF_ARRAY(SORT_TYPE_STRINGS); ++index )
	{
		if ( SORT_TYPE_STRINGS[index].first == sortType )
		{
			out = SORT_TYPE_STRINGS[index].second;
			return true;
		}
	}

	return false;
}

bool ServerModel::SortTypeFromString(const Rml::String& str, SortType& out)
{
	for ( size_t index = 0; index < SIZE_OF_ARRAY(SORT_TYPE_STRINGS); ++index )
	{
		if ( SORT_TYPE_STRINGS[index].second == str )
		{
			out = SORT_TYPE_STRINGS[index].first;
			return true;
		}
	}

	return false;
}

bool ServerModel::SortTypeToString(SortType sortType, bool ascending, Rml::String& out)
{
	Rml::String sortTypeStr;

	if ( !SortTypeToString(sortType, sortTypeStr) )
	{
		return false;
	}

	Rml::FormatString(out, "%s-%s", sortTypeStr.c_str(), ascending ? "asc" : "desc");
	return true;
}

bool ServerModel::SortTypeFromString(const Rml::String& str, SortType& outType, bool& outAscending)
{
	Rml::String::size_type dashPos = str.find('-');

	if ( dashPos == Rml::String::npos )
	{
		return false;
	}

	if ( !SortTypeFromString(str.substr(0, dashPos), outType) )
	{
		return false;
	}

	Rml::String ascStr = str.substr(dashPos + 1);

	if ( ascStr == "asc" )
	{
		outAscending = true;
		return true;
	}

	if ( ascStr == "desc" )
	{
		outAscending = false;
		return true;
	}

	return false;
}

ServerModel::ServerModel()
{
	// REMOVE ME
	AddTestEntries(24);
}

bool ServerModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<EntryPtr> entryType = constructor.RegisterStruct<EntryPtr>();

	if ( !entryType )
	{
		return false;
	}

	if ( !entryType.RegisterMember(PROP_INDEX, &EntryPtr::GetIndex) ||
		 !entryType.RegisterMember(PROP_PING, &EntryPtr::GetPing) ||
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

void ServerModel::Add(const netadr_t& address, Rml::String&& info)
{
	if ( ContainsServer(address, info) )
	{
		return;
	}

	std::unique_ptr<Entry> entry(new Entry {});
	entry->arrayIndex = m_Entries.size();
	entry->serverInfoStr = std::move(info);
	entry->address = address;

	entry->serverName = Info_ValueStrForKey(entry->serverInfoStr.c_str(), "host");
	entry->mapName = Info_ValueStrForKey(entry->serverInfoStr.c_str(), "map");
	entry->addressString = gTextfuncs.pfnAdrToString(entry->address);

	const int numClients = atoi(Info_ValuePtrForKey(entry->serverInfoStr.c_str(), "numcl"));
	entry->numClients = static_cast<size_t>(std::max<int>(numClients, 0));

	const int maxClients = atoi(Info_ValuePtrForKey(entry->serverInfoStr.c_str(), "maxcl"));
	entry->maxClients = static_cast<size_t>(std::max<int>(maxClients, 0));

	entry->hasPassword = strcmp(Info_ValuePtrForKey(entry->serverInfoStr.c_str(), "password"), "1") == 0;

	m_Entries.push_back(EntryPtr {std::move(entry)});
}

void ServerModel::Sort(SortType sortBy, bool ascending)
{
	std::sort(m_Entries.begin(), m_Entries.end(), GetSortFunction(sortBy, ascending));

	for ( size_t index = 0; index < m_Entries.size(); ++index )
	{
		m_Entries[index].inner->arrayIndex = index;
	}

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_SERVER_LIST);
	}
}

bool ServerModel::GetAddress(size_t row, netadr_t& out) const
{
	if ( row >= m_Entries.size() )
	{
		return false;
	}

	out = m_Entries[row].inner->address;
	return true;
}

bool ServerModel::GetRowForAddress(const netadr_t& address, size_t& out) const
{
	for ( size_t index = 0; index < m_Entries.size(); ++index )
	{
		const EntryPtr& entry = m_Entries[index];

		if ( gTextfuncs.pfnCompareAdr(&entry.inner->address, &address) == 0 )
		{
			out = index;
			return true;
		}
	}

	return false;
}

std::function<bool(const ServerModel::EntryPtr&, const ServerModel::EntryPtr&)>
ServerModel::GetSortFunction(SortType sortBy, bool ascending)
{
	// Ascending = a < b
	// Descending = a > b
	switch ( sortBy )
	{
		case SortType::PING:
		{
			return [ascending](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return ascending ? a.inner->ping < b.inner->ping : a.inner->ping > b.inner->ping;
			};
		}

		case SortType::SERVER_NAME:
		{
			return [ascending](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				const int result = PlatformLib_StrCaseCmp(a.inner->serverName.c_str(), b.inner->serverName.c_str());
				return ascending ? result < 0 : result > 0;
			};
		}

		case SortType::NUM_CLIENTS:
		{
			return [ascending](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				return ascending ? a.inner->numClients < b.inner->numClients
								 : a.inner->numClients > b.inner->numClients;
			};
		}

		case SortType::ADDRESS:
		{
			return [ascending](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				const int result = gTextfuncs.pfnCompareAdr(&a.inner->address, &b.inner->address);
				return ascending ? result < 0 : result > 0;
			};
		}

		case SortType::MAP_NAME:
		default:
		{
			return [ascending](const EntryPtr& a, const EntryPtr& b) -> bool
			{
				const int result = PlatformLib_StrCaseCmp(a.inner->mapName.c_str(), b.inner->mapName.c_str());
				return ascending ? result < 0 : result > 0;
			};
		}
	}
}

void ServerModel::AddTestEntries(size_t count)
{
	static const char* const MAPS[] = {
		"dm_power",
		"dm_japan",
		"dm_casino",
		"dm_island",
	};

	for ( size_t index = 0; index < count; ++index )
	{
		// We have to set up a convincing fake address, or sorting will break.
		netadr_t address {};
		memset(&address, 0, sizeof(address));
		address.ip.ip4.type = NA_IP;
		address.ip.ip4.ip.bytes[0] = 192;
		address.ip.ip4.ip.bytes[1] = 168;
		address.ip.ip4.ip.bytes[2] = 10;
		address.ip.ip4.ip.bytes[2] = static_cast<uint8_t>(m_Entries.size());
		address.port = 27015;

		Entry* entry = new Entry {};
		entry->address = address;
		Rml::FormatString(entry->serverName, "Test Server %zu", m_Entries.size() + 1);
		entry->mapName = MAPS[gEngfuncs.pfnRandomLong(0, SIZE_OF_ARRAY_AS_INT(MAPS) - 1)];
		entry->hasPassword = gEngfuncs.pfnRandomLong(0, 1) == 0;
		entry->maxClients = gEngfuncs.pfnRandomLong(8, 32);
		entry->numClients = gEngfuncs.pfnRandomLong(0, static_cast<int>(entry->maxClients));
		entry->ping = gEngfuncs.pfnRandomLong(5, 200);
		m_Entries.push_back(EntryPtr {std::unique_ptr<Entry>(entry)});
	}
}

bool ServerModel::ContainsServer(const netadr_t& address, const Rml::String& info) const
{
	if ( ContainsServer(address) )
	{
		return true;
	}

	for ( const EntryPtr& entry : m_Entries )
	{
		if ( info == entry.inner->serverInfoStr )
		{
			return true;
		}
	}

	return false;
}

bool ServerModel::ContainsServer(const netadr_t& address) const
{
	for ( const EntryPtr& entry : m_Entries )
	{
		if ( gTextfuncs.pfnCompareAdr(&address, &entry.inner->address) )
		{
			return true;
		}
	}

	return false;
}
