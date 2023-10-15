#pragma once

class BufferedFileReader;

class ISerialisableComponent
{
public:
	virtual ~ISerialisableComponent() = default;

	virtual void Clear() = 0;
	virtual void ReadComponentData(BufferedFileReader& data) = 0;
};
