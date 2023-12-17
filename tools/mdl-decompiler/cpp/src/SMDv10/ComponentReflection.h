#pragma once

#include "Traits/Reflection.h"
#include "SMDv10/Components.h"

namespace Reflection
{
	template<>
	struct Traits<SMDv10::Node>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "SMDv10::Node";
	};

	template<>
	struct Traits<SMDv10::NodeFrame>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "SMDv10::NodeFrame";
	};

	template<>
	struct Traits<SMDv10::Frame>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "SMDv10::Frame";
	};

	template<>
	struct Traits<SMDv10::Vertex>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "SMDv10::Vertex";
	};

	template<>
	struct Traits<SMDv10::Triangle>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "SMDv10::Triangle";
	};
}  // namespace Reflection
