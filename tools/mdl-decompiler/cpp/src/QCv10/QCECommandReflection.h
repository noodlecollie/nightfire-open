#pragma once

#include "Traits/Reflection.h"
#include "QCv10/QCECommands.h"

namespace Reflection
{
	template<>
	struct Traits<QCv10::QCEGame>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCEGame";
	};

	template<>
	struct Traits<QCv10::QCEVersion>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCEVersion";
	};

	template<>
	struct Traits<QCv10::QCEReplaceActivity>
	{
		static constexpr const char* const QUALIFIED_TYPE_NAME = "QCv10::QCEReplaceActivity";
	};
}
