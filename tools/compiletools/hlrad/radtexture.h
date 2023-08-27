#ifndef RADTEXTURE_H
#define RADTEXTURE_H

#include <string>
#include <cstdint>
#include <vector>
#include "mathtypes.h"
#include "miptexwrapper.h"

class RadTexture
{
public:
	typedef byte RGB[3];
	typedef byte RGBA[4];

	enum AttributeFlag
	{
		IsAlphaMasked = (1 << 0),
		IsTranslucent = (1 << 1),
		IsSpecial = (1 << 2)
	};

	RadTexture();

	bool isValid() const;
	uint32_t width() const;
	uint32_t height() const;
	uint32_t totalPixels() const;
	uint32_t attributeFlags() const;

	void invalidate(bool clearName = true);

	// If the name is a path, the separator is assumed to be '/'
	// since PNGs from a BSP textures lump should always use this.
	std::string name() const;
	void setName(const std::string& name, bool isPath);

	void reflectivity(vec3_t reflectivity) const;
	void setReflectivity(const vec3_t reflectivity);

	bool hasPalette() const;
	const RGB* paletteColour(uint8_t index) const;
	const RGB* paletteTransparentColour() const;
	void setPaletteColour(uint8_t index, const RGB* colour);

	const RGB* canvasColour(uint32_t sequentialIndex) const;
	const RGB* canvasColour(uint32_t x, uint32_t y) const;

	// 0 = transparent, 255 = opaque.
	// Takes into account attributes.
	uint8_t opacity(uint32_t sequentialIndex) const;
	uint8_t opacity(uint32_t x, uint32_t y) const;

	// Only supported with non-palette textures.
	const RGBA* canvasColourWithAlpha(uint32_t sequentialIndex) const;
	const RGBA* canvasColourWithAlpha(uint32_t x, uint32_t y) const;

	// Loads with palette.
	bool loadFromMiptex(const MiptexWrapper& miptex);

	// Loads without palette.
	// Width and height must be multiples of 16.
	// Data is expected in row-major order.
	bool loadFromRGBAData(uint32_t width, uint32_t height, const RGBA* data, uint32_t dataLengthInPixels);
	bool loadFromRGBData(uint32_t width, uint32_t height, const RGB* data, uint32_t dataLengthInPixels);
	void setToDefaultTextureImage(const std::string& name);

private:
	static bool isSpecialTexturePath(const std::string& path);
	static bool isPowerOfTwo(uint32_t num);

	const RGB* canvasColourViaPalette(uint32_t sequentialIndex) const;
	const RGB* canvasColourDirect(uint32_t sequentialIndex) const;
	void computeAttributesFromName(bool treatAsPath);

	std::string m_Name;
	uint32_t m_Width;
	uint32_t m_Height;
	std::vector<byte> m_Canvas;	// Bytes if palette is used, RGBA otherwise.
	RGB m_Palette[MiptexWrapper::PALETTE_SIZE];
	vec3_t m_Reflectivity;
	bool m_IgnorePalette;
	uint32_t m_Attributes;
};

#endif // RADTEXTURE_H
