#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/Elements/ElementTabSet.h>
#include "framework/BaseMenuObserver.h"
#include "framework/MenuEventListenerObject.h"

class OptionsTabBarDataBinding : private BaseMenuObserver
{
public:
	using GoToNewTabFunc = std::function<void(const Rml::String&)>;

	OptionsTabBarDataBinding(BaseMenu* parentMenu);

	void SetGoToNewTabCallback(GoToNewTabFunc cb);

protected:
	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleDocumentShow(Rml::Event& event);
	void HandleGoToTab(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args);
	void RequestChangeMenu(const Rml::String& newMenu);
	void SetActiveTab();

	GoToNewTabFunc m_GoToNewTabCallback;
	Rml::ElementTabSet* m_TabSetElement = nullptr;
	MenuEventListenerObject m_ShowListener;
};
