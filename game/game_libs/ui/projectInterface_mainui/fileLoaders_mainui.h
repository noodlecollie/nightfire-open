#pragma once

#include "projectInterface/IFileLoader.h"
#include "utlstring.h"

class FileLoader_MainUI : public IFileLoader
{
public:
	virtual ~FileLoader_MainUI()
	{
	}

	virtual uint8_t* Load(const CUtlString& filePath, size_t& length) override;
	virtual void Free(uint8_t* data) override;
	virtual char** ListDirectory(const CUtlString& directoryPath, size_t& numFiles, bool gamedirOnly = true) override;
};
