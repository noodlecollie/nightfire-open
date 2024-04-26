#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include "Common/Math.h"
#include "Common/Container.h"
#include "Common/Types.h"
#include "MDLv14/AnimationDataHolder.h"

namespace MDLv14
{
#define MDLV14_ACTIVITY_LIST \
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
	LIST_ITEM(ACT_BLINDED, 40 /* 0x00000028 */) \
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
	LIST_ITEM(ACT_FLINCH_RIGHTLEG, 76 /* 0x0000004C */) \
	LIST_ITEM(ACT_FLINCH_RIGHTLEGUPPER, 77 /* 0x0000004D */) \
	LIST_ITEM(ACT_FLINCH_BACK, 78 /* 0x0000004E */) \
	LIST_ITEM(ACT_FLINCH_LEFTARMLOWER, 79 /* 0x0000004F */) \
	LIST_ITEM(ACT_FLINCH_LEFTARMHAND, 80 /* 0x00000050 */) \
	LIST_ITEM(ACT_FLINCH_RIGHTARMLOWER, 81 /* 0x00000051 */) \
	LIST_ITEM(ACT_FLINCH_RIGHTARMHAND, 82 /* 0x00000052 */) \
	LIST_ITEM(ACT_FLINCH_LEFTLEGLOWER, 83 /* 0x00000053 */) \
	LIST_ITEM(ACT_FLINCH_RIGHTLEGLOWER, 84 /* 0x00000054 */) \
	LIST_ITEM(ACT_HOSTAGE, 85 /* 0x00000055 */) \
	LIST_ITEM(ACT_HOSTAGE_THREAT, 86 /* 0x00000056 */) \
	LIST_ITEM(ACT_DIE_NUTSHOT, 89 /* 0x00000059 */)

	enum Activity
	{
#define LIST_ITEM(constant, value) constant = value,
		MDLV14_ACTIVITY_LIST
#undef LIST_ITEM
	};

	enum TypeFlag
	{
		TypeFlag_Vehicle = 512,
		TypeFlag_Water = 2048,
		TypeFlag_MultiplayerCharacter = 8192,
		TypeFlag_NonPlayableCharacter = 16384
	};

	enum LevelOfDetailFlag
	{
		LevelOfDetailFlag_None = 0,
		LevelOfDetailFlag_TimesOne = 1,
		LevelOfDetailFlag_TimesTwo = 2,
		LevelOfDetailFlag_TimesFive = 4,
		LevelOfDetailFlag_PlusOne = 256,
		LevelOfDetailFlag_PlusTwo = 512,
		LevelOfDetailFlag_PlusFour = 1024
	};

	enum SequenceFlag
	{
		SequenceFlag_Looping = 1
	};

	enum NodeFlag
	{
		NodeFlag_Reverse = 1
	};

	struct MDLVec3D : public Vec3D
	{
		MDLVec3D() = default;

		MDLVec3D(float inX, float inY, float inZ) :
			Vec3D(inX, inY, inZ)
		{
		}
	};

	struct CountOffsetPair
	{
		int32_t count = 0;
		int32_t offset = 0;
	};

	struct BoundingBox
	{
		MDLVec3D min;
		MDLVec3D max;
	};

	struct EyePosition
	{
		MDLVec3D pos;
	};

	struct Header
	{
		std::string identifier;  // 0
		int32_t version = 0;  // 4
		std::string name;  // 8
		int32_t length = 0;  // 72
		EyePosition eyePosition;  // 76
		BoundingBox boundingBox;  // 88
		BoundingBox clippingBox;  // 112
		uint32_t typeFlags = 0;  // 136
		CountOffsetPair bones;  // 140
		CountOffsetPair boneControllers;  // 148
		CountOffsetPair hitBoxes;  // 156
		CountOffsetPair sequences;  // 164
		CountOffsetPair sequenceGroups;  // 172
		CountOffsetPair textures;  // 180
		int32_t textureInfo = 0;  // 188
		int32_t skinReferences = 0;  // 192
		int32_t skinFamilies = 0;  // 196
		int32_t skinsOffset = 0;  // 200
		CountOffsetPair bodyGroups;  // 204
		CountOffsetPair attachments;  // 212
		CountOffsetPair soundGroups;  // 220
		int32_t soundsOffset = 0;  // 228
		int32_t transitionsCount = 0;  // 232
		int32_t transitionFlagsOffset = 0;  // 236
		int32_t transitionsOffset = 0;  // 240
		uint32_t levelOfDetailFlags = 0;  // 244
		int32_t modelCount = 0;  // 248
		int32_t vertexCount = 0;  // 252
		int32_t triangleCount = 0;  // 256
		int32_t triangleMapOffset = 0;  // 260
		int32_t verticesOffset = 0;  // 264
		int32_t normalsOffset = 0;  // 268
		int32_t textureCoOrdsOffset = 0;  // 272
		int32_t unusedOffset = 0;  // 276
		int32_t blendingScalesOffset = 0;  // 280
		int32_t blendingOffset = 0;  // 284
		int32_t boneFixUpOffset = 0;  // 288
		Container<int32_t> modelOffsets;  // 292
	};

	struct Bone
	{
		std::string name;
		int32_t parent = -1;
		uint32_t flags = 0;
		Container<int32_t> controllers;
		MDLVec3D position;
		MDLVec3D rotation;
		MDLVec3D scalePosition;
		MDLVec3D scaleRotation;
	};

	struct BoneController
	{
		int32_t bone = -1;
		CommonTypes::MotionFlag motionType = CommonTypes::MotionFlag_None;
		float start;
		float end;
		int32_t rest = 0;
		int32_t index = 0;
	};

	struct HitBox
	{
		int32_t bone = -1;
		int32_t group = 0;
		MDLVec3D min;
		MDLVec3D max;
	};

	struct Event
	{
		int32_t frame = 0;
		int32_t event = 0;
		int32_t type = 0;
		std::string options;
	};

	struct Pivot
	{
		MDLVec3D position;
		int32_t start;
		int32_t end;
	};

	struct Sequence
	{
		std::string name;
		float framesPerSecond = 0.0f;
		uint32_t flags = 0;
		int32_t activity = 0;
		int32_t activityWeight = 0;
		CountOffsetPair events;
		int32_t frameCount = 0;
		CountOffsetPair pivots;
		CommonTypes::MotionFlag motionType = CommonTypes::MotionFlag_Invalid;
		int32_t motionBone;
		MDLVec3D linearMovement;
		int32_t automovePositionIndex = 0;
		int32_t automoveAngleIndex = 0;
		BoundingBox boundingBox;
		int32_t blendCount = 0;
		int32_t animationOffset = 0;
		CommonTypes::MotionFlag blendType0 = CommonTypes::MotionFlag_Invalid;
		CommonTypes::MotionFlag blendType1 = CommonTypes::MotionFlag_Invalid;
		float blendStart0 = 0.0f;
		float blendStart1 = 0.0f;
		float blendEnd0 = 0.0f;
		float blendEnd1 = 0.0f;
		int32_t blendParent = -1;
		int32_t sequenceGroupIndex = 0;
		int32_t nodeEntry = 0;
		int32_t nodeExit = 0;
		uint32_t nodeFlags = 0;
		int32_t unused0 = 0;
		int32_t unused1 = 0;
		int32_t unused2 = 0;
		int32_t unused3 = 0;

		// Deferred reading:
		Container<Event> eventCollection;
		Container<Pivot> pivotCollection;
		AnimationDataHolder animationCollection;
	};

	struct AnimationValue
	{
		struct ValidOfTotal
		{
			uint8_t valid;
			uint8_t total;
		};

		union ValueUnion
		{
			int16_t value;
			ValidOfTotal validOfTotal;
		};

		ValueUnion u {};
	};

	struct SequenceGroup
	{
		std::string label;
		std::string fileName;
		int32_t cache = 0;
		int32_t data = 0;
	};

	struct LevelOfDetail
	{
		int32_t levels = 0;
		Container<int32_t> distance;
	};

	struct Texture
	{
		std::string materialName;
		std::string textureName;
		int32_t referenceCount = 0;
		int32_t unused = 0;
	};

	struct BodyGroup
	{
		std::string name;
		int32_t modelCount = 0;
		int32_t bodyCount = 0;
		int32_t modelOffset = 0;
	};

	struct Attachment
	{
		std::string name;
		int32_t type = 0;
		int32_t bone = -1;
		MDLVec3D position;
		MDLVec3D vector1;
		MDLVec3D vector2;
		MDLVec3D vector3;
	};

	struct Sounds
	{
		int32_t unused;
		Container<std::string> soundNames;
	};

	struct SoundGroup
	{
		std::string name;
		int32_t offset = 0;
		Sounds sounds;
	};

	struct TriangleMap
	{
		uint16_t vertexIndex = 0;
	};

	struct Vertex
	{
		MDLVec3D position;
		float scale = 0.0f;
	};

	struct Normal
	{
		MDLVec3D position;
		float scale = 0.0f;
	};

	struct TextureCoOrdinate
	{
		float u = 0.0f;
		float v = 0.0f;
	};

	struct BlendingScales
	{
		float val0 = 0.0f;
		float val1 = 0.0f;
		float val2 = 0.0f;
		float val3 = 0.0f;
	};

	struct Blending
	{
		int8_t val0 = 0;
		int8_t val1 = 0;
		int8_t val2 = 0;
		int8_t val3 = 0;
	};

	struct BoneFixUp
	{
		float xScale = 0.0f;
		float xSkewY = 0.0f;
		float xSkewZ = 0.0f;
		float xPosition = 0.0f;
		float ySkewX = 0.0f;
		float yScale = 0.0f;
		float ySkewZ = 0.0f;
		float yPosition = 0.0f;
		float zSkewX = 0.0f;
		float zSkewY = 0.0f;
		float zScale = 0.0f;
		float zPosition = 0.0f;
	};

	struct Mesh
	{
		Container<int8_t> boneMappings;
		uint16_t triangleIndex = 0;
		uint16_t triangleCount = 0;
		uint16_t vertexIndexFrom = 0;
		uint16_t vertexIndexTo = 0;
	};

	struct ModelInfo
	{
		int32_t skinReference = 0;
		CountOffsetPair meshes;

		Container<Mesh> meshList;
	};

	struct Model
	{
		std::string name;
		int32_t index = 0;
		Container<int32_t> modelInfoOffsets;

		Container<ModelInfo> modelInfos;
	};
}  // namespace MDLv14
