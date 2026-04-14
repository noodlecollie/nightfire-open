#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include "framework/BaseTemplateBinding.h"
#include "framework/DataVar.h"

class OptionsTabBarDataBinding : public BaseTemplateBinding
{
public:
	using ActiveTabChangeFunc = std::function<void(const Rml::String&)>;

	OptionsTabBarDataBinding(const char* defaultValue = "");
	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;

	const Rml::String& ActiveTab() const;
	void SetActiveTab(const Rml::String& value);
	void SetActiveTabChangeCallback(ActiveTabChangeFunc cb);

private:
	DataVar<Rml::String> m_ActiveTab;
	Rml::DataModelHandle m_DataModelHandle;
	ActiveTabChangeFunc m_ChangeCallback;
};
