#pragma once

#include "Traits/Reflection.h"
#include "QCv14/QCCommands.h"

namespace Reflection
{
	template<>
	struct Traits<QCv14::QCModelName>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv14::QCModelName";
	};

	template<>
	struct Traits<QCv14::QCAttachment>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv14::QCAttachment";
	};
}
