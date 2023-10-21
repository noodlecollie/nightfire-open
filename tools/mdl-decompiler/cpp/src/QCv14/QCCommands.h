#pragma once

#include <string>

namespace QCv14
{
	struct QCVec3D
	{
		float x = 0;
		float y = 0;
		float z = 0;

		QCVec3D() = default;

		QCVec3D(float inX, float inY, float inZ) :
			x(inX),
			y(inY),
			z(inZ)
		{
		}
	};

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
		QCVec3D position;

		QCAttachment() = default;

		QCAttachment(const std::string& inName, const std::string& inBone, const QCVec3D& inPosition) :
			name(inName),
			bone(inBone),
			position(inPosition)
		{
		}
	};
}  // namespace QCv14
