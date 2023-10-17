#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace MDLv14
{
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
		int32_t typeFlags = 0;  // TODO: Proper type
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
		int32_t levelOfDetailFlags = 0;  // TODO: Proper type
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
}  // namespace MDLv14
