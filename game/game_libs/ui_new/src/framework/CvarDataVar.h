#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/Log.h>
#include "framework/DataVar.h"
#include "framework/CvarAccessor.h"
#include "UIDebug.h"

// Class that holds a value backed by a cvar.
// The value must be refreshed manually.
template<typename T>
class CvarDataVar
{
public:
	CvarDataVar(const char* name, const char* cvarName, T value = T()) :
		m_CvarName(cvarName),
		m_Var {name, value}
	{
		ASSERT(m_CvarName && *m_CvarName);
	}

	const char* Name() const
	{
		return m_Var.name;
	}

	const char* CvarName() const
	{
		return m_CvarName;
	}

	bool Refresh()
	{
		T newValue = CvarAccessor<T>::GetValue(m_CvarName);
		CvarAccessor<T>::DbgLog(m_CvarName, newValue, false);

		if ( newValue == m_Var.value )
		{
			return false;
		}

		m_Var.value = newValue;
		return true;
	}

	const T& CachedValue() const
	{
		return m_Var.value;
	}

	void SetValue(const T& val)
	{
		if ( val != m_Var.value )
		{
			m_Var.value = val;
			CvarAccessor<T>::DbgLog(m_CvarName, m_Var.value, true);
			CvarAccessor<T>::SetValue(m_CvarName, m_Var.value);
		}
	}

	bool Bind(Rml::DataModelConstructor& constructor)
	{
		return constructor.BindFunc(
			Name(),
			[this](Rml::Variant& outVal)
			{
				outVal = Rml::Variant(CachedValue());
			},
			[this](const Rml::Variant& inVal)
			{
				SetValue(inVal.Get<T>());
			}
		);
	}

private:
	const char* m_CvarName = nullptr;
	DataVar<T> m_Var;
};
