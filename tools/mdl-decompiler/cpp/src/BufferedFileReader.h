#pragma once

#include <memory>
#include <type_traits>
#include <vector>
#include <functional>

class BufferedFile;

class BufferedFileReader
{
public:
	class Ref
	{
	public:
		explicit Ref(BufferedFileReader& parent) :
			m_ParentReader(parent)
		{
		}

		BufferedFileReader CreateSubReader(size_t offset, size_t length)
		{
			return m_ParentReader.CreateSubReader(offset, length);
		}

		BufferedFileReader CreateSubReader(size_t length = 0)
		{
			return m_ParentReader.CreateSubReader(length);
		}

	private:
		BufferedFileReader& m_ParentReader;
	};

	BufferedFileReader(const std::shared_ptr<const BufferedFile>& bufferedFile, size_t offset, size_t length);
	~BufferedFileReader();

	BufferedFileReader(BufferedFileReader&& other);
	BufferedFileReader& operator=(BufferedFileReader&& other);

	BufferedFileReader CreateSubReader(size_t offset, size_t length);
	BufferedFileReader CreateSubReader(size_t length);

	std::string FilePath() const;
	const uint8_t* Data() const;
	size_t Length() const;

	// Utility for reading: ensure that the cursor is at the end
	// of the block of data, indicating that all data in the
	// reader was read.
	void EnsureAtEnd() const;

	size_t BaseInFile() const;
	size_t CurrentPosition() const;
	bool PositionIsEOF() const;
	void Seek(int64_t delta);
	void SeekForward(size_t delta);
	void SeekBackward(size_t delta);
	void SeekFromBeginning(size_t offset);
	void SeekFromEnd(size_t offset);
	void GoToBeginning();
	void GoToEnd();

	void ReadBytes(void* buffer, size_t length);
	std::string ReadString(size_t numInputBytes);
	std::vector<std::string> ReadStrings(size_t numStrings, size_t numBytesPerString);

	template<typename TargetType, typename ContainerType, typename ConverterFunctor>
	TargetType ReadAndConvertElement(const ConverterFunctor& converter)
	{
		static_assert(
			std::is_trivial<ContainerType>::value,
			"Container type must be trivial for a safe memcpy to take place.");

		ContainerType container;
		ReadBytes(&container, sizeof(container));

		return converter(container);
	}

	template<typename TargetType, typename ContainerType>
	TargetType ReadElement()
	{
		return ReadAndConvertElement<TargetType, ContainerType, std::function<TargetType(const ContainerType&)>>(
			[](const ContainerType& in) -> TargetType
			{
				return TargetType(in);
			});
	}

	template<typename TargetType, typename ContainerType, typename ConverterFunctor>
	std::vector<TargetType> ReadAndConvertElements(size_t numElements, const ConverterFunctor& converter)
	{
		static_assert(
			std::is_trivial<ContainerType>::value,
			"Container type must be trivial for a safe memcpy to take place.");

		if ( numElements < 1 )
		{
			return std::vector<TargetType>();
		}

		std::vector<ContainerType> intermediateElements(numElements);
		ReadBytes(intermediateElements.data(), intermediateElements.size() * sizeof(ContainerType));

		std::vector<TargetType> outVector;
		outVector.reserve(intermediateElements.size());

		for ( size_t index = 0; index < intermediateElements.size(); ++index )
		{
			outVector.emplace_back(converter(index, intermediateElements[index]));
		}

		return outVector;
	}

	template<typename TargetType, typename ContainerType>
	std::vector<TargetType> ReadElements(size_t numElements)
	{
		return ReadAndConvertElements<
			TargetType,
			ContainerType,
			std::function<TargetType(size_t, const ContainerType&)>>(
			[](size_t, const ContainerType& in) -> TargetType
			{
				return TargetType(in);
			});
	}

	// Common use specialisations:

	template<typename TargetType>
	TargetType ReadElement()
	{
		static_assert(
			std::is_trivial<TargetType>::value,
			"Target type must be trivial for a safe memcpy to take place.");

		TargetType out;
		ReadBytes(&out, sizeof(out));

		return out;
	}

	template<typename TargetType>
	std::vector<TargetType> ReadElements(size_t numElements)
	{
		static_assert(
			std::is_trivial<TargetType>::value,
			"Target type must be trivial for a safe memcpy to take place.");

		if ( numElements < 1 )
		{
			return std::vector<TargetType>();
		}

		std::vector<TargetType> outVector(numElements);
		ReadBytes(outVector.data(), outVector.size() * sizeof(TargetType));

		return outVector;
	}

private:
	bool DeltaWouldExceedFile(size_t delta) const;

	std::shared_ptr<const BufferedFile> m_File;
	size_t m_Base = 0;
	size_t m_Length = 0;
	size_t m_OffsetFromBase = 0;
};
