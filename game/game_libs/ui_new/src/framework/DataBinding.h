#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataStructHandle.h>
#include <type_traits>

template<typename T>
class DataBinding
{
public:
	DataBinding(Rml::String&& name, T&& defaultValue) :
		m_Name(std::move(name)),
		m_Value(std::move(defaultValue))
	{
		static_assert(std::is_move_assignable<T>::value, "Type T must be move-assignable");
		static_assert(std::is_move_constructible<T>::value, "Type T must be move-constructible");
	}

	DataBinding(const DataBinding& other) = default;
	DataBinding(DataBinding&& other) = default;
	DataBinding& operator=(const DataBinding& other) = default;
	DataBinding& operator=(DataBinding&& other) = default;

	const Rml::String& Name() const
	{
		return m_Name;
	}

	// TODO: operator ->()

	const T& Value() const
	{
		return m_Value;
	}

	T& Value()
	{
		return m_Value;
	}

	bool Bind(Rml::DataModelConstructor& constructor) const
	{
		return constructor.Bind(m_Name, &m_Value);
	}

private:
	Rml::String m_Name;
	T m_Value;
};

template<typename T, typename Container>
inline bool RegisterMember(Rml::StructHandle<Container>& handle, Container& container, DataBinding<T> Container::* ptr)
{
	return handle.RegisterMember((container.*ptr).Name(), ptr);
}
