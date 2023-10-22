#pragma once

#include <string>
#include "Common/Math.h"

namespace QCv14
{
	struct QCBaseCommand
	{
		virtual ~QCBaseCommand() = default;

		virtual bool IsValid() const = 0;
	};

	struct QCModelName : public QCBaseCommand
	{
		std::string name;

		QCModelName() = default;

		QCModelName(const std::string& inName) :
			name(inName)
		{
		}

		bool IsValid() const override
		{
			return !name.empty();
		}
	};

	struct QCAttachment : public QCBaseCommand
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

		bool IsValid() const override
		{
			return !name.empty() && !bone.empty();
		}
	};

	struct QCBBox : public QCBaseCommand
	{
		Vec3D min;
		Vec3D max;

		QCBBox()
		{
			min.SetNAN();
			max.SetNAN();
		}

		QCBBox(const Vec3D& inMin, const Vec3D& inMax) :
			min(inMin),
			max(inMax)
		{
		}

		bool IsValid() const override
		{
			return !min.IsNAN() && !max.IsNAN();
		}
	};
}  // namespace QCv14
