#include "BufferedFileReader.h"
#include <fstream>
#include <stdexcept>
#include <limits>
#include "Exceptions.h"

BufferedFileReader::BufferedFileReader(const std::string& filePath) :
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

size_t BufferedFileReader::CurrentPosition() const
{
	return m_CurrentPos;
}

bool BufferedFileReader::PositionIsEOF() const
{
	return m_CurrentPos >= m_Data.size();
}

void BufferedFileReader::Seek(int64_t delta)
{
	if ( delta > 0 )
	{
		SeekForward(static_cast<size_t>(delta));
	}
	else if ( delta < 0 )
	{
		SeekBackward(static_cast<size_t>(-delta));
	}
}

void BufferedFileReader::SeekForward(size_t delta)
{
	if ( DeltaWouldExceedFile(delta) )
	{
		throw FileIOException(
			m_FilePath,
			"Forward seek of (" + std::to_string(m_CurrentPos) + " + " + std::to_string(delta) +
				") bytes exceeded file length of " + std::to_string(m_Data.size()) + " bytes.");
	}

	SeekFromBeginning(m_CurrentPos + delta);
}

void BufferedFileReader::SeekBackward(size_t delta)
{
	if ( delta > m_CurrentPos )
	{
		throw FileIOException(
			m_FilePath,
			"Backward seek of (" + std::to_string(m_CurrentPos) + " - " + std::to_string(delta) +
				") bytes seeked back past beginning of file.");
	}

	SeekFromBeginning(m_CurrentPos - delta);
}

void BufferedFileReader::SeekFromBeginning(size_t offset)
{
	if ( offset > m_Data.size() )
	{
		throw FileIOException(
			m_FilePath,
			"Seek to offset of " + std::to_string(offset) + " bytes exceeded file length of " +
				std::to_string(m_Data.size()) + " bytes.");
	}

	m_CurrentPos = offset;
}

void BufferedFileReader::SeekFromEnd(size_t offset)
{
	if ( offset > m_Data.size() )
	{
		throw FileIOException(
			m_FilePath,
			"Reverse seek to offset of (" + std::to_string(m_Data.size()) + " - " + std::to_string(offset) +
				") bytes seeked back past beginning of file.");
	}

	SeekFromBeginning(m_Data.size() - offset);
}

void BufferedFileReader::GoToBeginning()
{
	SeekFromBeginning(0);
}

void BufferedFileReader::GoToEnd()
{
	SeekFromEnd(0);
}

void BufferedFileReader::ReadBytes(void* buffer, size_t length)
{
	if ( !buffer )
	{
		throw FileIOException(m_FilePath, "Cannot read bytes into null buffer.");
	}

	if ( length < 1 )
	{
		throw FileIOException(m_FilePath, "Cannot read zero bytes into buffer.");
	}

	if ( DeltaWouldExceedFile(length) )
	{
		throw FileIOException(
			m_FilePath,
			"Reading " + std::to_string(length) + " bytes from offset of " + std::to_string(m_CurrentPos) +
				" exceeded file length of " + std::to_string(m_Data.size()) + " bytes.");
	}

	std::memcpy(buffer, m_Data.data() + m_CurrentPos, length);
}

std::string BufferedFileReader::ReadString(size_t numInputBytes)
{
	if ( numInputBytes < 1 )
	{
		return std::string();
	}

	std::vector<char> rawChars = ReadElements<char>(numInputBytes);

	if ( rawChars.empty() )
	{
		return std::string();
	}

	// Ensure the array is terminated.
	if ( rawChars.at(rawChars.size() - 1) != '\0' )
	{
		rawChars.emplace_back('\0');
	}

	return std::string(rawChars.data());
}

bool BufferedFileReader::DeltaWouldExceedFile(size_t delta) const
{
	// Done carefully to avoid the possibility of overflowing the size_t:
	return (m_Data.size() - m_CurrentPos) < delta;
}
