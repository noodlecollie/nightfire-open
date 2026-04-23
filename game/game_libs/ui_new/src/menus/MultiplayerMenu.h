#pragma once

#include "framework/MenuPage.h"
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
	void AddServerToList(const netadr_t& address, Rml::String&& info);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	ServerModel m_ServerModel;
};
