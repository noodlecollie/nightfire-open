#pragma once

#include "framework/BaseTemplateBinding.h"
#include "framework/DataVar.h"

class OptionsTabBarDataBinding : public BaseTemplateBinding
{
public:
	static constexpr const char* const TAB_GAMEPLAY = "gameplay";
	static constexpr const char* const TAB_KEYS = "keys";
	static constexpr const char* const TAB_MOUSE = "mouse";
	static constexpr const char* const TAB_AV = "av";

	OptionsTabBarDataBinding(const char* defaultValue = "");
	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;

	const Rml::String& ActiveTab() const;
	bool ActiveTabChanged() const;
	void SetActiveTab(const Rml::String& value);

private:
	DataVar<Rml::String> m_ActiveTab;
	Rml::DataModelHandle m_DataModelHandle;
};
