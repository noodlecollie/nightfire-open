#pragma once

#include "projectInterface/IFileLoader.h"

class FileLoader_Server : public IFileLoader
{
public:
	virtual ~FileLoader_Server()
	{
	}

	virtual uint8_t* Load(const CUtlString& filePath, size_t& length) override;
	virtual void Free(uint8_t* data) override;
	virtual char** ListDirectory(const CUtlString& directoryPath, size_t& numFiles, bool gamedirOnly = true) override;
};
