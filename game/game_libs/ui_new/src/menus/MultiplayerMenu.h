#pragma once

#include "framework/MenuPage.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/EventListenerObject.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/ServerModel.h"
#include "EnginePublicAPI/netadr.h"

class MultiplayerMenu : public MenuPage
{
public:
	MultiplayerMenu();

	void Update(float currentTime) override;

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	static constexpr int INVALID_ROW = -1;

	struct PageModel
	{
		Rml::String sortType;
		int selectedRow = INVALID_ROW;

		ServerModel::SortType sortBy = ServerModel::SortType::PING;
		bool sortAscending = true;
	};

	void ProcessShowHideEvents(Rml::Event& event);
	void AddServerToList(const netadr_t& address, Rml::String&& info);
	void HandleColumnSortRequested(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args);
	void HandleSelectServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args);
	void HandleConnectToSelectedServer(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void ReSortServerModel(const Rml::String& sortTypeStr = Rml::String());
	void UpdateSortTypeVariable();
	void RefreshServersLocal();

	EventListenerObject m_ShowHideEventListener;
	MenuFrameDataBinding m_MenuFrameDataBinding;
	ServerModel m_ServerModel;
	PageModel m_PageModel;
	Rml::DataModelHandle m_ModelHandle;
};
