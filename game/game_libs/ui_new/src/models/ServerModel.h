#pragma once

#include "framework/BaseTableModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "EnginePublicAPI/netadr.h"
#include <memory>
#include <vector>

class ServerModel : public BaseTableModel
{
public:
	ServerModel();

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;

private:
	struct Entry
	{
		netadr_t address;
		size_t ping = 0;
		size_t numClients = 0;
		size_t maxClients = 0;
		Rml::String serverName;
		Rml::String mapName;
		bool hasPassword = false;

		Entry()
		{
			memset(&address, 0, sizeof(address));
		}
	};

	// A thin wrapper around the entry, so that we can reorder the list
	// without incurring a lot of copying.
	struct EntryPtr
	{
		std::unique_ptr<Entry> inner;

		// RmlUi expects these to be non-const... :c
		size_t GetPing()
		{
			return inner->ping;
		}

		size_t GetNumClients()
		{
			return inner->numClients;
		}

		size_t GetMaxClients()
		{
			return inner->maxClients;
		}

		Rml::String GetServerName()
		{
			return inner->serverName;
		}

		Rml::String GetMapName()
		{
			return inner->mapName;
		}

		bool GetHasPassword()
		{
			return inner->hasPassword;
		}
	};

	std::vector<EntryPtr> m_Entries;
	Rml::DataModelHandle m_ModelHandle;
};
