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
	CvarDataVar(Rml::String name, Rml::String cvarName, T value = T()) :
		m_CvarName(cvarName),
		m_Var {name, std::move(value)}
	{
		ASSERT(!m_CvarName.empty());
	}

	const Rml::String& Name() const
	{
		return m_Var.name;
	}

	const Rml::String& CvarName() const
	{
		return m_CvarName;
	}

	bool Refresh()
	{
		T newValue = CvarAccessor<T>::GetValue(m_CvarName.c_str());
		CvarAccessor<T>::DbgLog(m_CvarName.c_str(), newValue, false);

		if ( newValue == m_Var.value )
		{
			return false;
		}

		m_Var.value = std::move(newValue);
		return true;
	}

	const T& CachedValue() const
	{
		return m_Var.value;
	}

	bool SetValue(T val)
	{
		if ( val == m_Var.value )
		{
			return false;
		}

		m_Var.value = std::move(val);
		CvarAccessor<T>::DbgLog(m_CvarName.c_str(), m_Var.value, true);
		CvarAccessor<T>::SetValue(m_CvarName.c_str(), m_Var.value);
		return true;
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
	Rml::String m_CvarName = nullptr;
	DataVar<T> m_Var;
};

// Helper for binding this value to a variable where the value is inverted.
// Only applies to boolean values. Assumes the pointer lives long enough
// not to be invalid for any variable get/set call.
static inline bool
BindInverse(Rml::DataModelConstructor& constructor, CvarDataVar<bool>* origVar, const Rml::String& name)
{
	return constructor.BindFunc(
		name,
		[origVar](Rml::Variant& outVal)
		{
			outVal = Rml::Variant(!origVar->CachedValue());
		},
		[origVar](const Rml::Variant& inVal)
		{
			origVar->SetValue(!inVal.Get<bool>());
		}
	);
}
