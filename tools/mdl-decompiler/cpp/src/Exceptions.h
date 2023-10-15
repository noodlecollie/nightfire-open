#pragma once

#include <string>
#include <stdexcept>

class BaseException : public std::exception
{
public:
	const char* what() const noexcept override
	{
		if ( !m_BuiltErrorString )
		{
			m_ErrorString = BuildErrorString();
			m_BuiltErrorString = true;
		}

		return m_ErrorString.c_str();
	}

protected:
	virtual std::string BuildErrorString() const noexcept = 0;

private:
	mutable std::string m_ErrorString;
	mutable bool m_BuiltErrorString = false;
};

class FileIOException : public BaseException
{
public:
	FileIOException(std::string filePath, std::string description) :
		m_FilePath(std::move(filePath)),
		m_Description(std::move(description))
	{
	}

	std::string FilePath() const
	{
		return m_FilePath;
	}

	std::string Description() const
	{
		return m_Description;
	}

protected:
	std::string BuildErrorString() const noexcept override
	{
		return m_FilePath + ": " + m_Description;
	}

private:
	std::string m_FilePath;
	std::string m_Description;
};

class DataReadException : public FileIOException
{
public:
	DataReadException(std::string filePath, std::string description) :
		FileIOException(filePath, description)
	{
	}
};
