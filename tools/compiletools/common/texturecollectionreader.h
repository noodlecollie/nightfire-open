#ifndef TEXTURECOLLECTIONREADER_H
#define TEXTURECOLLECTIONREADER_H

#include <cstdint>
#include <string>
#include <vector>
#include "mathtypes.h"
#include "bspfile.h"

class TextureCollection;
class TextureDirectoryListing;

class TextureCollectionReader
{
public:
	TextureCollectionReader(TextureCollection& collection, const TextureDirectoryListing& listing);

	bool load(const void* data, uint32_t length);
	bool appendMiptex(const void* miptex, uint32_t length, bool headerOnly = false);

private:
	enum class TextureReadMode
	{
		Infer = 0,
		HeaderOnly,
		AllData
	};

	static bool validateMiptex(const byte* proposedMiptex,
							   uint32_t availableSize,
							   int32_t index = -1,
							   TextureReadMode mode = TextureReadMode::Infer);

	void clearInternalData();
	bool loadPngs();
	bool loadMiptex();
	bool validateAllMiptex() const;
	bool fillAllMiptex();
	bool validateAllPngPaths() const;
	bool loadAllPngs();

	TextureCollection& m_Collection;
	const TextureDirectoryListing& m_Listing;

	const byte* m_InputData;
	uint32_t m_InputLength;
	const dtexturelumpheader_t* m_TextureHeader;
	uint32_t m_MiptexCount;
	uint32_t m_PngCount;
	std::vector<uint32_t> m_MiptexOffsets;
};

#endif	// TEXTURECOLLECTIONREADER_H
