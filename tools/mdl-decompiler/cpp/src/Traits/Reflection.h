#pragma once

#include <string>

namespace Reflection
{
	template<typename T>
	struct Traits
	{
	};

	template<typename T>
	static inline std::string QualifiedTypeName()
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
}  // namespace Reflection
