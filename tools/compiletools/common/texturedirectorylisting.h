#ifndef TEXTUREDIRECTORYLISTING_H
#define TEXTUREDIRECTORYLISTING_H

#include <string>
#include <map>
#include <unordered_map>
#include <cstdint>
#include <vector>

class TextureDirectoryListing
{
public:
	static constexpr int32_t INVALID_TEXTURE_INDEX = -1;

	TextureDirectoryListing();

	std::string textureDirPath();
	void setTextureDirPath(const std::string& path);

	bool makeListing();
	uint32_t texturePathsSearched() const;

	// For any functions that take a texture path as input,
	// the path is treated as case-insensitive.
	bool containsTexture(const std::string& textureRelPath) const;
	bool textureIsReferenced(const std::string& textureRelPath) const;

	// Returns INVALID_TEXTURE_INDEX if the texture doesn't exist or is not referenced.
	int32_t textureIndex(const std::string& textureRelPath) const;

	// Returns the assigned index, or INVALID_TEXTURE_INDEX if the texture did not exist.
	int32_t assignTextureIndex(const std::string& textureRelPath);

	std::string texturePath(const uint32_t index) const;

	void textureList(std::vector<std::string>& list) const;
	void referencedTextureList(std::vector<std::string>& list) const;
	size_t count() const;
	void clear();

	// The input path here is not case-insensitive.
	std::string makeFullTexturePath(const std::string textureRelPath) const;

private:
	typedef std::map<std::string, int32_t> TextureIndexMap;
	typedef std::unordered_map<std::string, std::string> PathCaseMap;

	static std::string makeSystemCanonicalTexturePath(const std::string& origPath);
	static bool fileNameIsPNG(const char* path);
	static std::string fileNameWithoutExtension(const char* origName);

	bool readTexturesFromDirectory(const std::string& path);
	PathCaseMap::const_iterator caseSensPathIterator(const std::string& path, bool convertToLower = true) const;
	bool getTextureDirectories(std::vector<std::string>& dirPaths) const;

	std::string m_TextureDirPath;
	TextureIndexMap m_TextureToIndex;
	PathCaseMap m_InsensToSensPath;
	int32_t m_NextTextureIndex;
	uint32_t m_NumTexturePathsSearched;
	std::vector<std::string> m_IndexToTexturePath;
};

#endif // TEXTUREDIRECTORYLISTING_H
