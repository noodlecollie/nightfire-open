#pragma once

#include "framework/BaseTableModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "EnginePublicAPI/netadr.h"
#include <memory>
#include <vector>

class ServerModel : public BaseTableModel
{
public:
	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;

private:
	struct Entry
	{
		netadr_t address;
		float ping = 0;
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

	std::vector<std::unique_ptr<Entry>> m_Entries;
	Rml::DataModelHandle m_ModelHandle;
};
