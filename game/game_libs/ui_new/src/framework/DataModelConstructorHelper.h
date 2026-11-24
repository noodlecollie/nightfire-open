#pragma once

#include <RmlUi/Core/DataModelHandle.h>

// RmlUi throws a fit if a data type is registered more than once, but *also* throws
// a fit if you try and fetch a data type that doesn't exist in order to check if
// it was registered! To work around this, this code is basically a copy of what
// the data model constructor implements, but without the assert when a duplicate
// type is detected. Really, we need to PR something upstream to allow us to fix
// this issue properly.
class DataModelConstructorHelper
{
public:
	explicit DataModelConstructorHelper(Rml::DataModelConstructor& constructor) :
		m_Constructor(constructor)
	{
	}

	template<typename T, typename F>
	bool RegisterStructIdempotent(const F& callback)
	{
		static_assert(std::is_class<T>::value, "Type must be a struct or class type.");
		const Rml::FamilyId id = Rml::Family<T>::Id();

		auto structDefinition = Rml::MakeUnique<Rml::StructDefinition>();
		Rml::StructDefinition* structVariableRaw = structDefinition.get();

		Rml::DataTypeRegister* typeRegister = m_Constructor.GetDataTypeRegister();
		const bool inserted = typeRegister->RegisterDefinition(id, std::move(structDefinition));

		if ( !inserted )
		{
			// Rather than fail an assertion, just return.
			return true;
		}

		Rml::StructHandle<T> handle(typeRegister, structVariableRaw);
		Rml::StructHandle<T>& handleRef = handle;

		return callback(handleRef);
	}

	template<typename Container>
	bool RegisterArrayIdempotent()
	{
		using value_type = typename Container::value_type;

		Rml::DataTypeRegister* typeRegister = m_Constructor.GetDataTypeRegister();
		Rml::VariableDefinition* valueVariable = typeRegister->GetDefinition<value_type>();

		ASSERTSZ(valueVariable, "Underlying value type of array has not been registered.");

		if ( !valueVariable )
		{
			return false;
		}

		const Rml::FamilyId container_id = Rml::Family<Container>::Id();
		auto array_definition = Rml::MakeUnique<Rml::ArrayDefinition<Container>>(valueVariable);

		// Rather than fail an assertion, it's OK if this definition is already registered.
		typeRegister->RegisterDefinition(container_id, std::move(array_definition));
		return true;
	}

private:
	Rml::DataModelConstructor& m_Constructor;
};
