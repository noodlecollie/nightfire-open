#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include "MDLv14/AnimationDataHolder.h"

namespace MDLv14
{
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

	enum MotionFlag
	{
		MotionFlag_Invalid = -1,
		MotionFlag_None = 0,
		MotionFlag_X = 0x00000001,
		MotionFlag_Y = 0x00000002,
		MotionFlag_Z = 0x00000004,
		MotionFlag_XR = 0x00000008,
		MotionFlag_YR = 0x00000010,
		MotionFlag_ZR = 0x00000020,
		MotionFlag_LX = 0x00000040,
		MotionFlag_LY = 0x00000080,
		MotionFlag_LZ = 0x00000100,
		MotionFlag_AX = 0x00000200,
		MotionFlag_AY = 0x00000400,
		MotionFlag_AZ = 0x00000800,
		MotionFlag_AXR = 0x00001000,
		MotionFlag_AYR = 0x00002000,
		MotionFlag_AZR = 0x00004000,
	};

	enum SequenceFlag
	{
		SequenceFlag_Looping = 1
	};

	enum NodeFlag
	{
		NodeFlag_Reverse = 1
	};

	struct CountOffsetPair
	{
		int32_t count = 0;
		int32_t offset = 0;
	};

	struct Vec3D
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct BoundingBox
	{
		Vec3D min;
		Vec3D max;
	};

	struct EyePosition
	{
		Vec3D pos;
	};

	struct Header
	{
		std::string identifier;
		int32_t version = 0;
		std::string name;
		int32_t length = 0;
		EyePosition eyePosition;
		BoundingBox boundingBox;
		BoundingBox clippingBox;
		uint32_t typeFlags = 0;
		CountOffsetPair bones;
		CountOffsetPair boneControllers;
		CountOffsetPair hitBoxes;
		CountOffsetPair sequences;
		CountOffsetPair sequenceGroups;
		CountOffsetPair textures;
		int32_t textureInfo = 0;
		int32_t skinReferences = 0;
		int32_t skinFamilies = 0;
		int32_t skinsOffset = 0;
		CountOffsetPair bodyGroups;
		CountOffsetPair attachments;
		CountOffsetPair soundGroups;
		int32_t soundsOffset = 0;
		int32_t transitionsCount = 0;
		int32_t transitionFlagsOffset = 0;
		int32_t transitionsOffset = 0;
		uint32_t levelOfDetailFlags = 0;
		int32_t modelCount = 0;
		int32_t vertexCount = 0;
		int32_t triangleCount = 0;
		int32_t triangleMapOffset = 0;
		int32_t verticesOffset = 0;
		int32_t normalsOffset = 0;
		int32_t textureCoOrdsOffset = 0;
		int32_t unusedOffset = 0;
		int32_t blendingScalesOffset = 0;
		int32_t blendingOffset = 0;
		int32_t boneFixUpOffset = 0;
		std::vector<int32_t> modelOffsets;
	};

	struct Bone
	{
		std::string name;
		int32_t parent = -1;
		uint32_t flags = 0;
		std::vector<int32_t> controllers;
		Vec3D position;
		Vec3D rotation;
		Vec3D scalePosition;
		Vec3D scaleRotation;
	};

	struct BoneController
	{
		int32_t bone = -1;
		int32_t motionFlags = MotionFlag_None;
		float start;
		float end;
		int32_t rest = 0;
		int32_t index = 0;
	};

	struct HitBox
	{
		int32_t bone = -1;
		int32_t group = 0;
		Vec3D min;
		Vec3D max;
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
		Vec3D position;
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
		uint32_t motionType;
		int32_t motionBone;
		Vec3D linearMovement;
		int32_t automovePositionIndex = 0;
		int32_t automoveAngleIndex = 0;
		BoundingBox boundingBox;
		int32_t blendCount = 0;
		int32_t animationOffset = 0;
		uint32_t blendType0 = 0;
		uint32_t blendType1 = 0;
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
		std::vector<Event> eventCollection;
		std::vector<Pivot> pivotCollection;
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
		std::vector<int32_t> distance;
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
		Vec3D position;
		Vec3D vector1;
		Vec3D vector2;
		Vec3D vector3;
	};

	struct Sounds
	{
		int32_t unused;
		std::vector<std::string> soundNames;
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
		Vec3D position;
		float scale = 0.0f;
	};

	struct Normal
	{
		Vec3D position;
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
}  // namespace MDLv14
