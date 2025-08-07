#include "fileLoaders_client.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"
#include "cl_dll.h"

uint8_t* FileLoader_Client::Load(const CUtlString& filePath, size_t& length)
{
	int localLength = 0;
	byte* fileData = gEngfuncs.COM_LoadFile(filePath.String(), 5, &localLength);
	length = localLength;
	return fileData;
}

void FileLoader_Client::Free(uint8_t* data)
{
	if ( data )
	{
		gEngfuncs.COM_FreeFile(data);
	}
}

char** FileLoader_Client::ListDirectory(const CUtlString&, size_t&, bool)
{
	// We don't currently support this on the client - there is no way to list a directory.
	// The engine API needs upating!
	return nullptr;
}
