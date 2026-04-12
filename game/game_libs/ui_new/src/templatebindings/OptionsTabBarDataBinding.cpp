#include "templatebindings/OptionsTabBarDataBinding.h"
#include "UIDebug.h"

OptionsTabBarDataBinding::OptionsTabBarDataBinding(const char* defaultValue) :
	m_ActiveTab {"activeTab", defaultValue}
{
}

bool OptionsTabBarDataBinding::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_ActiveTab.name, &m_ActiveTab.value) )
	{
		return false;
	}

	m_DataModelHandle = constructor.GetModelHandle();
	return true;
}

const Rml::String& OptionsTabBarDataBinding::ActiveTab() const
{
	return m_ActiveTab.value;
}

bool OptionsTabBarDataBinding::ActiveTabChanged() const
{
	// DataModelHandle is missing a lot of const attributes that
	// it should have...
	Rml::DataModelHandle* handle = const_cast<Rml::DataModelHandle*>(&m_DataModelHandle);
	ASSERT(handle->operator bool());

	return handle->operator bool() && handle->IsVariableDirty(m_ActiveTab.name);
}

void OptionsTabBarDataBinding::SetActiveTab(const Rml::String& value)
{
	if ( m_ActiveTab.value != value )
	{
		m_ActiveTab.value = value;

		if ( m_DataModelHandle )
		{
			m_DataModelHandle.DirtyVariable(m_ActiveTab.name);
		}
	}
}
