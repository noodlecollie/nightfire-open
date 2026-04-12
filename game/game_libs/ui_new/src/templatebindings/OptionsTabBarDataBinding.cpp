#include "templatebindings/OptionsTabBarDataBinding.h"

OptionsTabBarDataBinding::OptionsTabBarDataBinding(const char* defaultValue) :
	m_ActiveTab {"activeTab", defaultValue}
{
}

bool OptionsTabBarDataBinding::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	const bool boundActiveTab = constructor.BindFunc(
		m_ActiveTab.name,
		[this](Rml::Variant& out)
		{
			out = Rml::Variant(m_ActiveTab.value);
		},
		[this](const Rml::Variant& val)
		{
			Rml::String newStr = val.Get<Rml::String>();

			// We don't actually change the value,
			// since we only want to react to the set
			// by changing the menu. We just broadcast
			// the value we received on the callback.
			if ( newStr != m_ActiveTab.value && m_ChangeCallback )
			{
				m_ChangeCallback(newStr);
			}
		}
	);

	if ( !boundActiveTab )
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

void OptionsTabBarDataBinding::SetActiveTabChangeCallback(ActiveTabChangeFunc cb)
{
	m_ChangeCallback = std::move(cb);
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

		if ( m_ChangeCallback )
		{
			m_ChangeCallback(m_ActiveTab.value);
		}
	}
}
