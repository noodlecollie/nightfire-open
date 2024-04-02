#pragma once

#include <string>
#include "Common/Math.h"
#include "Common/Container.h"
#include "Common/Types.h"

namespace QCv10
{
#define QCV10_ACTIVITY_LIST \
	LIST_ITEM(ACT_INVALID, -1 /* 0xFFFFFFFF */) \
	LIST_ITEM(ACT_RESET, 0) \
	LIST_ITEM(ACT_IDLE, 1) \
	LIST_ITEM(ACT_GUARD, 2) \
	LIST_ITEM(ACT_WALK, 3) \
	LIST_ITEM(ACT_RUN, 4) \
	LIST_ITEM(ACT_FLY, 5) \
	LIST_ITEM(ACT_SWIM, 6) \
	LIST_ITEM(ACT_HOP, 7) \
	LIST_ITEM(ACT_LEAP, 8) \
	LIST_ITEM(ACT_FALL, 9) \
	LIST_ITEM(ACT_LAND, 10 /* 0x0000000A */) \
	LIST_ITEM(ACT_STRAFE_LEFT, 11 /* 0x0000000B */) \
	LIST_ITEM(ACT_STRAFE_RIGHT, 12 /* 0x0000000C */) \
	LIST_ITEM(ACT_ROLL_LEFT, 13 /* 0x0000000D */) \
	LIST_ITEM(ACT_ROLL_RIGHT, 14 /* 0x0000000E */) \
	LIST_ITEM(ACT_TURN_LEFT, 15 /* 0x0000000F */) \
	LIST_ITEM(ACT_TURN_RIGHT, 16 /* 0x00000010 */) \
	LIST_ITEM(ACT_CROUCH, 17 /* 0x00000011 */) \
	LIST_ITEM(ACT_CROUCHIDLE, 18 /* 0x00000012 */) \
	LIST_ITEM(ACT_STAND, 19 /* 0x00000013 */) \
	LIST_ITEM(ACT_USE, 20 /* 0x00000014 */) \
	LIST_ITEM(ACT_SIGNAL1, 21 /* 0x00000015 */) \
	LIST_ITEM(ACT_SIGNAL2, 22 /* 0x00000016 */) \
	LIST_ITEM(ACT_SIGNAL3, 23 /* 0x00000017 */) \
	LIST_ITEM(ACT_TWITCH, 24 /* 0x00000018 */) \
	LIST_ITEM(ACT_COWER, 25 /* 0x00000019 */) \
	LIST_ITEM(ACT_SMALL_FLINCH, 26 /* 0x0000001A */) \
	LIST_ITEM(ACT_BIG_FLINCH, 27 /* 0x0000001B */) \
	LIST_ITEM(ACT_RANGE_ATTACK1, 28 /* 0x0000001C */) \
	LIST_ITEM(ACT_RANGE_ATTACK2, 29 /* 0x0000001D */) \
	LIST_ITEM(ACT_MELEE_ATTACK1, 30 /* 0x0000001E */) \
	LIST_ITEM(ACT_MELEE_ATTACK2, 31 /* 0x0000001F */) \
	LIST_ITEM(ACT_RELOAD, 32 /* 0x00000020 */) \
	LIST_ITEM(ACT_ARM, 33 /* 0x00000021 */) \
	LIST_ITEM(ACT_DISARM, 34 /* 0x00000022 */) \
	LIST_ITEM(ACT_EAT, 35 /* 0x00000023 */) \
	LIST_ITEM(ACT_DIESIMPLE, 36 /* 0x00000024 */) \
	LIST_ITEM(ACT_DIEBACKWARD, 37 /* 0x00000025 */) \
	LIST_ITEM(ACT_DIEFORWARD, 38 /* 0x00000026 */) \
	LIST_ITEM(ACT_DIEVIOLENT, 39 /* 0x00000027 */) \
	LIST_ITEM(ACT_BARNACLE_HIT, 40 /* 0x00000028 */) \
	LIST_ITEM(ACT_BARNACLE_PULL, 41 /* 0x00000029 */) \
	LIST_ITEM(ACT_BARNACLE_CHOMP, 42 /* 0x0000002A */) \
	LIST_ITEM(ACT_BARNACLE_CHEW, 43 /* 0x0000002B */) \
	LIST_ITEM(ACT_SLEEP, 44 /* 0x0000002C */) \
	LIST_ITEM(ACT_INSPECT_FLOOR, 45 /* 0x0000002D */) \
	LIST_ITEM(ACT_INSPECT_WALL, 46 /* 0x0000002E */) \
	LIST_ITEM(ACT_IDLE_ANGRY, 47 /* 0x0000002F */) \
	LIST_ITEM(ACT_WALK_HURT, 48 /* 0x00000030 */) \
	LIST_ITEM(ACT_RUN_HURT, 49 /* 0x00000031 */) \
	LIST_ITEM(ACT_HOVER, 50 /* 0x00000032 */) \
	LIST_ITEM(ACT_GLIDE, 51 /* 0x00000033 */) \
	LIST_ITEM(ACT_FLY_LEFT, 52 /* 0x00000034 */) \
	LIST_ITEM(ACT_FLY_RIGHT, 53 /* 0x00000035 */) \
	LIST_ITEM(ACT_DETECT_SCENT, 54 /* 0x00000036 */) \
	LIST_ITEM(ACT_SNIFF, 55 /* 0x00000037 */) \
	LIST_ITEM(ACT_BITE, 56 /* 0x00000038 */) \
	LIST_ITEM(ACT_THREAT_DISPLAY, 57 /* 0x00000039 */) \
	LIST_ITEM(ACT_FEAR_DISPLAY, 58 /* 0x0000003A */) \
	LIST_ITEM(ACT_EXCITED, 59 /* 0x0000003B */) \
	LIST_ITEM(ACT_SPECIAL_ATTACK1, 60 /* 0x0000003C */) \
	LIST_ITEM(ACT_SPECIAL_ATTACK2, 61 /* 0x0000003D */) \
	LIST_ITEM(ACT_COMBAT_IDLE, 62 /* 0x0000003E */) \
	LIST_ITEM(ACT_WALK_SCARED, 63 /* 0x0000003F */) \
	LIST_ITEM(ACT_RUN_SCARED, 64 /* 0x00000040 */) \
	LIST_ITEM(ACT_VICTORY_DANCE, 65 /* 0x00000041 */) \
	LIST_ITEM(ACT_DIE_HEADSHOT, 66 /* 0x00000042 */) \
	LIST_ITEM(ACT_DIE_CHESTSHOT, 67 /* 0x00000043 */) \
	LIST_ITEM(ACT_DIE_GUTSHOT, 68 /* 0x00000044 */) \
	LIST_ITEM(ACT_DIE_BACKSHOT, 69 /* 0x00000045 */) \
	LIST_ITEM(ACT_FLINCH_HEAD, 70 /* 0x00000046 */) \
	LIST_ITEM(ACT_FLINCH_CHEST, 71 /* 0x00000047 */) \
	LIST_ITEM(ACT_FLINCH_STOMACH, 72 /* 0x00000048 */) \
	LIST_ITEM(ACT_FLINCH_LEFTARM, 73 /* 0x00000049 */) \
	LIST_ITEM(ACT_FLINCH_RIGHTARM, 74 /* 0x0000004A */) \
	LIST_ITEM(ACT_FLINCH_LEFTLEG, 75 /* 0x0000004B */) \
	LIST_ITEM(ACT_FLINCH_RIGHTLEG, 76 /* 0x0000004C */)

	enum Activity
	{
#define LIST_ITEM(constant, value) constant = value,
		QCV10_ACTIVITY_LIST
#undef LIST_ITEM
	};

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
		CommonTypes::MotionFlag motionType = CommonTypes::MotionFlag_None;
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

	struct QCOptionActivity : public QCBaseCommand
	{
		Activity activity = ACT_INVALID;
		float weight = 0.0f;

		QCOptionActivity() = default;

		QCOptionActivity(Activity inActivity, float inWeight) :
			activity(inActivity),
			weight(inWeight)
		{
		}

		bool IsValid() const override
		{
			return activity != ACT_INVALID;
		}
	};

	struct QCOptionBlend : public QCBaseCommand
	{
		CommonTypes::MotionFlag motionFlags = CommonTypes::MotionFlag_Invalid;
		float start = 0.0f;
		float end = 0.0f;

		QCOptionBlend() = default;

		QCOptionBlend(CommonTypes::MotionFlag inMotionFlags, float inStart, float inEnd) :
			motionFlags(inMotionFlags),
			start(inStart),
			end(inEnd)
		{
		}

		bool IsValid() const override
		{
			return motionFlags != CommonTypes::MotionFlag_Invalid;
		}
	};

	struct QCOptionEvent : public QCBaseCommand
	{
		int32_t value = 0;
		int32_t frame = 0;
		std::string options;

		QCOptionEvent() = default;

		QCOptionEvent(int32_t inValue, int32_t inFrame, const std::string& inOptions) :
			value(inValue),
			frame(inFrame),
			options(inOptions)
		{
		}

		bool IsValid() const override
		{
			return value > 0;
		}
	};

	struct QCOptionTransition : public QCBaseCommand
	{
		int32_t sourceBone = -1;
		int32_t targetBone = -1;
		bool reverse = false;

		QCOptionTransition() = default;

		QCOptionTransition(int32_t inSource, int32_t inTarget, bool inReverse = false) :
			sourceBone(inSource),
			targetBone(inTarget),
			reverse(inReverse)
		{
		}

		bool IsValid() const override
		{
			return sourceBone >= 0 && targetBone >= 0;
		}
	};

	struct QCOptionPivot : public QCBaseCommand
	{
		int32_t index = 0;
		float start = 0.0f;
		float end = 0.0f;

		QCOptionPivot() = default;

		QCOptionPivot(int32_t inIndex, float inStart, float inEnd) :
			index(inIndex),
			start(inStart),
			end(inEnd)
		{
		}

		bool IsValid() const override
		{
			return true;
		}
	};

	struct QCSequence : public QCBaseCommand
	{
		std::string name;
		std::vector<std::string> files;
		QCOptionActivity activity;
		Container<QCOptionBlend> blends;
		CommonTypes::MotionFlag controlFlags = CommonTypes::MotionFlag_Invalid;
		Container<QCOptionEvent> events;
		float framesPerSecond = 30.0f;
		bool loop = false;
		int8_t nodeEntryBone = -1;
		QCOptionTransition transition;
		Container<QCOptionPivot> pivots;

		QCSequence() = default;

		bool IsValid() const override
		{
			return !name.empty();
		}
	};
}  // namespace QCv10
