#ifndef PNGTEXTURE_H
#define PNGTEXTURE_H

#include <cstdint>
#include <vector>
#include <string>
#include "bspfile.h"

class PNGTexturePath
{
public:
	// -1 to include room for terminator when writing to textures lump.
	static constexpr size_t MAX_PATH_LENGTH = MAX_TEXTURE_NAME_LENGTH - 1;

	PNGTexturePath();

	void invalidate();

	std::string path() const;
	bool setPath(const std::string& path);
	bool hasValidPath() const;

	// Exporting this texture just exports the path, as it is loaded from disk when required.
	size_t exportDataSize() const;
	bool exportData(void* buffer, size_t length) const;

private:
	std::string m_Path;
};

#endif // PNGTEXTURE_H
