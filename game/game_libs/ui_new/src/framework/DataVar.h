#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataStructHandle.h>

template<typename T>
struct DataVar
{
	const char* name;
	T value {};
};
