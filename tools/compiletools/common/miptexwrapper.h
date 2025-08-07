#ifndef MIPTEXWRAPPER_H
#define MIPTEXWRAPPER_H

#include "bspfile.h"
#include <vector>
#include <memory>
#include "cppmemory.h"

/*
	Miptex structure, as far as I have been able to deduce:
	- Name: MIPTEX_NAME_LENGTH chars
	- Width: uint32
	- Height: uint32
	- Mipmap offsets: 4 * int32 (offsets are from beginning of miptex, or -1)
	- Mipmap 0: width * height bytes
	- Mipmap 1: (width / 2) * (height / 2) bytes
	- Mipmap 2: (width / 4) * (height / 4) bytes
	- Mipmap 3: (width / 8) * (height / 8) bytes
	- Palette size: uint16
	- Palette data: size * 3 bytes (ie. size * RGB pixels)
	- Unknown: uint16, zero. Terminator? Padding?
*/
class MiptexWrapper
{
public:
	typedef byte rgbpixel_t[3];
	static constexpr uint32_t PALETTE_SIZE = 256;

	// Passing width and height is equivalent to calling setDimensions().
	MiptexWrapper(const char* name = NULL, uint32_t width = 0, uint32_t height = 0);

	// A texture is valid if it has a non-zero area.
	// A valid texture can still be without mipmaps or a palette -
	// for example, if these are stored in some external WAD.
	inline bool isValid() const
	{
		return m_Width > 0 && m_Height > 0;
	}

	void invalidate(bool clearName = true);

	uint32_t width() const;
	uint32_t height() const;

	// The returned name will never be NULL.
	const char* name() const;
	void setName(const char* newName);

	bool hasPalette() const;
	void initialisePalette();

	bool hasAnyMipmap() const;
	bool hasMipmaps(uint32_t level) const;	// Up to and including the given level.
	bool hasMipmap(uint32_t level) const;
	void initialiseMipmap(int32_t level = -1);	// Up to and including the given level. -1 initialises all.
	int32_t maxMipLevel() const;

	// These functions are purely mathematical - they do not require data to
	// have been allocated yet for the target mip level.
	uint32_t widthForMipLevel(uint32_t level) const;
	uint32_t heightForMipLevel(uint32_t level) const;
	uint32_t areaForMipLevel(uint32_t level) const;

	// Returns an index into the colour palette for the specified pixel, or -1 on error.
	int32_t paletteIndexAt(uint32_t mipLevel, uint32_t x, uint32_t y) const;

	// Length of data is areaForMipLevel(level) bytes.
	// Data is stored in row-major order ((y * width) + x).
	uint8_t* rawMipmapData(uint32_t level);
	const uint8_t* rawMipmapData(uint32_t level) const;

	// Length of data is PALETTE_SIZE * sizeof(rgbpixel_t) bytes.
	rgbpixel_t* rawPaletteData();
	const rgbpixel_t* rawPaletteData() const;

	// Returns a pointer to the RGB triple for the palette index, or NULL on error.
	const rgbpixel_t* paletteColour(uint8_t paletteIndex) const;

	// Convenience function to get the colour at a given position on the texture.
	// Returns NULL on error.
	const rgbpixel_t* colourAt(uint32_t mipLevel, uint32_t x, uint32_t y) const;

	// Palette and mipmaps will be initialised to blank (ie. black, index 0).
	// Returns true on success, or false if the dimensions are not valid.
	// In this case, the entire texture will be reset to an invalid state.
	bool initialise(uint32_t width, uint32_t height);

	// Same as above, but palette and mipmaps are left undimensioned.
	bool setDimensions(uint32_t width, uint32_t height);

	// Sets the colour palette to be black, and all mipmaps to use the colour at index 0.
	// The dimensions of the mipmaps will be initialised appropriately.
	void setBlank();

	// Assumes that the miptex pointer contains enough data given its dimensions.
	bool setFromMiptex(const miptex_t* miptex, bool headerOnly = false);

	// Assumes that the miptex pointer points to exportDataSize() bytes of data.
	bool exportToMiptex(miptex_t* miptex) const;
	size_t exportDataSize() const;
	bool hasValidImage() const;

	// Numebr of bytes currently being used by the texture, regardless of
	// whether it is in an exportable state.
	size_t dataSize() const;

	static uint32_t dimensionForMipLevel(uint32_t dim, uint32_t level);
	static uint32_t areaForMipLevel(uint32_t width, uint32_t height, uint32_t level);
	static uint32_t totalIdealBytesRequired(uint32_t width, uint32_t height);
	static uint32_t totalBytesRequired(const miptex_t* miptex, bool headerOnly = false);

private:
	typedef std::vector<byte> ByteArray;

	char m_Name[MAX_TEXTURE_NAME_LENGTH];
	uint32_t m_Width;
	uint32_t m_Height;
	ByteArray m_Mipmaps[MIPLEVELS];
	ByteArray m_Palette;
};

#endif // MIPTEXWRAPPER_H
