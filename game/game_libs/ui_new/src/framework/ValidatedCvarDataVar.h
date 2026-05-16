#pragma once

#include <RmlUi/Core/Types.h>
#include "framework/CvarDataVar.h"
#include <functional>

enum ValidationResult
{
	// Value is not valid, do not set it
	FAILED = 0,

	// Value is valid
	OK = 1,

	// Value was not valid, and has been overridden
	// with a new valid value.
	OVERRIDDEN = 2,
};

struct ValidatedCvarResult
{
	ValidationResult validationResult = ValidationResult::OK;
	bool cvarValueChanged = false;
};

template<typename T>
class ValidatedCvarDataVar
{
public:
	using Validator = std::function<ValidationResult(T& /*value*/)>;

	ValidatedCvarDataVar(Rml::String variableName, CvarDataVar<T>* cvar, Validator validator) :
		m_Name(std::move(variableName)),
		m_Cvar(cvar),
		m_Validator(std::move(validator))
	{
		ASSERT(m_Cvar);
	}

	const Rml::String& VariableName() const
	{
		return m_Name;
	}

	const CvarDataVar<T>* Cvar() const
	{
		return m_Cvar;
	}

	ValidatedCvarResult ValidateAndSubmit()
	{
		ValidatedCvarResult result;
		result.validationResult = m_Validator ? m_Validator(m_BufferedValue) : ValidationResult::OK;

		if ( result.validationResult != ValidationResult::FAILED )
		{
			result.cvarValueChanged = m_Cvar->SetValue(m_BufferedValue);
		}

		return result;
	}

	ValidatedCvarResult Refresh()
	{
		const bool cvarChanged = m_Cvar->Refresh();
		T newValue = m_Cvar->CachedValue();

		ValidatedCvarResult result;
		result.cvarValueChanged = cvarChanged;
		result.validationResult = m_Validator ? m_Validator(newValue) : ValidationResult::OK;

		if ( result.validationResult != ValidationResult::FAILED )
		{
			m_BufferedValue = std::move(newValue);
		}

		return result;
	}

private:
	Rml::String m_Name;
	CvarDataVar<T>* m_Cvar;
	Validator m_Validator;
	T m_BufferedValue = {};
};
