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

	OptionsTabBarDataBinding();
	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;

private:
	DataVar<Rml::String> m_ActiveTab;
};
