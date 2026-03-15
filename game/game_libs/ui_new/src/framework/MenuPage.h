#pragma once

#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/EventListener.h>

// A menu which assumes that the entire RML page has a data model,
// and which automatically implements push_menu and pop_menu.
class MenuPage : public BaseMenu, public Rml::EventListener
{
public:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;
	void ProcessEvent(Rml::Event& event) override;

protected:
	MenuPage(const char* name, const char* rmlFilePath);

private:
	void HandlePushMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandlePopMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
};
