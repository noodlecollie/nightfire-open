#pragma once

#include "Traits/Reflection.h"
#include "QCv10/QCCommands.h"

namespace Reflection
{
	template<>
	struct Traits<QCv10::QCModelName>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCModelName";
	};

	template<>
	struct Traits<QCv10::QCAttachment>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCAttachment";
	};
}
