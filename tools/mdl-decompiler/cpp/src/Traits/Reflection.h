#pragma once

#include <string>
#include <cstdint>

namespace Reflection
{
	template<typename T>
	struct Traits
	{
	};

	// Common types:

	template<>
	struct Traits<int8_t>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "int8_t";
		static constexpr size_t READ_SIZE = 1;
	};

	template<>
	struct Traits<int32_t>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "int32_t";
		static constexpr size_t READ_SIZE = 4;
	};

	template<>
	struct Traits<std::string>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "std::string";
	};

	template<typename T>
	static inline constexpr const char* QualifiedTypeName()
	{
		return Traits<T>::QUALIFIED_TYPE_NAME;
	}

	template<typename T>
	static inline std::string TypeName()
	{
		const std::string qualifiedTypeName = QualifiedTypeName<T>();
		const size_t lastSep = qualifiedTypeName.rfind("::");

		return lastSep != std::string::npos ? qualifiedTypeName.substr(lastSep + 2) : qualifiedTypeName;
	}

	template<typename T>
	static inline constexpr size_t ReadSize()
	{
		return Traits<T>::READ_SIZE;
	}
}  // namespace Reflection
