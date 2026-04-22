#include "models/ServerModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataStructHandle.h>

static constexpr const char* const NAME_SERVER_LIST = "serverList";
static constexpr const char* const PROP_ADDRESS = "address";
static constexpr const char* const PROP_PING = "ping";
static constexpr const char* const PROP_NUM_CLIENTS = "numClients";
static constexpr const char* const PROP_MAX_CLIENTS = "maxClients";
static constexpr const char* const PROP_SERVER_NAME = "serverName";
static constexpr const char* const PROP_MAP_NAME = "mapName";
static constexpr const char* const PROP_HAS_PASSWORD = "hasPassword";

bool ServerModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<Entry> entryType = constructor.RegisterStruct<Entry>();

	if ( !entryType )
	{
		return false;
	}

	if ( !entryType.RegisterMember(PROP_ADDRESS, &Entry::address) ||
		 !entryType.RegisterMember(PROP_PING, &Entry::ping) ||
		 !entryType.RegisterMember(PROP_NUM_CLIENTS, &Entry::numClients) ||
		 !entryType.RegisterMember(PROP_MAX_CLIENTS, &Entry::maxClients) ||
		 !entryType.RegisterMember(PROP_SERVER_NAME, &Entry::serverName) ||
		 !entryType.RegisterMember(PROP_MAP_NAME, &Entry::mapName) ||
		 !entryType.RegisterMember(PROP_HAS_PASSWORD, &Entry::hasPassword) )
	{
		return false;
	}

	if ( !constructor.RegisterArray<std::vector<Entry>>() || !constructor.Bind(NAME_SERVER_LIST, &m_Entries) )
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
