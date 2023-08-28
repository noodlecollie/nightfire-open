#ifndef TEXTURECOLLECTIONWRITER_H
#define TEXTURECOLLECTIONWRITER_H

#include <vector>
#include "mathtypes.h"
#include "bspfile.h"

class TextureCollection;

class TextureCollectionWriter
{
public:
	TextureCollectionWriter(const TextureCollection& collection);

	bool exportAll();
	int calculateChecksum();

	const std::vector<byte>& exportedData() const;

	// This maps a texture's original index to its new index, as the textures are reordered during export.
	const std::vector<int32_t>& indexMap() const;

private:
	void writePngPaths(dpngtexturepath_t* paths, uint32_t count);
	void writeMiptexData(uint32_t* headerOffsets, uint32_t count, uint32_t pngCount);
	void normaliseSlashes(char* path);

	const TextureCollection& m_Collection;
	std::vector<byte> m_ExportData;
	std::vector<int32_t> m_IndexMap;
};

#endif // TEXTURECOLLECTIONWRITER_H
