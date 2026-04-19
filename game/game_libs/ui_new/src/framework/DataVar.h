#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>

template<typename T>
struct DataVar
{
	Rml::String name;
	T value {};

	void Update(Rml::DataModelHandle modelHandle, T newValue)
	{
		if ( newValue != value )
		{
			value = newValue;

			if ( modelHandle )
			{
				modelHandle.DirtyVariable(name);
			}
		}
	}
};
