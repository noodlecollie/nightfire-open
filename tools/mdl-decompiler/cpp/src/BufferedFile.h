#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <functional>
#include "BufferedFileReader.h"

class BufferedFile : public std::enable_shared_from_this<BufferedFile>
{
public:
	static std::shared_ptr<BufferedFile> OpenFile(const std::string& filePath);

	std::string FilePath() const;
	const uint8_t* Data() const;
	size_t Length() const;

	BufferedFileReader CreateReader(size_t offset = 0, size_t length = 0) const;

private:
	explicit BufferedFile(const std::string& filePath);

	std::string m_FilePath;
	std::vector<uint8_t> m_Data;
};
