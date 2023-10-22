#include <fstream>
#include <stdexcept>
#include <limits>
#include <cstring>
#include "BufferedFileReader.h"
#include "BufferedFile.h"
#include "Exceptions.h"

static void ValidateOffsetAndLength(const std::string& filePath, size_t dataLength, size_t offset, size_t length)
{
	if ( offset > dataLength )
	{
		throw FileIOException(
			filePath,
			"BufferedFileReader base offset of " + std::to_string(offset) + " bytes exceeded buffer length of " +
				std::to_string(dataLength) + " bytes.");
	}

	// Done carefully so as not to overflow size_t:
	if ( (dataLength - offset) < length )
	{
		throw FileIOException(
			filePath,
			"BufferedFileReader file segment with offset of " + std::to_string(offset) + " bytes and length of " +
				std::to_string(length) + " bytes exceeded " + std::to_string(dataLength - offset) +
				" bytes of data in buffer.");
	}
}

BufferedFileReader::BufferedFileReader(
	const std::shared_ptr<const BufferedFile>& bufferedFile,
	size_t offset,
	size_t length)
{
	if ( !bufferedFile )
	{
		throw std::invalid_argument("Cannot create BufferedFileReader from a null BufferedFile.");
	}

	ValidateOffsetAndLength(bufferedFile->FilePath(), bufferedFile->Length(), offset, length);

	m_File = bufferedFile;
	m_Base = offset;
	m_Length = length;
}

BufferedFileReader::~BufferedFileReader()
{
}

BufferedFileReader::BufferedFileReader(BufferedFileReader&& other)
{
	*this = std::move(other);
}

BufferedFileReader& BufferedFileReader::operator=(BufferedFileReader&& other)
{
	m_File = std::move(other.m_File);
	m_Base = std::move(other.m_Base);
	m_OffsetFromBase = std::move(other.m_OffsetFromBase);
	m_Length = std::move(other.m_Length);

	return *this;
}

BufferedFileReader BufferedFileReader::CreateSubReader(size_t offset, size_t length)
{
	if ( length < 1 && offset <= m_Length )
	{
		length = m_Length - offset;
	}

	ValidateOffsetAndLength(m_File->FilePath(), m_Length, offset, length);

	return BufferedFileReader(m_File, m_Base + offset, length);
}

BufferedFileReader BufferedFileReader::CreateSubReader(size_t length)
{
	return CreateSubReader(CurrentPosition(), length);
}

std::string BufferedFileReader::FilePath() const
{
	return m_File->FilePath();
}

const uint8_t* BufferedFileReader::Data() const
{
	return m_File->Data() + m_Base;
}

size_t BufferedFileReader::Length() const
{
	return m_Length;
}

void BufferedFileReader::EnsureAtEnd() const
{
	if ( !PositionIsEOF() )
	{
		throw DataReadException(
			m_File->FilePath(),
			"Input file section (" + std::to_string(m_Base) + "," + std::to_string(m_Length) +
				") was not fully consumed.");
	}
}

size_t BufferedFileReader::BaseInFile() const
{
	return m_Base;
}

size_t BufferedFileReader::CurrentPosition() const
{
	return m_OffsetFromBase;
}

bool BufferedFileReader::PositionIsEOF() const
{
	return CurrentPosition() >= Length();
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
			m_File->FilePath(),
			"Forward seek of (" + std::to_string(CurrentPosition()) + " + " + std::to_string(delta) +
				") bytes exceeded file length of " + std::to_string(Length()) + " bytes.");
	}

	SeekFromBeginning(CurrentPosition() + delta);
}

void BufferedFileReader::SeekBackward(size_t delta)
{
	if ( delta > CurrentPosition() )
	{
		throw FileIOException(
			m_File->FilePath(),
			"Backward seek of (" + std::to_string(CurrentPosition()) + " - " + std::to_string(delta) +
				") bytes seeked back past beginning of file.");
	}

	SeekFromBeginning(CurrentPosition() - delta);
}

void BufferedFileReader::SeekFromBeginning(size_t offset)
{
	if ( offset > Length() )
	{
		throw FileIOException(
			m_File->FilePath(),
			"Seek to offset of " + std::to_string(offset) + " bytes exceeded file length of " +
				std::to_string(Length()) + " bytes.");
	}

	m_OffsetFromBase = offset;
}

void BufferedFileReader::SeekFromEnd(size_t offset)
{
	if ( offset > Length() )
	{
		throw FileIOException(
			m_File->FilePath(),
			"Reverse seek to offset of (" + std::to_string(Length()) + " - " + std::to_string(offset) +
				") bytes seeked back past beginning of file.");
	}

	SeekFromBeginning(Length() - offset);
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
		throw FileIOException(m_File->FilePath(), "Cannot read bytes into null buffer.");
	}

	if ( length < 1 )
	{
		throw FileIOException(m_File->FilePath(), "Cannot read zero bytes into buffer.");
	}

	if ( DeltaWouldExceedFile(length) )
	{
		throw FileIOException(
			m_File->FilePath(),
			"Reading " + std::to_string(length) + " bytes from offset of " + std::to_string(CurrentPosition()) +
				" exceeded data buffer length of " + std::to_string(Length()) + " bytes.");
	}

	std::memcpy(buffer, Data() + CurrentPosition(), length);
	m_OffsetFromBase += length;
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

std::vector<std::string> BufferedFileReader::ReadStrings(size_t numStrings, size_t numBytesPerString)
{
	std::vector<std::string> out;
	out.reserve(numStrings);

	for ( size_t index = 0; index < numStrings; ++index )
	{
		out.emplace_back(ReadString(numBytesPerString));
	}

	return out;
}

bool BufferedFileReader::DeltaWouldExceedFile(size_t delta) const
{
	// Done carefully to avoid the possibility of overflowing the size_t:
	return (Length() - CurrentPosition()) < delta;
}
