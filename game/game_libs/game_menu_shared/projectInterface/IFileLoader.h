#pragma once

#include <cstdint>
#include <cstddef>

class CUtlString;

class IFileLoader
{
public:
	virtual ~IFileLoader()
	{
	}

	virtual uint8_t* Load(const CUtlString& filePath, size_t& length) = 0;
	virtual void Free(uint8_t* data) = 0;
	virtual char** ListDirectory(const CUtlString& directoryPath, size_t& numFiles, bool gamedirOnly = true) = 0;
};
