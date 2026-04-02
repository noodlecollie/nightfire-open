#pragma once

#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>

// A menu which assumes that the entire RML page has a data model,
// and which automatically implements push_menu and pop_menu.
class MenuPage : public BaseMenu
{
public:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	void ProcessEvent(Rml::Event& event) override;

protected:
	MenuPage(const char* name, const char* rmlFilePath);

	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	void HandlePushMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandlePopMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
};
