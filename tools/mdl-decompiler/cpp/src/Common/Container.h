#pragma once

#include <vector>
#include <stdexcept>
#include "Traits/Reflection.h"

template<typename T>
class Container : public std::vector<T>
{
public:
	Container() = default;

	Container(const Container<T>& other) :
		std::vector<T>(other)
	{
	}

	Container(const std::vector<T>& other) :
		std::vector<T>(other)
	{
	}

	Container(Container<T>&& other) :
		std::vector<T>(std::move(other))
	{
	}

	Container(std::vector<T>&& other) :
		std::vector<T>(std::move(other))
	{
	}

	Container<T>& operator=(const Container<T>& other)
	{
		static_cast<std::vector<T>*>(this)->operator=(other);
		return *this;
	}

	Container<T>& operator=(const std::vector<T>& other)
	{
		static_cast<std::vector<T>*>(this)->operator=(other);
		return *this;
	}

	Container<T>& operator=(Container<T>&& other)
	{
		static_cast<std::vector<T>*>(this)->operator=(std::move(other));
		return *this;
	}

	Container<T>& operator=(std::vector<T>&& other)
	{
		static_cast<std::vector<T>*>(this)->operator=(std::move(other));
		return *this;
	}

	T& GetElementChecked(size_t index)
	{
		CheckIndex(index);
		return std::vector<T>::operator[](index);
	}

	const T& GetElementChecked(size_t index) const
	{
		CheckIndex(index);
		return std::vector<T>::operator[](index);
	}

private:
	void CheckIndex(size_t index) const
	{
		if ( index >= std::vector<T>::size() )
		{
			throw std::invalid_argument(
				"Container<" + Reflection::TypeName<T>() + ">: Index " + std::to_string(index) +
				" was out of range for container of size " + std::to_string(std::vector<T>::size()));
		}
	}
};
