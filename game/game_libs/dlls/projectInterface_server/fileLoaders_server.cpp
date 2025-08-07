#include "fileLoaders_server.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

uint8_t* FileLoader_Server::Load(const CUtlString& filePath, size_t& length)
{
	int localLength = 0;
	byte* fileData = LOAD_FILE(filePath.String(), &localLength);
	length = localLength;
	return fileData;
}

void FileLoader_Server::Free(uint8_t* data)
{
	if ( data )
	{
		FREE_FILE(data);
	}
}

char** FileLoader_Server::ListDirectory(const CUtlString& directoryPath, size_t& numFiles, bool gamedirOnly)
{
	CUtlString wildcard(directoryPath);
	wildcard.Append("/*");

	int numFilesInt = 0;
	char** fileList = g_physfuncs.pfnGetFilesList(wildcard.String(), &numFilesInt, gamedirOnly ? TRUE : FALSE);

	if ( numFilesInt < 1 )
	{
		return nullptr;
	}

	numFiles = static_cast<size_t>(numFilesInt);
	return fileList;
}
