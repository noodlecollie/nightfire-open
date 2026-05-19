#pragma once

#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/Types.h>
#include <functional>
#include "UIDebug.h"

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

template<typename T>
class ValidatedVariable : private BaseMenuObserver
{
public:
	using ValidationFunction = std::function<ValidationResult(T& /*value*/)>;
	using ValidationAppliedFunction = std::function<void(const T& /*newValue*/, bool /*changed*/)>;

	ValidatedVariable(
		BaseMenu parentMenu,
		const char* variableName,
		ValidationFunction validator,
		T defaultValue = T()
	) :
		BaseMenuObserver(parentMenu),
		m_VariableName(variableName),
		m_Validator(std::move(validator)),
		m_Value(std::move(defaultValue))
	{
		ASSERT(variableName && *variableName);
	}

	ValidationResult ApplyValidation()
	{
		T newValue = m_Value;
		ValidationResult result = m_Validator ? m_Validator(newValue) : ValidationResult::OK;

		if ( result == ValidationResult::FAILED )
		{
			return result;
		}

		const bool changed = newValue != m_Value;

		if ( changed )
		{
			m_Value = std::move(newValue);
			DirtyVariable(m_VariableName);
		}

		if ( m_ValidationAppliedCallback )
		{
			m_ValidationAppliedCallback(m_Value, changed);
		}
	}

	void SetValidationAppliedCallback(ValidationAppliedFunction callback)
	{
		m_ValidationAppliedCallback = std::move(callback);
	}

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override
	{
		return constructor.Bind(m_VariableName, &m_Value);
	}

private:
	const char* m_VariableName;
	T m_Value = {};
	ValidationFunction m_Validator;
	ValidationAppliedFunction m_ValidationAppliedCallback;
};
