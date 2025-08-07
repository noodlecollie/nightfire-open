#include "stb_image.h"
#include "fcaseopen.h"
#include "cmdlib.h"

int LoadPNG(const char* fileName, int* width, int* height)
{
	FILE* inFile = NULL;
	stbi_uc* rawData = NULL;
	stbi_uc* imageData = NULL;
	qboolean success = false;

	do
	{
		int inFileLength = 0;

		inFile = fcaseopen(fileName, "rb");

		if ( !inFile )
		{
			break;
		}

		inFileLength = filelength(inFile);

		if ( inFileLength < 1 )
		{
			break;
		}

		rawData = (stbi_uc*)malloc(inFileLength);

		if ( !rawData || fread(rawData, 1, inFileLength, inFile) != (size_t)inFileLength )
		{
			break;
		}

		imageData = stbi_load_from_memory(rawData, inFileLength, width, height, NULL, 0);
		success = imageData != NULL;
	}
	while ( false );

	if ( imageData )
	{
		// We just load to get the dimensions.
		// The actual data we don't care about here.
		stbi_image_free(imageData);
	}

	if ( rawData )
	{
		free(rawData);
	}

	if ( inFile )
	{
		fclose(inFile);
	}

	return success ? 0 : 1;
}
