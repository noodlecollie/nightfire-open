#pragma once

#include <RmlUi/Core/Types.h>

template<typename T>
struct DataVar
{
	Rml::String name;
	T value {};
};
