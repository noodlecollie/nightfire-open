#pragma once

#include "Traits/Reflection.h"
#include "QCv14/QCECommands.h"

namespace Reflection
{
	template<>
	struct Traits<QCv14::QCEGame>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv14::QCEGame";
	};

	template<>
	struct Traits<QCv14::QCEVersion>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv14::QCEVersion";
	};

	template<>
	struct Traits<QCv14::QCEReplaceActivity>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv14::QCEReplaceActivity";
	};
}
