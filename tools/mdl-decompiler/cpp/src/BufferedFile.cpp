#include "BufferedFile.h"
#include <fstream>
#include <stdexcept>
#include <limits>
#include "Exceptions.h"
#include "BufferedFileReader.h"

std::shared_ptr<BufferedFile> BufferedFile::OpenFile(const std::string& filePath)
{
	struct BufferedFileReader_SharedEnabler : public BufferedFile
	{
		BufferedFileReader_SharedEnabler(const std::string& filePath) :
			BufferedFile(filePath)
		{
		};
	};

	return std::make_shared<BufferedFileReader_SharedEnabler>(filePath);
}

BufferedFile::BufferedFile(const std::string& filePath) :
	m_FilePath(filePath)
{
	std::ifstream inStream(m_FilePath, std::ios_base::in | std::ios_base::binary);

	if ( !inStream.is_open() )
	{
		throw FileIOException(m_FilePath, "Could not open file.");
	}

	inStream.seekg(0, std::ios_base::end);
	const size_t size = inStream.tellg();
	inStream.seekg(0, std::ios_base::beg);

	m_Data.resize(size);
	inStream.read(reinterpret_cast<char*>(m_Data.data()), m_Data.size());

	if ( inStream.bad() )
	{
		throw FileIOException(m_FilePath, "Could not read contents of file.");
	}
}

std::string BufferedFile::FilePath() const
{
	return m_FilePath;
}

const uint8_t* BufferedFile::Data() const
{
	return m_Data.data();
}

size_t BufferedFile::Length() const
{
	return m_Data.size();
}

BufferedFileReader BufferedFile::CreateReader(size_t offset, size_t length) const
{
	if ( length < 1 && offset <= Length() )
	{
		length = Length() - offset;
	}

	return BufferedFileReader(shared_from_this(), offset, length);
}
