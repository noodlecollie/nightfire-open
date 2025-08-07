#include "miptexwrapper.h"
#include "hlassert.h"
#include "log.h"

#define WARNING(...) Warning("MiptexWrapper: " __VA_ARGS__)

MiptexWrapper::MiptexWrapper(const char* name, uint32_t width, uint32_t height) :
	m_Name(),
	m_Width(0),
	m_Height(0),
	m_Mipmaps(),
	m_Palette()
{
	m_Name[0] = '\0';

	if ( name )
	{
		setName(name);
	}

	if ( width > 0 && height > 0 )
	{
		setDimensions(width, height);
	}
}

void MiptexWrapper::invalidate(bool clearName)
{
	m_Width = 0;
	m_Height = 0;

	if ( clearName )
	{
		m_Name[0] = '\0';
	}

	for ( uint32_t level = 0; level < MIPLEVELS; ++level )
	{
		m_Mipmaps[level].clear();
	}

	m_Palette.clear();
}

void MiptexWrapper::setBlank()
{
	if ( !isValid() )
	{
		return;
	}

	initialiseMipmap();
	initialisePalette();
}

bool MiptexWrapper::initialise(uint32_t width, uint32_t height)
{
	if ( !setDimensions(width, height) )
	{
		return false;
	}

	setBlank();
	return true;
}

bool MiptexWrapper::setDimensions(uint32_t width, uint32_t height)
{
	// Reset everything except the name.
	invalidate(false);

	if ( width < 1 || height < 1 || width % 16 != 0 || height % 16 != 0 )
	{
		return false;
	}

	m_Width = width;
	m_Height = height;

	return true;
}

uint32_t MiptexWrapper::width() const
{
	return m_Width;
}

uint32_t MiptexWrapper::height() const
{
	return m_Height;
}

const char* MiptexWrapper::name() const
{
	return m_Name;
}

bool MiptexWrapper::hasPalette() const
{
	return !m_Palette.empty();
}

void MiptexWrapper::initialisePalette()
{
	if ( !isValid() )
	{
		return;
	}

	m_Palette.clear();
	m_Palette.resize(PALETTE_SIZE * sizeof(rgbpixel_t), 0);
}

bool MiptexWrapper::hasAnyMipmap() const
{
	for ( uint32_t level = 0; level < MIPLEVELS; ++level )
	{
		if ( hasMipmap(level) )
		{
			return true;
		}
	}

	return false;
}

bool MiptexWrapper::hasMipmaps(uint32_t level) const
{
	if ( level >= MIPLEVELS )
	{
		return false;
	}

	for ( uint32_t testLevel = 0; testLevel <= level; ++testLevel )
	{
		if ( m_Mipmaps[testLevel].empty() )
		{
			return false;
		}
	}

	return true;
}

bool MiptexWrapper::hasMipmap(uint32_t level) const
{
	return level < MIPLEVELS && !m_Mipmaps[level].empty();
}

// Initialises up to and including the given level.
void MiptexWrapper::initialiseMipmap(int32_t level)
{
	if ( !isValid() )
	{
		return;
	}

	if ( level < 0 || level >= MIPLEVELS )
	{
		level = MIPLEVELS - 1;
	}

	for ( uint32_t index = 0; index <= level; ++index )
	{
		m_Mipmaps[index].clear();
		m_Mipmaps[index].resize(areaForMipLevel(index), 0);
	}
}

void MiptexWrapper::setName(const char* name)
{
	if ( !name )
	{
		m_Name[0] = '\0';
		return;
	}

	safe_strncpy(m_Name, name, sizeof(m_Name));
}

int32_t MiptexWrapper::paletteIndexAt(uint32_t mipLevel, uint32_t x, uint32_t y) const
{
	if ( !hasMipmap(mipLevel) || mipLevel >= MIPLEVELS || x >= widthForMipLevel(mipLevel) || y >= heightForMipLevel(mipLevel) )
	{
		return -1;
	}

	const ByteArray& mipmap = m_Mipmaps[mipLevel];
	return static_cast<int32_t>(mipmap[(y * m_Width) + x]);
}

const MiptexWrapper::rgbpixel_t* MiptexWrapper::paletteColour(uint8_t paletteIndex) const
{
	if ( !hasPalette() )
	{
		return NULL;
	}

	return reinterpret_cast<const rgbpixel_t*>(&m_Palette[paletteIndex * sizeof(rgbpixel_t)]);
}

const MiptexWrapper::rgbpixel_t* MiptexWrapper::colourAt(uint32_t mipLevel, uint32_t x, uint32_t y) const
{
	int32_t paletteIndex = paletteIndexAt(mipLevel, x, y);
	if ( paletteIndex < 0 )
	{
		return NULL;
	}

	return paletteColour(paletteIndex);
}

uint8_t* MiptexWrapper::rawMipmapData(uint32_t level)
{
	if ( level >= MIPLEVELS || !hasMipmap(level) )
	{
		return NULL;
	}

	return m_Mipmaps[level].data();
}

const uint8_t* MiptexWrapper::rawMipmapData(uint32_t level) const
{
	if ( level >= MIPLEVELS || !hasMipmap(level) )
	{
		return NULL;
	}

	return m_Mipmaps[level].data();
}

MiptexWrapper::rgbpixel_t* MiptexWrapper::rawPaletteData()
{
	if ( !hasPalette() )
	{
		return NULL;
	}

	return reinterpret_cast<rgbpixel_t*>(m_Palette.data());
}

const MiptexWrapper::rgbpixel_t* MiptexWrapper::rawPaletteData() const
{
	if ( !hasPalette() )
	{
		return NULL;
	}

	return reinterpret_cast<const rgbpixel_t*>(m_Palette.data());
}

// According to the old code in bspfile.cpp, only the dmiptexlump_t data needs to be swapped for endianness.
// Once the miptex_t is being referenced, it's fine.
bool MiptexWrapper::setFromMiptex(const miptex_t* miptex, bool headerOnly)
{
	if ( !miptex )
	{
		WARNING("Invalid miptex input.");
		return false;
	}

	invalidate();

	safe_strncpy(m_Name, miptex->name, sizeof(m_Name));

	if ( !setDimensions(miptex->width, miptex->height) )
	{
		WARNING("Invalid dimensions %dx%d.", miptex->width, miptex->height);
		invalidate();
		return false;
	}

	if ( headerOnly )
	{
		return true;
	}

	const byte* mipmapAddresses[MIPLEVELS];
	memset(mipmapAddresses, 0, MIPLEVELS * sizeof(byte*));
	size_t totalMipDataSize = 0;
	int32_t maxMipLevel = -1;

	for ( uint32_t mipLevel = 0; mipLevel < MIPLEVELS; ++mipLevel )
	{
		if ( miptex->offsets[mipLevel] < 1 )
		{
			// Exit out at the first missing mipmap.
			break;
		}

		mipmapAddresses[mipLevel] = reinterpret_cast<const byte*>(miptex) + miptex->offsets[mipLevel];
		totalMipDataSize += areaForMipLevel(mipLevel);
		maxMipLevel = mipLevel;
	}

	const byte* paletteBase = NULL;
	uint16_t paletteSize = 0;

	// If there is at least some mipmap data, we need a valid palette.
	if ( totalMipDataSize > 0 )
	{
		paletteBase = reinterpret_cast<const byte*>(miptex) + sizeof(*miptex) + totalMipDataSize;
		paletteSize = *reinterpret_cast<const uint16_t*>(paletteBase);

		if ( paletteSize != PALETTE_SIZE)
		{
			WARNING("Invalid palette size - expected %u but got %u.", PALETTE_SIZE, paletteSize);
			invalidate();
			return false;
		}
	}

	if ( maxMipLevel >= 0 )
	{
		initialiseMipmap(maxMipLevel);
	}

	// Copy in the mipmap data.
	for ( uint32_t mipLevel = 0; mipLevel < MIPLEVELS; ++mipLevel )
	{
		if ( !mipmapAddresses[mipLevel] )
		{
			break;
		}
		else
		{
			memcpy(rawMipmapData(mipLevel), mipmapAddresses[mipLevel], m_Mipmaps[mipLevel].size());
		}
	}

	if ( paletteSize > 0 )
	{
		// Copy in the palette data.
		const byte* paletteData = paletteBase + sizeof(uint16_t);
		initialisePalette();
		memcpy(rawPaletteData(), paletteData, m_Palette.size());
	}

	return true;
}

bool MiptexWrapper::exportToMiptex(miptex_t* miptex) const
{
	if ( !isValid() )
	{
		return false;
	}

	safe_strncpy(miptex->name, m_Name, sizeof(miptex->name));
	miptex->width = m_Width;
	miptex->height = m_Height;

	if ( !hasValidImage() )
	{
		memset(miptex->offsets, 0, sizeof(miptex->offsets));
		return true;
	}

	byte* const mipDataBase = reinterpret_cast<byte*>(miptex) + sizeof(*miptex);
	int mipDataWritten = 0;

	for ( uint32_t mipLevel = 0; mipLevel < MIPLEVELS; ++mipLevel )
	{
		if ( m_Mipmaps[mipLevel].empty() )
		{
			miptex->offsets[mipLevel] = 0;
			continue;
		}

		miptex->offsets[mipLevel] = sizeof(*miptex) + mipDataWritten;
		memcpy(mipDataBase + mipDataWritten, rawMipmapData(mipLevel), m_Mipmaps[mipLevel].size());

		mipDataWritten += m_Mipmaps[mipLevel].size();
	}

	byte* const paletteBase = mipDataBase + mipDataWritten;
	uint16_t* const paletteSize = reinterpret_cast<uint16_t*>(paletteBase);
	byte* const paletteData = paletteBase + sizeof(uint16_t);

	*paletteSize = m_Palette.size() / sizeof(rgbpixel_t);
	memcpy(paletteData, rawPaletteData(), m_Palette.size());

	// Terminator
	*reinterpret_cast<uint16_t*>(paletteData + m_Palette.size()) = 0;

	return true;
}

int32_t MiptexWrapper::maxMipLevel() const
{
	for ( int32_t index = 0; index < MIPLEVELS; ++index )
	{
		if ( m_Mipmaps[index].empty() )
		{
			return index - 1;
		}
	}

	return MIPLEVELS - 1;
}

bool MiptexWrapper::hasValidImage() const
{
	return isValid() && hasAnyMipmap() && hasPalette();
}

size_t MiptexWrapper::exportDataSize() const
{
	return isValid() ? dataSize() : 0;
}

size_t MiptexWrapper::dataSize() const
{
	if ( !hasValidImage() )
	{
		return sizeof(miptex_t);
	}

	size_t size = sizeof(miptex_t);

	for ( uint32_t mipLevel = 0; mipLevel < MIPLEVELS; ++mipLevel )
	{
		// Stop at first invalid mipmap.
		if ( m_Mipmaps[mipLevel].size() < 1 )
		{
			break;
		}

		size += m_Mipmaps[mipLevel].size();
	}

	size += sizeof(uint16_t);	// Palette size
	size += m_Palette.size();
	size += sizeof(uint16_t);	// Null terminator

	return size;
}

uint32_t MiptexWrapper::widthForMipLevel(uint32_t level) const
{
	return dimensionForMipLevel(m_Width, level);
}

uint32_t MiptexWrapper::heightForMipLevel(uint32_t level) const
{
	return dimensionForMipLevel(m_Height, level);
}

uint32_t MiptexWrapper::areaForMipLevel(uint32_t level) const
{
	return areaForMipLevel(m_Width, m_Height, level);
}

uint32_t MiptexWrapper::dimensionForMipLevel(uint32_t dim, uint32_t level)
{
	if ( level >= MIPLEVELS )
	{
		level = MIPLEVELS - 1;
	}

	return dim >> level;
}

uint32_t MiptexWrapper::areaForMipLevel(uint32_t width, uint32_t height, uint32_t level)
{
	return dimensionForMipLevel(width, level) * dimensionForMipLevel(height, level);
}

uint32_t MiptexWrapper::totalIdealBytesRequired(uint32_t width, uint32_t height)
{
	size_t size = sizeof(miptex_t);

	for ( uint32_t mipLevel = 0; mipLevel < MIPLEVELS; ++mipLevel )
	{
		size += areaForMipLevel(width, height, mipLevel);
	}

	size += sizeof(uint16_t);					// Palette size
	size += PALETTE_SIZE * sizeof(rgbpixel_t);	// Palette data
	size += sizeof(uint16_t);					// Null terminator

	return size;
}

// Nothing past the end of the miptex header should be assumed to be readable.
uint32_t MiptexWrapper::totalBytesRequired(const miptex_t* miptex, bool headerOnly)
{
	if ( !miptex )
	{
		return 0;
	}

	uint32_t size = 0;

	if ( !headerOnly )
	{
		for ( uint32_t mipLevel = 0; mipLevel < MIPLEVELS; ++mipLevel )
		{
			if ( miptex->offsets[mipLevel] >= sizeof(miptex_t) )
			{
				size += areaForMipLevel(miptex->width, miptex->height, mipLevel);
			}
		}

		if ( size < 1 )
		{
			headerOnly = true;
		}
	}

	if ( headerOnly )
	{
		return sizeof(miptex_t);
	}

	// We definitely have some mipmap data, so add on the rest of the data sizes.
	// We already calculated mipmap bytes, so we need:
	size += sizeof(miptex_t);					// Header
	size += sizeof(uint16_t);					// Palette size
	size += MiptexWrapper::PALETTE_SIZE * 3;	// Required number of palette colours
	size += sizeof(uint16_t);					// Terminator

	return size;
}
