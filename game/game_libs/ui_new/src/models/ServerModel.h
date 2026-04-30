#pragma once

#include "framework/BaseTableModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "EnginePublicAPI/netadr.h"
#include <memory>
#include <vector>
#include <functional>

class ServerModel : public BaseTableModel
{
public:
	enum class SortType
	{
		PING,
		SERVER_NAME,
		MAP_NAME,
		NUM_CLIENTS,
		ADDRESS,
	};

	struct Entry
	{
		size_t arrayIndex = 0;
		netadr_t address;
		Rml::String addressString;
		size_t ping = 0;
		size_t numClients = 0;
		size_t maxClients = 0;
		Rml::String serverName;
		Rml::String mapName;
		bool hasPassword = false;

		// For quickly determining whether this server matches
		// another incoming one:
		Rml::String serverInfoStr;

		Entry()
		{
			memset(&address, 0, sizeof(address));
		}
	};

	static bool SortTypeToString(SortType sortType, Rml::String& out);
	static bool SortTypeFromString(const Rml::String& str, SortType& out);
	static bool SortTypeToString(SortType sortType, bool ascending, Rml::String& out);
	static bool SortTypeFromString(const Rml::String& str, SortType& outType, bool& outAscending);

	ServerModel();

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;

	bool Add(const netadr_t& address, Rml::String&& info);
	void Sort(SortType sortBy, bool ascending);
	void Clear();

	bool GetRowForAddress(const netadr_t& address, size_t& out) const;
	const Entry* GetEntry(size_t row) const;

private:
	// A thin wrapper around the entry, so that we can reorder the list
	// without incurring a lot of copying.
	struct EntryPtr
	{
		std::unique_ptr<Entry> inner;

		// RmlUi expects these to be non-const... :c
		size_t GetIndex()
		{
			return inner->arrayIndex;
		}

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

	static std::function<bool(const EntryPtr&, const EntryPtr&)> GetSortFunction(SortType sortBy, bool ascending);

	void AddTestEntries(size_t count);
	bool ContainsServer(const netadr_t& address, const Rml::String& info) const;
	bool ContainsServer(const netadr_t& address) const;

	std::vector<EntryPtr> m_Entries;
	Rml::DataModelHandle m_ModelHandle;
};
