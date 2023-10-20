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
	static constexpr size_t BONECONTROLLER_READ_SIZE = 6 * 4;
	static constexpr size_t HITBOX_READ_SIZE = (2 * 4) + (6 * 4);
	static constexpr size_t EVENT_READ_SIZE = (3 * 4) + 64;
	static constexpr size_t PIVOT_READ_SIZE = 5 * 4;
	static constexpr size_t SEQUENCEGROUP_READ_SIZE = 32 + 64 + (2 * 4);
	static constexpr size_t LEVELOFDETAIL_READ_SIZE = 5 * 4;
	static constexpr size_t TEXTURE_READ_SIZE = (2 * 64) + (2 * 4);
	static constexpr size_t BODYGROUP_READ_SIZE = 64 + (3 * 4);

	static constexpr size_t SEQUENCE_READ_SIZE =  //
		32 +  // strings
		(5 * 4) +  // floats
		(21 * 4) +  // ints
		(4 * 4) +  // count-offset pairs
		(9 * 4);  // Vec3Ds

	static constexpr size_t HEADER_READ_SIZE =  //
		68 +  // strings
		(23 * 4) +  // ints
		(9 * 2 * 4) +  // count-offset pairs
		(15 * 4) +  // Vec3Ds
		(48 * 4);  // model offsets

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

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, BoneController& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(BONECONTROLLER_READ_SIZE);

		component.bone = subReader.ReadElement<int32_t>();
		component.motionFlags = subReader.ReadElement<int32_t>();
		component.start = subReader.ReadElement<float>();
		component.end = subReader.ReadElement<float>();
		component.rest = subReader.ReadElement<int32_t>();
		component.index = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, HitBox& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(HITBOX_READ_SIZE);

		component.bone = subReader.ReadElement<int32_t>();
		component.group = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.min);
		ReadNestedComponent(subReader, component.max);

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Event& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(EVENT_READ_SIZE);

		component.frame = subReader.ReadElement<int32_t>();
		component.event = subReader.ReadElement<int32_t>();
		component.type = subReader.ReadElement<int32_t>();
		component.options = subReader.ReadString(64);

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Pivot& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(PIVOT_READ_SIZE);

		ReadNestedComponent(subReader, component.position);
		component.start = subReader.ReadElement<int32_t>();
		component.end = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Sequence& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(SEQUENCE_READ_SIZE);

		component.name = subReader.ReadString(32);
		component.framesPerSecond = subReader.ReadElement<float>();
		component.flags = subReader.ReadElement<uint32_t>();
		component.activity = subReader.ReadElement<int32_t>();
		component.activityWeight = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.events);
		component.frameCount = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.pivots);
		component.motionType = subReader.ReadElement<uint32_t>();
		component.motionBone = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.linearMovement);
		component.automovePositionIndex = subReader.ReadElement<int32_t>();
		component.automoveAngleIndex = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.boundingBox);
		component.blendCount = subReader.ReadElement<int32_t>();
		component.animationOffset = subReader.ReadElement<int32_t>();
		component.blendType0 = subReader.ReadElement<uint32_t>();
		component.blendType1 = subReader.ReadElement<uint32_t>();
		component.blendStart0 = subReader.ReadElement<float>();
		component.blendStart1 = subReader.ReadElement<float>();
		component.blendEnd0 = subReader.ReadElement<float>();
		component.blendEnd1 = subReader.ReadElement<float>();
		component.blendParent = subReader.ReadElement<int32_t>();
		component.sequenceGroupIndex = subReader.ReadElement<int32_t>();
		component.nodeEntry = subReader.ReadElement<int32_t>();
		component.nodeExit = subReader.ReadElement<int32_t>();
		component.nodeFlags = subReader.ReadElement<uint32_t>();
		component.unused0 = subReader.ReadElement<int32_t>();
		component.unused1 = subReader.ReadElement<int32_t>();
		component.unused2 = subReader.ReadElement<int32_t>();
		component.unused3 = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, SequenceGroup& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(SEQUENCEGROUP_READ_SIZE);

		component.label = subReader.ReadString(32);
		component.fileName = subReader.ReadString(64);
		component.cache = subReader.ReadElement<int32_t>();
		component.data = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, LevelOfDetail& component)
	{
		static constexpr size_t NUM_DISTANCES = 4;

		BufferedFileReader subReader = ref.CreateSubReader(LEVELOFDETAIL_READ_SIZE);

		component.levels = subReader.ReadElement<int32_t>();
		component.distance = subReader.ReadElements<int32_t>(NUM_DISTANCES);

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, Texture& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(TEXTURE_READ_SIZE);

		component.materialName = subReader.ReadString(64);
		component.textureName = subReader.ReadString(64);
		component.referenceCount = subReader.ReadElement<int32_t>();
		component.unused = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, BodyGroup& component)
	{
		BufferedFileReader subReader = ref.CreateSubReader(BODYGROUP_READ_SIZE);

		component.name = subReader.ReadString(64);
		component.modelCount = subReader.ReadElement<int32_t>();
		component.bodyCount = subReader.ReadElement<int32_t>();
		component.modelOffset = subReader.ReadElement<int32_t>();

		return subReader;
	}

	BufferedFileReader
	ComponentReader::ReadInternal(BufferedFileReader::Ref ref, AnimationDataHolder& component, int32_t frameCount)
	{
		BufferedFileReader subReader = ref.CreateSubReader();
		component.Clear();

		for ( size_t blendIndex = 0; blendIndex < component.BlendCount(); ++blendIndex )
		{
			for ( size_t boneIndex = 0; boneIndex < component.BoneCount(); ++boneIndex )
			{
				size_t rowOffset = subReader.CurrentPosition();

				for ( size_t axisIndex = 0; axisIndex < AnimationDataHolder::AXIS_COUNT; ++axisIndex )
				{
					uint16_t offsetIntoRow = subReader.ReadElement<uint16_t>();

					if ( offsetIntoRow > 0 )
					{
						size_t columnOffset = subReader.CurrentPosition();
						subReader.SeekFromBeginning(rowOffset + offsetIntoRow);

						int32_t numFramesHandled = 0;

						while ( numFramesHandled < frameCount )
						{
							AnimationValue frameRange {};
							frameRange.u.value = subReader.ReadElement<int16_t>();

							const uint8_t& valid = frameRange.u.validOfTotal.valid;
							const uint8_t& total = frameRange.u.validOfTotal.total;

							numFramesHandled += total;

							int16_t valueToAdd = 0;

							for ( uint8_t frameIndex = 0; frameIndex < valid; ++frameIndex )
							{
								valueToAdd = subReader.ReadElement<int16_t>();
								component.GetDataArray(blendIndex, boneIndex, axisIndex).emplace_back(valueToAdd);
							}

							for ( uint8_t frameIndex = valid; frameIndex < total; ++frameIndex )
							{
								component.GetDataArray(blendIndex, boneIndex, axisIndex).emplace_back(valueToAdd);
							}
						}

						subReader.SeekFromBeginning(columnOffset);
					}
				}
			}
		}

		return subReader;
	}

	BufferedFileReader ComponentReader::ReadInternal(BufferedFileReader::Ref ref, SkinDataHolder& component)
	{
		BufferedFileReader subReader =
			ref.CreateSubReader(component.SkinFamilyCount() * component.SkinReferenceCount() * 2);

		component.Clear();

		for ( size_t skinFamilyIndex = 0; skinFamilyIndex < component.SkinFamilyCount(); ++skinFamilyIndex )
		{
			for ( size_t skinRefIndex = 0; skinRefIndex < component.SkinReferenceCount(); ++skinRefIndex )
			{
				component.GetEntry(skinFamilyIndex, skinRefIndex) = subReader.ReadElement<int16_t>();
			}
		}

		return subReader;
	}
}  // namespace MDLv14
