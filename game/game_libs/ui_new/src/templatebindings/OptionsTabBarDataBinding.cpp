#include "templatebindings/OptionsTabBarDataBinding.h"

OptionsTabBarDataBinding::OptionsTabBarDataBinding() :
	m_ActiveTab {"activeTab", TAB_GAMEPLAY}
{
}

bool OptionsTabBarDataBinding::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	constructor.Bind(m_ActiveTab.name, &m_ActiveTab.value);
	return true;
}
