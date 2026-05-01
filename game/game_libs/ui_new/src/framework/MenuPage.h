#pragma once

#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/EventListenerObject.h"

// A menu which assumes that the entire RML page has a data model,
// and which automatically implements pushMenu and popMenu.
class MenuPage : public BaseMenu
{
public:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	bool RequestPopOnEscapeKey() const;
	void SetRequestPopOnEscapeKey(bool enable);

protected:
	MenuPage(const char* name, const char* rmlFilePath);

	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

	void RequestPop(Rml::String menuToSwapIn = Rml::String());
	void RequestCutStack(size_t newSize, Rml::String menuToSwapIn = Rml::String());
	virtual bool ShouldPop(const Rml::String& menuToSwapIn) const;

private:
	void ProcessEvent(Rml::Event& event);
	void HandlePushMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandlePopMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandleCutStack(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);

	EventListenerObject m_KeyEventListener;
	bool m_RequestPopOnEscapeKey = true;
};
