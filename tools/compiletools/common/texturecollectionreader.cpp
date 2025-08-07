#include "texturecollectionreader.h"
#include "texturecollection.h"
#include "texturedirectorylisting.h"
#include "miptexwrapper.h"
#include "pngtexturepath.h"
#include "log.h"
#include "hlassert.h"
#include "cmdlib.h"

#define WARNING(...) Warning("TextureCollectionReader: " __VA_ARGS__)

TextureCollectionReader::TextureCollectionReader(TextureCollection& collection, const TextureDirectoryListing& listing) :
	m_Collection(collection),
	m_Listing(listing),
	m_InputData(NULL),
	m_InputLength(0),
	m_TextureHeader(NULL),
	m_MiptexCount(0),
	m_PngCount(0),
	m_MiptexOffsets()
{
}

bool TextureCollectionReader::appendMiptex(const void* miptex, uint32_t length, bool headerOnly)
{
	if ( !miptex || length < sizeof(miptex_t) )
	{
		WARNING("Data provided to appendMiptex() was invalid or too short.");
		return false;
	}

	const TextureReadMode mode = headerOnly ? TextureReadMode::HeaderOnly : TextureReadMode::AllData;

	if ( !validateMiptex(reinterpret_cast<const byte*>(miptex), length, -1, mode) )
	{
		return false;
	}

	m_Collection.allocateAndAppend(1, TextureCollection::ItemType::Miptex);

	MiptexWrapper* wrapper = m_Collection.miptexAt(m_Collection.count() - 1);
	hlassert(wrapper);

	return wrapper->setFromMiptex(reinterpret_cast<const miptex_t*>(miptex), headerOnly);
}

// The previous implementation of this reordered the textures by their offset and removed any invalid textures.
// Actually we just want to fail if any of the textures doesn't validate, as changing the incoming data would
// invalidate anything that expects to refer to a texture by a previously known index.
bool TextureCollectionReader::load(const void* data, uint32_t length)
{
	clearInternalData();
	m_Collection.clear();

	m_InputData = reinterpret_cast<const byte*>(data);
	m_InputLength = length;

	if ( !m_InputData || m_InputLength < 1 )
	{
		WARNING("Invalid input data provided.");
		return false;
	}

	if ( m_InputLength < sizeof(dtexturelumpheader_t) )
	{
		WARNING("Not enough data to read header.");
		return false;
	}

	m_TextureHeader = reinterpret_cast<const dtexturelumpheader_t*>(m_InputData);
	m_PngCount = LittleLong(m_TextureHeader->pngCount);
	m_MiptexCount = LittleLong(m_TextureHeader->miptexCount);

	return loadPngs() && loadMiptex();
}

bool TextureCollectionReader::loadPngs()
{
	return validateAllPngPaths() && loadAllPngs();
}

bool TextureCollectionReader::validateAllPngPaths() const
{
	const uint32_t pngPathDataLength = m_PngCount * sizeof(dpngtexturepath_t);
	if ( sizeof(dtexturelumpheader_t) + pngPathDataLength > m_InputLength )
	{
		WARNING("Not enough data to read %u PNG paths.", m_PngCount);
		return false;
	}

	const dpngtexturepath_t* paths = reinterpret_cast<const dpngtexturepath_t*>(m_InputData + sizeof(dtexturelumpheader_t));

	for ( uint32_t index = 0; index < m_PngCount; ++index )
	{
		if ( !TerminatedString(paths[index].path, MAX_TEXTURE_NAME_LENGTH) )
		{
			WARNING("PNG path %u is unterminated.", index);
			return false;
		}

		const uint32_t pathLength = strlen(paths[index].path);
		if ( pathLength < 1 || pathLength >= MAX_TEXTURE_NAME_LENGTH )
		{
			WARNING("PNG path %u has invalid length %u.", index, pathLength);
			return false;
		}
	}

	return true;
}

bool TextureCollectionReader::loadAllPngs()
{
	const uint32_t prevCount = m_Collection.count();
	m_Collection.allocateAndAppend(m_PngCount, TextureCollection::ItemType::PngOnDisk);

	const dpngtexturepath_t* paths = reinterpret_cast<const dpngtexturepath_t*>(m_InputData + sizeof(dtexturelumpheader_t));

	for ( uint32_t index = 0; index < m_PngCount; ++index )
	{
		PNGTexturePath* texture = m_Collection.pngTextureAt(prevCount + index);
		hlassert(texture);

		const dpngtexturepath_t* path = &paths[index];
		texture->setPath(path->path);
	}

	return true;
}

bool TextureCollectionReader::loadMiptex()
{
	const uint32_t priorDataLength = sizeof(dtexturelumpheader_t) + (m_PngCount * sizeof(dpngtexturepath_t));
	const uint32_t offsetsDataLength = m_MiptexCount * sizeof(uint32_t);

	if ( priorDataLength + offsetsDataLength > m_InputLength )
	{
		WARNING("Not enough data to read miptex offsets.");
		return false;
	}

	const uint32_t* offsets = reinterpret_cast<const uint32_t*>(m_InputData + priorDataLength);
	m_MiptexOffsets.resize(m_MiptexCount);

	for ( uint32_t index = 0; index < m_MiptexCount; ++index )
	{
		m_MiptexOffsets[index] = LittleLong(offsets[index]);
	}

	return validateAllMiptex() && fillAllMiptex();
}

bool TextureCollectionReader::validateAllMiptex() const
{
	for ( uint32_t index = 0; index < m_MiptexOffsets.size(); ++index )
	{
		const uint32_t offset = m_MiptexOffsets[index];
		if ( offset >= m_InputLength )
		{
			WARNING("Not enough data to read miptex %u at offset %u.", index, offset);
			return false;
		}

		const byte* data = m_InputData + offset;
		const uint32_t dataMaxLength = m_InputLength - offset;

		if ( !validateMiptex(data, dataMaxLength, static_cast<int32_t>(index), TextureReadMode::Infer) )
		{
			return false;
		}
	}

	return true;
}

bool TextureCollectionReader::fillAllMiptex()
{
	const uint32_t miptexOffsetCount = m_MiptexOffsets.size();
	const uint32_t prevCount = m_Collection.count();

	m_Collection.allocateAndAppend(miptexOffsetCount, TextureCollection::ItemType::Miptex);

	for ( uint32_t index = 0; index < miptexOffsetCount; ++index )
	{
		MiptexWrapper* wrapper = m_Collection.miptexAt(prevCount + index);
		hlassert(wrapper);

		const miptex_t* miptex = reinterpret_cast<const miptex_t*>(m_InputData + m_MiptexOffsets[index]);
		if ( !wrapper->setFromMiptex(miptex) )
		{
			WARNING("Failed to load miptex at index %u.", prevCount + index);
			return false;
		}
	}

	return true;
}

bool TextureCollectionReader::validateMiptex(const byte* proposedMiptex,
											 uint32_t availableSize,
											 int32_t index,
											 TextureReadMode mode)
{
	const std::string indexString = index >= 0 ? std::string(" ") + std::to_string(index) : std::string("");

	// Sanity:
	if ( !proposedMiptex || availableSize < 1 )
	{
		WARNING("Internal error. Could not validate texture%s: input data was not valid.", indexString.c_str());
		return false;
	}

	if ( availableSize < sizeof(miptex_t) )
	{
		WARNING("Texture%s invalid: not enough space in buffer to read header.", indexString.c_str());
		return false;
	}

	const miptex_t* const miptex = reinterpret_cast<const miptex_t*>(proposedMiptex);

	if ( miptex->width < 1 || miptex->width % 16 != 0 || miptex->height < 1 || miptex->height % 16 != 0 )
	{
		WARNING("Texture%s has invalid dimensions %ux%u. Dimensions must be a multiple of 16.",
				indexString.c_str(),
				miptex->width,
				miptex->height);

		return false;
	}

	if ( availableSize < sizeof(miptex_t) )
	{
		WARNING("Texture%s invalid - %u bytes required for header, but data buffer only had space for %u bytes.",
				indexString.c_str(),
				static_cast<uint32_t>(sizeof(miptex_t)),
				availableSize);

		return false;
	}

	const uint32_t sizeRequired = (mode == TextureReadMode::AllData)
		? MiptexWrapper::totalIdealBytesRequired(miptex->width, miptex->height)
		: MiptexWrapper::totalBytesRequired(miptex, mode == TextureReadMode::HeaderOnly);

	if ( availableSize < sizeRequired )
	{
		WARNING("Texture%s invalid - %u bytes required, but data buffer only had space for %u bytes.",
				indexString.c_str(),
				sizeRequired,
				availableSize);

		return false;
	}

	if ( mode != TextureReadMode::HeaderOnly && availableSize > sizeRequired )
	{
		WARNING("Data layout provided texture%s with %u bytes of data, when only %u bytes were required. "
				"Extra bytes are ignored.",
				indexString.c_str(),
				availableSize,
				sizeRequired);
	}

	return true;
}

void TextureCollectionReader::clearInternalData()
{
	m_InputData = NULL;
	m_InputLength  = 0;
	m_TextureHeader = NULL;
	m_MiptexCount = 0;
	m_PngCount = 0;
	m_MiptexOffsets.clear();
}
