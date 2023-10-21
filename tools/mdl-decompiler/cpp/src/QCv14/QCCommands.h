#pragma once

#include <string>

namespace QCv14
{
	struct QCModelName
	{
		std::string name;

		QCModelName() = default;

		QCModelName(const std::string& inName) :
			name(inName)
		{
		}
	};
}  // namespace QCv14
