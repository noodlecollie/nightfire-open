#pragma once

#include <string>
#include "Common/Math.h"

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

	struct QCAttachment
	{
		std::string name;
		std::string bone;
		Vec3D position;

		QCAttachment() = default;

		QCAttachment(const std::string& inName, const std::string& inBone, const Vec3D& inPosition) :
			name(inName),
			bone(inBone),
			position(inPosition)
		{
		}
	};
}  // namespace QCv14
