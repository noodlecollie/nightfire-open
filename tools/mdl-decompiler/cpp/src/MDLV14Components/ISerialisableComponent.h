#pragma once

#include <cstddef>

class BufferedFileReader;

class ISerialisableComponent
{
public:
	virtual ~ISerialisableComponent() = default;

	// Clears all data stored in the component.
	virtual void Clear() = 0;

	// Populates the component data from the input data, and returns
	// how many bytes were read.
	virtual size_t ReadComponentData(BufferedFileReader& data) = 0;
};
