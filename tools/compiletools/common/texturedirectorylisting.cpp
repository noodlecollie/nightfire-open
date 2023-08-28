#include "texturedirectorylisting.h"
#include <vector>
#include <algorithm>
#include <cctype>
#include "log.h"
#include "filelib.h"
#include "cmdlib.h"
#include "hlassert.h"

#define WARNING(...) Warning("TextureDirectoryListing: " __VA_ARGS__)

namespace
{
	static inline void ltrim(std::string& str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch)
		{
			return !std::isspace(ch);
		}));
	}

	static inline void rtrim(std::string& str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch)
		{
			return !std::isspace(ch);
		}).base(), str.end());
	}

	static inline void trim(std::string& str)
	{
		ltrim(str);
		rtrim(str);
	}

	static inline void toLower(std::string& str)
	{
		std::for_each(str.begin(), str.end(), [](char & c)
		{
			c = ::tolower(c);
		});
	}
}

TextureDirectoryListing::TextureDirectoryListing() :
	m_TextureDirPath(),
	m_TextureToIndex(),
	m_NextTextureIndex(0),
	m_NumTexturePathsSearched(0),
	m_IndexToTexturePath()
{
}

std::string TextureDirectoryListing::textureDirPath()
{
	return m_TextureDirPath;
}

void TextureDirectoryListing::setTextureDirPath(const std::string& path)
{
	std::string trimmed = path;
	trim(trimmed);

	if ( trimmed.at(trimmed.size() - 1) == SYSTEM_SLASH_CHAR )
	{
		m_TextureDirPath = trimmed.substr(0, trimmed.length() - 1);
	}
	else
	{
		m_TextureDirPath = trimmed;
	}
}

void TextureDirectoryListing::textureList(std::vector<std::string>& list) const
{
	list.clear();

	for ( auto iterator = m_TextureToIndex.begin(); iterator != m_TextureToIndex.end(); ++iterator )
	{
		list.push_back(iterator->first);
	}
}

void TextureDirectoryListing::referencedTextureList(std::vector<std::string>& list) const
{
	list.clear();

	for ( const std::string& path : m_IndexToTexturePath )
	{
		list.push_back(path);
	}
}

void TextureDirectoryListing::clear()
{
	m_TextureToIndex.clear();
	m_IndexToTexturePath.clear();
}

size_t TextureDirectoryListing::count() const
{
	return m_TextureToIndex.size();
}

uint32_t TextureDirectoryListing::texturePathsSearched() const
{
	return m_NumTexturePathsSearched;
}

std::string TextureDirectoryListing::texturePath(const uint32_t index) const
{
	if ( index >= m_IndexToTexturePath.size() )
	{
		return std::string();
	}

	return m_IndexToTexturePath[index];
}

bool TextureDirectoryListing::containsTexture(const std::string& textureRelPath) const
{
	return caseSensPathIterator(makeSystemCanonicalTexturePath(textureRelPath)) != m_InsensToSensPath.end();
}

bool TextureDirectoryListing::textureIsReferenced(const std::string& textureRelPath) const
{
	PathCaseMap::const_iterator iterator = caseSensPathIterator(makeSystemCanonicalTexturePath(textureRelPath));
	if ( iterator == m_InsensToSensPath.end() )
	{
		return false;
	}

	TextureIndexMap::const_iterator indexIt = m_TextureToIndex.find(iterator->second);
	return indexIt != m_TextureToIndex.end() && indexIt->second != INVALID_TEXTURE_INDEX;
}

int32_t TextureDirectoryListing::textureIndex(const std::string& textureRelPath) const
{
	PathCaseMap::const_iterator iterator = caseSensPathIterator(makeSystemCanonicalTexturePath(textureRelPath));
	if ( iterator == m_InsensToSensPath.end() )
	{
		return false;
	}

	TextureIndexMap::const_iterator indexIt = m_TextureToIndex.find(iterator->second);
	return indexIt != m_TextureToIndex.end() ? indexIt->second : INVALID_TEXTURE_INDEX;
}

int32_t TextureDirectoryListing::assignTextureIndex(const std::string& textureRelPath)
{
	// In practice we should never reach this.
	if ( m_NextTextureIndex < 0 )
	{
		return INVALID_TEXTURE_INDEX;
	}

	const std::string canonicalPath = makeSystemCanonicalTexturePath(textureRelPath);
	PathCaseMap::const_iterator iterator = caseSensPathIterator(canonicalPath);
	if ( iterator == m_InsensToSensPath.end() )
	{
		return INVALID_TEXTURE_INDEX;
	}

	TextureIndexMap::iterator texIt = m_TextureToIndex.find(iterator->second);

	if ( texIt->second == INVALID_TEXTURE_INDEX )
	{
		texIt->second = m_NextTextureIndex++;

		hlassert(texIt->second < m_IndexToTexturePath.size());
		m_IndexToTexturePath[texIt->second] = texIt->first;

		Developer(DEVELOPER_LEVEL_MESSAGE, "Assigned index %u to texture %s\n", texIt->second, canonicalPath.c_str());
	}

	return texIt->second;
}

bool TextureDirectoryListing::makeListing()
{
	m_TextureToIndex.clear();
	m_InsensToSensPath.clear();
	m_NumTexturePathsSearched = 0;

	if ( m_TextureDirPath.empty() )
	{
		WARNING("No texture directory path was set.");
		return false;
	}

	std::vector<std::string> dirPaths;

	if ( !getTextureDirectories(dirPaths) )
	{
		return false;
	}

	for ( const std::string& path : dirPaths )
	{
		if ( !readTexturesFromDirectory(path) )
		{
			return false;
		}

		++m_NumTexturePathsSearched;
	}

	return true;
}

bool TextureDirectoryListing::getTextureDirectories(std::vector<std::string>& dirPaths) const
{
	DirectoryEntry_t* dirEntries = FS_ListDirectory(m_TextureDirPath.c_str(), NULL);

	if ( !dirEntries )
	{
		WARNING("Could not open texture directory: %s", m_TextureDirPath.c_str());
		return false;
	}

	for ( DirectoryEntry_t* entry = dirEntries; entry; entry = entry->next )
	{
		// Note that the current and parent directories show up in this listing, so they should be ignored.
		if ( entry->isDirectory && strcmp(entry->name, ".") != 0 && strcmp(entry->name, "..") != 0 )
		{
			dirPaths.push_back(entry->name);
		}
	}

	FS_FreeDirectoryEntries(dirEntries);
	return true;
}

bool TextureDirectoryListing::readTexturesFromDirectory(const std::string& path)
{
	std::string fullPath = m_TextureDirPath;

	if ( path.size() > 0 )
	{
		fullPath += std::string(SYSTEM_SLASH_STR) + path;
	}

	DirectoryEntry_t* entryList = FS_ListDirectory(fullPath.c_str(), NULL);

	if ( !entryList )
	{
		WARNING("Could not scan texture subdirectory %s for texture files.", fullPath.c_str());
		return false;
	}

	uint32_t index = 0;
	for ( DirectoryEntry_t* entry = entryList; entry; entry = entry->next )
	{
		if ( entry->isDirectory || !TextureDirectoryListing::fileNameIsPNG(entry->name) )
		{
			continue;
		}

		std::string textureRelPath;

		if ( path.size() > 0 )
		{
			textureRelPath += path + std::string(SYSTEM_SLASH_STR);
		}

		textureRelPath += fileNameWithoutExtension(entry->name);
		std::string lowercasePath = textureRelPath;
		toLower(lowercasePath);

		if ( caseSensPathIterator(lowercasePath, false) == m_InsensToSensPath.end() )
		{
			Developer(DEVELOPER_LEVEL_SPAM, "Discovered texture: %s\n", textureRelPath.c_str());

			m_TextureToIndex[textureRelPath] = TextureDirectoryListing::INVALID_TEXTURE_INDEX;
			m_InsensToSensPath[lowercasePath] = textureRelPath;
		}
		else
		{
			WARNING("Duplicate texture found: %s. Note that texture paths are case-insensitive.\n", textureRelPath.c_str());
		}
	}

	m_IndexToTexturePath.resize(m_TextureToIndex.size());

	FS_FreeDirectoryEntries(entryList);
	return true;
}

bool TextureDirectoryListing::fileNameIsPNG(const char* path)
{
	std::string extension(FS_FileExtension(path));
	toLower(extension);
	return extension == std::string("png");
}

std::string TextureDirectoryListing::fileNameWithoutExtension(const char* origName)
{
	if ( !origName )
	{
		return std::string();
	}

	const size_t origLength = strlen(origName);
	if ( origLength < 1 )
	{
		return std::string();
	}

	std::vector<char> buffer(origLength);
	FS_FileBase(origName, buffer.data());

	return std::string(buffer.data());
}

std::string TextureDirectoryListing::makeFullTexturePath(const std::string textureRelPath) const
{
	const std::string textureRelPathWithExt = fileNameIsPNG(textureRelPath.c_str())
		? textureRelPath
		: textureRelPath + std::string(".png");

	return m_TextureDirPath +
		   std::string(SYSTEM_SLASH_STR) +
		   makeSystemCanonicalTexturePath(textureRelPathWithExt);
}

std::string TextureDirectoryListing::makeSystemCanonicalTexturePath(const std::string& origPath)
{
	std::vector<char> buffer(origPath.size() + 1);

	memcpy(buffer.data(), origPath.c_str(), origPath.size() + 1);
	FlipSlashes(buffer.data());

	return std::string(buffer.data());
}

TextureDirectoryListing::PathCaseMap::const_iterator TextureDirectoryListing::caseSensPathIterator(const std::string& path, bool convertToLower) const
{
	std::string lowercasePath = path;

	if ( convertToLower )
	{
		toLower(lowercasePath);
	}

	return m_InsensToSensPath.find(lowercasePath);
}
