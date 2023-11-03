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

		QCCD() = default;

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

		QCCDTexture() = default;

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

		QCClipToTextures() = default;

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

		QCExternalTextures() = default;

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

		QCRoot() = default;

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

		QCBoneController() = default;

		bool IsValid() const override
		{
			return index >= 0 && !bone.empty();
		}
	};

	struct QCHitBox : public QCBaseCommand
	{
		int32_t group = 0;
		std::string bone;
		Vec3D min;
		Vec3D max;

		QCHitBox()
		{
			min.SetNAN();
			max.SetNAN();
		}

		QCHitBox(int32_t inGroup, const std::string& inBone, const Vec3D& inMin, const Vec3D& inMax) :
			group(inGroup),
			bone(inBone),
			min(inMin),
			max(inMax)
		{
		}

		bool IsValid() const override
		{
			return !bone.empty() && !min.IsNAN() && !max.IsNAN();
		}
	};

	struct QCEyePosition : public QCBaseCommand
	{
		Vec3D position;

		QCEyePosition()
		{
			position.SetNAN();
		}

		QCEyePosition(const Vec3D& inPosition) :
			position(inPosition)
		{
		}

		bool IsValid() const override
		{
			return !position.IsNAN();
		}
	};

	struct QCPivot : public QCBaseCommand
	{
		int32_t index = 0;
		std::string bone;

		QCPivot() = default;

		QCPivot(int32_t inIndex, const std::string& inBone) :
			index(inIndex),
			bone(inBone)
		{
		}

		bool IsValid() const override
		{
			return !bone.empty();
		}
	};

	struct QCMirrorBone : public QCBaseCommand
	{
		std::string bone;

		QCMirrorBone() = default;

		QCMirrorBone(const std::string& inBone) :
			bone(inBone)
		{
		}

		bool IsValid() const override
		{
			return !bone.empty();
		}
	};

	struct QCRenameBone : public QCBaseCommand
	{
		std::string oldName;
		std::string newName;

		QCRenameBone() = default;

		QCRenameBone(const std::string& inOldName, const std::string& inNewName) :
			oldName(inOldName),
			newName(inNewName)
		{
		}

		bool IsValid() const override
		{
			return !oldName.empty() && !newName.empty();
		}
	};

	struct QCGamma : public QCBaseCommand
	{
		float value = 0.0f;

		QCGamma() = default;

		QCGamma(float inValue) :
			value(inValue)
		{
		}

		bool IsValid() const
		{
			return value >= 0.0f;
		}
	};

	struct QCScale : public QCBaseCommand
	{
		float value = 0.0f;

		QCScale() = default;

		QCScale(float inValue) :
			value(inValue)
		{
		}

		bool IsValid() const
		{
			return value != 0.0f;
		}
	};

	struct QCTypeFlags : public QCBaseCommand
	{
		uint32_t flags = 0;

		QCTypeFlags() = default;

		QCTypeFlags(uint32_t inFlags) :
			flags(inFlags)
		{
		}

		bool IsValid() const
		{
			return true;
		}
	};

	struct QCOrigin : public QCBaseCommand
	{
		Vec3D pos;

		QCOrigin()
		{
			pos.SetNAN();
		}

		QCOrigin(const Vec3D& inPos) :
			pos(inPos)
		{
		}

		bool IsValid() const
		{
			return !pos.IsNAN();
		}
	};

	struct QCTextureGroup : public QCBaseCommand
	{
		std::string name;
		Container<Container<std::string>> skins;

		QCTextureGroup() = default;

		QCTextureGroup(const std::string& inName, Container<Container<std::string>>&& inSkins) :
			name(inName),
			skins(std::move(inSkins))
		{
		}

		bool IsValid() const
		{
			return !name.empty();
		}
	};

	struct QCBody : public QCBaseCommand
	{
		std::string name;
		std::string file;

		QCBody() = default;

		QCBody(const std::string inName, const std::string& inFile = std::string()) :
			name(inName),
			file(inFile)
		{
		}

		bool IsValid() const override
		{
			return !name.empty() && !file.empty();
		}
	};

	struct QCSequence : public QCBaseCommand
	{
		std::string name;

		QCSequence() = default;

		bool IsValid() const override
		{
			return !name.empty();
		}
	};
}  // namespace QCv14
