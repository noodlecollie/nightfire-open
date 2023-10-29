#pragma once

#include <string>
#include "Common/Math.h"
#include "Common/Container.h"

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

	struct QCBodyGroupItem : public QCBaseCommand
	{
		std::string name;
		std::string file;

		QCBodyGroupItem() = default;

		QCBodyGroupItem(const std::string inName, const std::string& inFile = std::string()) :
			name(inName),
			file(inFile)
		{
		}

		bool IsValid() const override
		{
			return !name.empty() && !file.empty();
		}
	};

	struct QCBodyGroup : public QCBaseCommand
	{
		std::string name;
		Container<QCBodyGroupItem> bodies;

		QCBodyGroup() = default;

		QCBodyGroup(const std::string& inName, Container<QCBodyGroupItem>&& inBodies) :
			name(inName),
			bodies(std::move(inBodies))
		{
		}

		bool IsValid() const override
		{
			return !name.empty();
		}
	};

	struct QCCBox : public QCBaseCommand
	{
		Vec3D min;
		Vec3D max;

		QCCBox()
		{
			min.SetNAN();
			max.SetNAN();
		}

		QCCBox(const Vec3D& inMin, const Vec3D& inMax) :
			min(inMin),
			max(inMax)
		{
		}

		bool IsValid() const override
		{
			return !min.IsNAN() && !max.IsNAN();
		}
	};

	struct QCCD : public QCBaseCommand
	{
		std::string path;

		QCCD()
		{
		}

		QCCD(const std::string& inPath) :
			path(inPath)
		{
		}

		bool IsValid() const override
		{
			return !path.empty();
		}
	};

	struct QCCDTexture : public QCBaseCommand
	{
		std::string path;

		QCCDTexture()
		{
		}

		QCCDTexture(const std::string& inPath) :
			path(inPath)
		{
		}

		bool IsValid() const override
		{
			return !path.empty();
		}
	};

	struct QCClipToTextures : public QCBaseCommand
	{
		bool enabled = false;

		QCClipToTextures()
		{
		}

		QCClipToTextures(bool inEnabled) :
			enabled(inEnabled)
		{
		}

		bool IsValid() const override
		{
			return enabled;
		}
	};

	struct QCExternalTextures : public QCBaseCommand
	{
		bool enabled = false;

		QCExternalTextures()
		{
		}

		QCExternalTextures(bool inEnabled) :
			enabled(inEnabled)
		{
		}

		bool IsValid() const override
		{
			return enabled;
		}
	};

	struct QCRoot : public QCBaseCommand
	{
		std::string bone;

		QCRoot()
		{
		}

		QCRoot(const std::string& inBone) :
			bone(inBone)
		{
		}

		bool IsValid() const override
		{
			return !bone.empty();
		}
	};

	struct QCBoneController : public QCBaseCommand
	{
		int32_t index = -1;
		std::string bone;
		uint32_t motionFlags = 0;
		float start = 0;
		float end = 0;

		QCBoneController()
		{
		}

		bool IsValid() const override
		{
			return index >= 0 && !bone.empty();
		}
	};
}  // namespace QCv14
