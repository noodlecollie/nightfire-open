#include "pngtexturepath.h"

PNGTexturePath::PNGTexturePath() :
	m_Path()
{
}

void PNGTexturePath::invalidate()
{
	m_Path.clear();
}

std::string PNGTexturePath::path() const
{
	return m_Path;
}

bool PNGTexturePath::setPath(const std::string& path)
{
	if ( path.size() > MAX_PATH_LENGTH )
	{
		return false;
	}

	m_Path = path;
	return true;
}

bool PNGTexturePath::hasValidPath() const
{
	return m_Path.size() > 0 && m_Path.size() <= MAX_PATH_LENGTH;
}

size_t PNGTexturePath::exportDataSize() const
{
	return hasValidPath() ? MAX_TEXTURE_NAME_LENGTH : 0;
}

bool PNGTexturePath::exportData(void* buffer, size_t length) const
{
	const size_t dataSize = exportDataSize();

	if ( !buffer || dataSize < 1 || length < dataSize )
	{
		return false;
	}

	memset(buffer, 0, dataSize);
	strncpy(static_cast<char*>(buffer), m_Path.c_str(), m_Path.size());

	return true;
}
