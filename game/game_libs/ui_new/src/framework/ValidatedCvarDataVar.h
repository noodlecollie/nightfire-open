#pragma once

#include "framework/ValidatedVariable.h"
#include "framework/CvarDataVar.h"

template<typename T>
class ValidatedCvarDataVar
{
public:
	ValidatedCvarDataVar(const char* variableName, CvarDataVar<T>* cvar, Validator validator) :
		m_Variable(variableName, std::move(validator)),
		m_Cvar(cvar)
	{
		ASSERT(m_Cvar);

		m_Variable.SetValidationAppliedCallback(
			[this](const T& newValue, bool changed)
			{
				if ( changed )
				{
					m_Cvar->SetValue(newValue);
				}
			}
		);
	}

	ValidationResult ApplyValidation()
	{
		return m_Variable.ApplyValidation();
	}

private:
	ValidatedVariable<T> m_Variable;
	CvarDataVar<T>* m_Cvar;
};
