#include "radtexture.h"
#include "hlassert.h"
#include "filelib.h"

RadTexture::RadTexture() :
	m_Name(),
	m_Width(0),
	m_Height(0),
	m_Canvas(),
	m_Palette(),
	m_Reflectivity(),
	m_IgnorePalette(false),
	m_Attributes(0)
{
	memset(m_Palette, 0, sizeof(m_Palette));
	memset(m_Reflectivity, 0, sizeof(m_Reflectivity));
}

bool RadTexture::isValid() const
{
	return m_Width > 0 && m_Height > 0;
}

uint32_t RadTexture::width() const
{
	return m_Width;
}

uint32_t RadTexture::height() const
{
	return m_Height;
}

uint32_t RadTexture::totalPixels() const
{
	return m_Width * m_Height;
}

uint32_t RadTexture::attributeFlags() const
{
	return m_Attributes;
}

void RadTexture::invalidate(bool clearName)
{
	m_Width = 0;
	m_Height = 0;
	memset(m_Palette, 0, sizeof(m_Palette));
	memset(m_Reflectivity, 0, sizeof(m_Reflectivity));
	m_Canvas.clear();
	m_IgnorePalette = false;
	m_Attributes = 0;

	if ( clearName )
	{
		m_Name.clear();
	}
}

std::string RadTexture::name() const
{
	return m_Name;
}

void RadTexture::setName(const std::string& name, bool isPath)
{
	m_Name = name;
	computeAttributesFromName(isPath);
}

void RadTexture::reflectivity(vec3_t reflectivity) const
{
	if ( !reflectivity )
	{
		return;
	}

	memcpy(reflectivity, m_Reflectivity, sizeof(vec3_t));
}

void RadTexture::setReflectivity(const vec3_t reflectivity)
{
	if ( !reflectivity )
	{
		return;
	}

	memcpy(m_Reflectivity, reflectivity, sizeof(vec3_t));
}

bool RadTexture::hasPalette() const
{
	return !m_IgnorePalette;
}

const RadTexture::RGB* RadTexture::paletteColour(uint8_t index) const
{
	if ( !isValid() || !hasPalette() )
	{
		return NULL;
	}

	return &m_Palette[index];
}

const RadTexture::RGB* RadTexture::paletteTransparentColour() const
{
	return paletteColour(255);
}

void RadTexture::setPaletteColour(uint8_t index, const RGB* colour)
{
	if ( !isValid() || !hasPalette() || !colour )
	{
		return;
	}

	memcpy(&m_Palette[index], colour, sizeof(RGB));
}

const RadTexture::RGB* RadTexture::canvasColour(uint32_t sequentialIndex) const
{
	if ( !isValid() )
	{
		return NULL;
	}

	if ( hasPalette() )
	{
		return canvasColourViaPalette(sequentialIndex);
	}
	else
	{
		return canvasColourDirect(sequentialIndex);
	}
}

const RadTexture::RGB* RadTexture::canvasColour(uint32_t x, uint32_t y) const
{
	if ( !isValid() )
	{
		return NULL;
	}

	return canvasColour((y * m_Width) + x);
}

const RadTexture::RGBA* RadTexture::canvasColourWithAlpha(uint32_t sequentialIndex) const
{
	if ( !isValid() || hasPalette() || sequentialIndex >= m_Canvas.size() / sizeof(RGBA) )
	{
		return NULL;
	}

	return reinterpret_cast<const RGBA*>(m_Canvas.data()) + sequentialIndex;
}

const RadTexture::RGBA* RadTexture::canvasColourWithAlpha(uint32_t x, uint32_t y) const
{
	return canvasColourWithAlpha((y * m_Width) + x);
}

const RadTexture::RGB* RadTexture::canvasColourViaPalette(uint32_t sequentialIndex) const
{
	if ( sequentialIndex >= m_Canvas.size() )
	{
		return NULL;
	}

	const byte paletteIndex = m_Canvas[sequentialIndex];
	return &m_Palette[paletteIndex];
}

const RadTexture::RGB* RadTexture::canvasColourDirect(uint32_t sequentialIndex) const
{
	// Because RGB is a truncation of RGBA, we can just call this and truncate the return type.
	return reinterpret_cast<const RGB*>(canvasColourWithAlpha(sequentialIndex));
}

uint8_t RadTexture::opacity(uint32_t sequentialIndex) const
{
	if ( !isValid() || sequentialIndex >= m_Canvas.size() )
	{
		return 0xFF;
	}

	if ( hasPalette() )
	{
		// Only masked alpha is supported.
		// If the index into the palette is the last index, this means it is transparent.
		if ( (m_Attributes & IsAlphaMasked) && m_Canvas[sequentialIndex] == 0xFF )
		{
			return 0x00;
		}
	}
	else
	{
		const RGBA* pixel = reinterpret_cast<const RGBA*>(m_Canvas.data()) + sequentialIndex;

		if ( m_Attributes & IsAlphaMasked )
		{
			// I dunno what the canonical way of doing this is, but I'm gonna
			// treat anything that's not fully opaque as being transparent.
			return (*pixel)[3] < 0xFF ? 0x00 : 0xFF;
		}
		else if ( m_Attributes & IsTranslucent )
		{
			// Any value is allowed.
			return (*pixel)[3];
		}
	}

	// By default, return opaque.
	return 0xFF;
}

uint8_t RadTexture::opacity(uint32_t x, uint32_t y) const
{
	if ( !isValid() )
	{
		return 0xFF;
	}

	return opacity((y * m_Width) + x);
}

bool RadTexture::loadFromMiptex(const MiptexWrapper& miptex)
{
	invalidate();

	if ( !miptex.isValid() || !miptex.hasMipmap(0) || !miptex.hasPalette() )
	{
		return false;
	}

	m_Width = miptex.width();
	m_Height = miptex.height();
	m_IgnorePalette = false;
	setName(miptex.name(), false);

	m_Canvas.resize(miptex.areaForMipLevel(0));
	memcpy(m_Canvas.data(), miptex.rawMipmapData(0), m_Canvas.size());

	memcpy(m_Palette, miptex.rawPaletteData(), sizeof(m_Palette));

	return true;
}

bool RadTexture::loadFromRGBAData(uint32_t width, uint32_t height, const RGBA* data, uint32_t dataLengthInPixels)
{
	invalidate(false);

	if ( width < 1 || height < 1 ||
		 !isPowerOfTwo(width) || !isPowerOfTwo(height) ||
		 !data || dataLengthInPixels < width * height )
	{
		return false;
	}

	m_Width = width;
	m_Height = height;
	m_IgnorePalette = true;

	const uint32_t totalBytes = m_Width * m_Height * sizeof(RGBA);
	m_Canvas.resize(totalBytes);
	memcpy(m_Canvas.data(), data, totalBytes);

	memset(m_Palette, 0, sizeof(m_Palette));

	return true;
}

bool RadTexture::loadFromRGBData(uint32_t width, uint32_t height, const RGB* data, uint32_t dataLengthInPixels)
{
	invalidate(false);

	if ( width < 1 || height < 1 ||
		 !isPowerOfTwo(width) || !isPowerOfTwo(height) ||
		 !data || dataLengthInPixels < width * height )
	{
		return false;
	}

	m_Width = width;
	m_Height = height;
	m_IgnorePalette = true;

	const uint32_t totalBytes = m_Width * m_Height * sizeof(RGBA);
	m_Canvas.resize(totalBytes);

	for ( uint32_t index = 0; index < m_Width * m_Height; ++index )
	{
		const RGB* pixel = data + index;
		RGBA* dest = reinterpret_cast<RGBA*>(m_Canvas.data()) + index;

		// Fill the alpha channel to opaque.
		(*dest)[0] = (*pixel)[0];
		(*dest)[1] = (*pixel)[1];
		(*dest)[2] = (*pixel)[2];
		(*dest)[3] = 0xFF;
	}

	memset(m_Palette, 0, sizeof(m_Palette));

	return true;
}

void RadTexture::setToDefaultTextureImage(const std::string& name)
{
	invalidate();

	m_Width = 16;
	m_Height = 16;
	m_IgnorePalette = false;
	m_Name = name;

	m_Canvas.resize(m_Width * m_Height, 0);

	memset(m_Palette, 0x80, sizeof(m_Palette));
}

void RadTexture::computeAttributesFromName(bool treatAsPath)
{
	m_Attributes = 0;
	std::string name = m_Name;

	if ( treatAsPath )
	{
		std::vector<char> buffer(m_Name.size());
		FS_FileBase(m_Name.c_str(), buffer.data());
		name = std::string(buffer.data());
	}

	if ( strncmp(name.c_str(), SPECIALTEX_TRANSPARENT_PREFIX, sizeof(SPECIALTEX_TRANSPARENT_PREFIX) - 1) == 0 )
	{
		m_Attributes |= IsAlphaMasked;
	}
	else if ( strncmp(name.c_str(), SPECIALTEX_TRANSLUCENT_PREFIX_ONLY, sizeof(SPECIALTEX_TRANSLUCENT_PREFIX_ONLY) - 1) == 0 ||
			  strncmp(name.c_str(), SPECIALTEX_TRANSLUCENT, sizeof(SPECIALTEX_TRANSLUCENT) - 1) == 0 )
	{
		m_Attributes |= IsTranslucent;
	}

	if ( isSpecialTexturePath(m_Name) )
	{
		m_Attributes |= IsSpecial;
	}
}

bool RadTexture::isPowerOfTwo(uint32_t num)
{
	bool hasBit = false;

	for ( uint32_t bitIndex = 0; bitIndex < sizeof(uint32_t) * 8; ++bitIndex )
	{
		if ( num & (1 << bitIndex) )
		{
			if ( hasBit )
			{
				return false;
			}

			hasBit = true;
		}
	}

	return true;
}

bool RadTexture::isSpecialTexturePath(const std::string& path)
{
	// This is super icky, but yeah. This entire codebase is ugly anyway.
#define PATH_IS_EQUAL(specialName) (strcasecmp(path.c_str(), (specialName)) == 0)
#define PATH_BEGINS_WITH(specialName) (strncasecmp(path.c_str(), (specialName), sizeof(specialName) - 1) == 0)

	return
		PATH_BEGINS_WITH(SPECIALTEX_LIQUID_LAVA) ||
		PATH_BEGINS_WITH(SPECIALTEX_LIQUID_SLIME) ||
		PATH_BEGINS_WITH(SPECIALTEX_LIQUID_WATER) ||
		PATH_BEGINS_WITH(SPECIALTEX_LIQUID_LASER) ||
		PATH_IS_EQUAL(SPECIALTEX_LIQUID_CURRENT_0) ||
		PATH_IS_EQUAL(SPECIALTEX_LIQUID_CURRENT_90) ||
		PATH_IS_EQUAL(SPECIALTEX_LIQUID_CURRENT_180) ||
		PATH_IS_EQUAL(SPECIALTEX_LIQUID_CURRENT_270) ||
		PATH_IS_EQUAL(SPECIALTEX_LIQUID_CURRENT_UP) ||
		PATH_IS_EQUAL(SPECIALTEX_LIQUID_CURRENT_DOWN) ||
		PATH_IS_EQUAL(SPECIALTEX_SKIP) ||
		PATH_IS_EQUAL(SPECIALTEX_HINT) ||
		PATH_IS_EQUAL(SPECIALTEX_NULL) ||
		PATH_IS_EQUAL(SPECIALTEX_NODRAW) ||
		PATH_IS_EQUAL(SPECIALTEX_SKY) ||
		PATH_IS_EQUAL(SPECIALTEX_CLIP) ||
		PATH_IS_EQUAL(SPECIALTEX_PLAYERCLIP) ||
		PATH_IS_EQUAL(SPECIALTEX_ORIGIN) ||
		PATH_IS_EQUAL(SPECIALTEX_AAATRIGGER) ||
		PATH_IS_EQUAL(SPECIALTEX_BEVEL) ||
		PATH_IS_EQUAL(SPECIALTEX_BOUNDINGBOX) ||
		PATH_IS_EQUAL(SPECIALTEX_TRANSLUCENT) ||
		PATH_IS_EQUAL(SPECIALTEX_SOLIDHINT) ||
		PATH_IS_EQUAL(SPECIALTEX_SPLITFACE) ||
		PATH_IS_EQUAL(SPECIALTEX_CUSTOMCONTENT_SOLID) ||
		PATH_IS_EQUAL(SPECIALTEX_CUSTOMCONTENT_WATER) ||
		PATH_IS_EQUAL(SPECIALTEX_CUSTOMCONTENT_EMPTY) ||
		PATH_IS_EQUAL(SPECIALTEX_CUSTOMCONTENT_SKY) ||
		PATH_IS_EQUAL(BRUSHKEY_NOCLIP) ||
		PATH_IS_EQUAL(BRUSHKEY_NULLNOCLIP) ||
		PATH_IS_EQUAL(BRUSHKEY_BEVELBRUSH) ||
		PATH_IS_EQUAL(BRUSHKEY_CLIPBEVEL) ||
		PATH_IS_EQUAL(BRUSHKEY_CLIPBEVELBRUSH) ||
		PATH_BEGINS_WITH(BRUSHKEY_PREFIX_CLIPHULL)
		;

#undef PATH_BEGINS_WITH
#undef PATH_IS_EQUAL
}
