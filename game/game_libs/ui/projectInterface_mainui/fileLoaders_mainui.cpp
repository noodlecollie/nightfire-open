#include "fileLoaders_mainui.h"
#include "enginecallback_menu.h"
#include "utlstring.h"
#include "utlvector.h"

uint8_t* FileLoader_MainUI::Load(const CUtlString& filePath, size_t& length)
{
	int localLength = 0;
	byte* fileData = EngFuncs::COM_LoadFile(filePath.String(), &localLength);
	length = localLength;
	return fileData;
}

void FileLoader_MainUI::Free(uint8_t* data)
{
	if ( data )
	{
		EngFuncs::COM_FreeFile(data);
	}
}

char** FileLoader_MainUI::ListDirectory(const CUtlString& directoryPath, size_t& numFiles, bool gamedirOnly)
{
	CUtlString wildcard(directoryPath);
	wildcard.Append("/*");

	int numFilesInt = 0;
	char** fileList = EngFuncs::GetFilesList(wildcard.String(), &numFilesInt, gamedirOnly ? TRUE : FALSE);

	if ( numFilesInt < 1 )
	{
		return nullptr;
	}

	numFiles = static_cast<size_t>(numFilesInt);
	return fileList;
}
