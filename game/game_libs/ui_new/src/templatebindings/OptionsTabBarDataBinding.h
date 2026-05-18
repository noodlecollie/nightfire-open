#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include "framework/BaseMenuObserver.h"
#include "framework/DataVar.h"

class OptionsTabBarDataBinding : private BaseMenuObserver
{
public:
	using ActiveTabChangeFunc = std::function<void(const Rml::String&)>;

	OptionsTabBarDataBinding(BaseMenu* parentMenu, const char* defaultValue = "");

	const Rml::String& ActiveTab() const;
	void SetActiveTab(const Rml::String& value);
	void SetActiveTabChangeCallback(ActiveTabChangeFunc cb);

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	DataVar<Rml::String> m_ActiveTab;
	Rml::DataModelHandle m_DataModelHandle;
	ActiveTabChangeFunc m_ChangeCallback;
};
