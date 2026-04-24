#pragma once

#include "framework/MenuPage.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/ServerModel.h"
#include "EnginePublicAPI/netadr.h"

class MultiplayerMenu : public MenuPage
{
public:
	MultiplayerMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	struct PageModel
	{
		Rml::String sortColumn;
	};

	void AddServerToList(const netadr_t& address, Rml::String&& info);
	void HandleColumnSortRequested(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args);
	void ReSortServerModel(const Rml::String& sortTypeStr);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	ServerModel m_ServerModel;
	PageModel m_PageModel;
	Rml::DataModelHandle m_ModelHandle;
};
