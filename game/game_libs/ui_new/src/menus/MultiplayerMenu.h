#pragma once

#include "framework/MenuPage.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/EventListenerObject.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/ServerModel.h"
#include "EnginePublicAPI/netadr.h"
#include <limits>

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
	static constexpr float REFRESH_NEVER = std::numeric_limits<float>::max();
	static constexpr float REFRESH_INTERVAL_SECS = 20.0f;

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

	// TODO: Do we want an option for internet servers?
	// It'd be good to implement the logic at least, even if
	// we don't allow the user to query them yet.
	void RefreshServersLocal();

	void SetSelectedRow(int index);

	EventListenerObject m_ShowHideEventListener;
	MenuFrameDataBinding m_MenuFrameDataBinding;
	ServerModel m_ServerModel;
	PageModel m_PageModel;
	Rml::DataModelHandle m_ModelHandle;
	float m_NextRefreshTime = REFRESH_NEVER;
};
