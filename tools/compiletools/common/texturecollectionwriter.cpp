#include "texturecollectionwriter.h"
#include "texturecollection.h"
#include "miptexwrapper.h"
#include "hlassert.h"
#include "checksum.h"
#include "pngtexturepath.h"
#include "log.h"

TextureCollectionWriter::TextureCollectionWriter(const TextureCollection& collection) :
	m_Collection(collection),
	m_ExportData(),
	m_IndexMap()
{
}

const std::vector<byte>& TextureCollectionWriter::exportedData() const
{
	return m_ExportData;
}

const std::vector<int32_t>& TextureCollectionWriter::indexMap() const
{
	return m_IndexMap;
}

int TextureCollectionWriter::calculateChecksum()
{
	if ( m_ExportData.size() < 1 )
	{
		exportAll();
	}

	if ( m_ExportData.size() < 1 )
	{
		return 0;
	}

	return FastChecksum(m_ExportData.data(), m_ExportData.size());
}

bool TextureCollectionWriter::exportAll()
{
	m_IndexMap.clear();

	const uint32_t miptexCount = m_Collection.exportableTextureCount(TextureCollection::ItemType::Miptex);
	const uint32_t miptexOffsetBytesReqiured = miptexCount * sizeof(uint32_t);
	const uint32_t miptexDataBytesRequired = m_Collection.exportBytesRequired(TextureCollection::ItemType::Miptex);

	const uint32_t pngCount = m_Collection.exportableTextureCount(TextureCollection::ItemType::PngOnDisk);
	const uint32_t pngDataBytesRequired = pngCount * sizeof(dpngtexturepath_t);

	const uint32_t totalCount = miptexCount + pngCount;
	m_IndexMap.resize(totalCount, -1);

	const size_t totalBytesRequired =
		sizeof(dtexturelumpheader_t) +	// Header
		pngDataBytesRequired +			// All PNG paths
		miptexOffsetBytesReqiured +		// List of miptex data offsets
		miptexDataBytesRequired;		// Actual miptex data

	m_ExportData.resize(totalBytesRequired);

	dtexturelumpheader_t* header = reinterpret_cast<dtexturelumpheader_t*>(m_ExportData.data());
	header->pngCount = pngCount;
	header->miptexCount = miptexCount;

	dpngtexturepath_t* pngPaths = reinterpret_cast<dpngtexturepath_t*>(m_ExportData.data() + sizeof(dtexturelumpheader_t));
	writePngPaths(pngPaths, pngCount);

	uint32_t* miptexOffsetsBegin = reinterpret_cast<uint32_t*>(m_ExportData.data() + sizeof(dtexturelumpheader_t) + pngDataBytesRequired);
	writeMiptexData(miptexOffsetsBegin, miptexCount, pngCount);

	// Do a final byte swap on the header.
	header->pngCount = LittleLong(header->pngCount);
	header->miptexCount = LittleLong(header->miptexCount);

	for ( int32_t newIndex : m_IndexMap )
	{
		hlassert(newIndex >= 0);
	}

	if ( g_developer > DEVELOPER_LEVEL_SPAM )
	{
		for ( uint32_t index = 0; index < m_IndexMap.size(); ++index )
		{
			Developer(DEVELOPER_LEVEL_SPAM, "Texture %u (%s) remapped to index %u on export.\n",
					  index,
					  m_Collection.itemName(index).c_str(),
					  m_IndexMap[index]);
		}
	}

	return true;
}

void TextureCollectionWriter::writePngPaths(dpngtexturepath_t* paths, uint32_t count)
{
	const uint32_t totalCount = m_Collection.count();
	uint32_t nextPathIndex = 0;

	for ( uint32_t index = 0; index < totalCount; ++index )
	{
		const PNGTexturePath* texture = m_Collection.pngTextureAt(index);

		if ( !texture || !texture->hasValidPath() )
		{
			continue;
		}

		hlassert(texture->path().size() < MAX_TEXTURE_NAME_LENGTH);
		hlassert(nextPathIndex < count);

		dpngtexturepath_t& currentPath = paths[nextPathIndex];

		strncpy(currentPath.path, texture->path().c_str(), sizeof(paths[nextPathIndex].path));
		currentPath.path[sizeof(currentPath.path) - 1] = '\0';
		normaliseSlashes(currentPath.path);

		hlassert(m_IndexMap[index] == ~0);
		m_IndexMap[index] = nextPathIndex;

		++nextPathIndex;
	}

	hlassert(nextPathIndex == count);
}

void TextureCollectionWriter::writeMiptexData(uint32_t* headerOffsets, uint32_t count, uint32_t pngCount)
{
	byte* data = reinterpret_cast<byte*>(&headerOffsets[count]);
	const uint32_t totalCount = m_Collection.count();
	uint32_t nextMiptexIndex = 0;

	for ( uint32_t index = 0; index < totalCount; ++index )
	{
		const MiptexWrapper* wrapper = m_Collection.miptexAt(index);

		if ( !wrapper || !wrapper->isValid() )
		{
			continue;
		}

		hlassert(nextMiptexIndex < count);
		hlassert(m_IndexMap[index] == ~0);

		m_IndexMap[index] = pngCount + nextMiptexIndex;

		const uint32_t offset = data - m_ExportData.data();
		headerOffsets[nextMiptexIndex] = offset;

		const uint32_t exportBytes = wrapper->exportDataSize();
		hlassert(exportBytes > 0);
		hlassert(offset + exportBytes <= m_ExportData.size());

		wrapper->exportToMiptex(reinterpret_cast<miptex_t*>(data));

		data += exportBytes;
		++nextMiptexIndex;
	}

	hlassert(nextMiptexIndex == count);
}

void TextureCollectionWriter::normaliseSlashes(char* path)
{
	if ( !path )
	{
		return;
	}

	for ( char* current = path; *current; ++current )
	{
		if ( *current == '\\' )
		{
			*current = '/';
		}
	}
}
