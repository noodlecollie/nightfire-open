#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace MDLv14
{
	enum TypeFlag
	{
		Vehicle = 512,
		Water = 2048,
		MultiplayerCharacter = 8192,
		NonPlayableCharacter = 16384
	};

	enum LevelOfDetailFlag
	{
		NoLevelOfDetailFlags = 0,
		TimesOne = 1,
		TimesTwo = 2,
		TimesFive = 4,
		PlusOne = 256,
		PlusTwo = 512,
		PlusFour = 1024
	};

	enum MotionFlag
	{
		InvalidMotionFlags = -1,
		NoMotionFlags = 0,
		X = 0x00000001,
		Y = 0x00000002,
		Z = 0x00000004,
		XR = 0x00000008,
		YR = 0x00000010,
		ZR = 0x00000020,
		LX = 0x00000040,
		LY = 0x00000080,
		LZ = 0x00000100,
		AX = 0x00000200,
		AY = 0x00000400,
		AZ = 0x00000800,
		AXR = 0x00001000,
		AYR = 0x00002000,
		AZR = 0x00004000,
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
		int32_t motionFlags = NoMotionFlags;
		float start;
		float end;
		int32_t rest = 0;
		int32_t index = 0;
	};
}  // namespace MDLv14
