#include "MDLv14/ComponentReader.h"
#include "BufferedFileReader.h"
#include "Utils.h"

namespace MDLv14
{
	static constexpr size_t COUNTOFFSETPAIR_READ_SIZE = 2 * 4;
	static constexpr size_t VEC3D_READ_SIZE = 3 * 4;
	static constexpr size_t EYEPOSITION_READ_SIZE = 3 * 4;
	static constexpr size_t BOUNDINGBOX_READ_SIZE = 2 * VEC3D_READ_SIZE;
	static constexpr size_t BONE_READ_SIZE = 32 + (20 * 4);

	// TODO: Update to make this less opaque
	static constexpr size_t HEADER_READ_SIZE = 484;  // From original code

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, CountOffsetPair& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(COUNTOFFSETPAIR_READ_SIZE);

		component.count = subReader.ReadElement<int32_t>();
		component.offset = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Vec3D& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(VEC3D_READ_SIZE);

		component.x = subReader.ReadElement<float>();
		component.y = subReader.ReadElement<float>();
		component.z = subReader.ReadElement<float>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, BoundingBox& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(BOUNDINGBOX_READ_SIZE);

		ReadNestedComponent(subReader, component.min);
		ReadNestedComponent(subReader, component.max);

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, EyePosition& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(EYEPOSITION_READ_SIZE);

		// Deliberately in Y, X, Z order:
		component.pos.y = subReader.ReadElement<float>();
		component.pos.x = subReader.ReadElement<float>();
		component.pos.z = subReader.ReadElement<float>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Header& component)
	{
		static constexpr size_t NUM_MODEL_OFFSETS = 48;

		BufferedFileReader subReader = ref.CreateSubReader(HEADER_READ_SIZE);

		component.identifier = subReader.ReadString(4);
		component.version = subReader.ReadElement<int32_t>();
		component.name = subReader.ReadString(64);
		component.length = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.eyePosition);
		ReadNestedComponent(subReader, component.boundingBox);
		ReadNestedComponent(subReader, component.clippingBox);
		component.typeFlags = subReader.ReadElement<uint32_t>();
		ReadNestedComponent(subReader, component.bones);
		ReadNestedComponent(subReader, component.boneControllers);
		ReadNestedComponent(subReader, component.hitBoxes);
		ReadNestedComponent(subReader, component.sequences);
		ReadNestedComponent(subReader, component.sequenceGroups);
		ReadNestedComponent(subReader, component.textures);
		component.textureInfo = subReader.ReadElement<int32_t>();
		component.skinReferences = subReader.ReadElement<int32_t>();
		component.skinFamilies = subReader.ReadElement<int32_t>();
		component.skinsOffset = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.bodyGroups);
		ReadNestedComponent(subReader, component.attachments);
		ReadNestedComponent(subReader, component.soundGroups);
		component.soundsOffset = subReader.ReadElement<int32_t>();
		component.transitionsCount = subReader.ReadElement<int32_t>();
		component.transitionFlagsOffset = subReader.ReadElement<int32_t>();
		component.transitionsOffset = subReader.ReadElement<int32_t>();
		component.levelOfDetailFlags = subReader.ReadElement<uint32_t>();
		component.modelCount = subReader.ReadElement<int32_t>();
		component.vertexCount = subReader.ReadElement<int32_t>();
		component.triangleCount = subReader.ReadElement<int32_t>();
		component.triangleMapOffset = subReader.ReadElement<int32_t>();
		component.verticesOffset = subReader.ReadElement<int32_t>();
		component.normalsOffset = subReader.ReadElement<int32_t>();
		component.textureCoOrdsOffset = subReader.ReadElement<int32_t>();
		component.unusedOffset = subReader.ReadElement<int32_t>();
		component.blendingScalesOffset = subReader.ReadElement<int32_t>();
		component.blendingOffset = subReader.ReadElement<int32_t>();
		component.boneFixUpOffset = subReader.ReadElement<int32_t>();
		component.modelOffsets = subReader.ReadElements<int32_t>(NUM_MODEL_OFFSETS);

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Bone& component)
	{
		static constexpr size_t NUM_BONE_CONTROLLERS = 6;

		BufferedFileReader subReader = ref.CreateSubReader(BONE_READ_SIZE);

		component.name = subReader.ReadString(32);
		component.parent = subReader.ReadElement<int32_t>();
		component.flags = subReader.ReadElement<uint32_t>();
		component.controllers = subReader.ReadElements<int32_t>(NUM_BONE_CONTROLLERS);
		ReadNestedComponent(subReader, component.position);
		ReadNestedComponent(subReader, component.rotation);
		ReadNestedComponent(subReader, component.scalePosition);
		ReadNestedComponent(subReader, component.scaleRotation);

		return subReader;
	}
}  // namespace MDLv14
