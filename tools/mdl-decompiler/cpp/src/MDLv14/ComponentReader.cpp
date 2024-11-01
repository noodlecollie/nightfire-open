#include "MDLv14/ComponentReader.h"
#include "BufferedFileReader.h"
#include "Conversions/MotionFlags.h"
#include "Traits/Reflection.h"
#include "Utils.h"
#include "Exceptions.h"

namespace MDLv14
{
	void ComponentReader::ReadInternal(BufferedFileReader& subReader, CountOffsetPair& component)
	{
		component.count = subReader.ReadElement<int32_t>();
		component.offset = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Vec3D& component)
	{
		component.x = subReader.ReadElement<float>();
		component.y = subReader.ReadElement<float>();
		component.z = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, BoundingBox& component)
	{
		ReadNestedComponent(subReader, component.min);
		ReadNestedComponent(subReader, component.max);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, EyePosition& component)
	{
		// Deliberately in Y, X, Z order:
		component.pos.y = subReader.ReadElement<float>();
		component.pos.x = subReader.ReadElement<float>();
		component.pos.z = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Header& component)
	{
		static constexpr size_t NUM_MODEL_OFFSETS = 48;

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
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Bone& component)
	{
		static constexpr size_t NUM_BONE_CONTROLLERS = 6;

		component.name = subReader.ReadString(32);
		component.parent = subReader.ReadElement<int32_t>();
		component.flags = subReader.ReadElement<uint32_t>();
		component.controllers = subReader.ReadElements<int32_t>(NUM_BONE_CONTROLLERS);
		ReadNestedComponent(subReader, component.position);
		ReadNestedComponent(subReader, component.rotation);
		ReadNestedComponent(subReader, component.scalePosition);
		ReadNestedComponent(subReader, component.scaleRotation);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, BoneController& component)
	{
		component.bone = subReader.ReadElement<int32_t>();

		int32_t motionType = subReader.ReadElement<int32_t>();

		if ( !Conversion::IsValidMotionFlag(motionType) )
		{
			throw ValidationException(Reflection::TypeName<BoneController>(), "Bone controller has invalid motion type \"" + std::to_string(motionType) + "\".");
		}

		component.motionType = static_cast<CommonTypes::MotionFlag>(motionType);

		component.start = subReader.ReadElement<float>();
		component.end = subReader.ReadElement<float>();
		component.rest = subReader.ReadElement<int32_t>();
		component.index = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, HitBox& component)
	{
		component.bone = subReader.ReadElement<int32_t>();
		component.group = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.min);
		ReadNestedComponent(subReader, component.max);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Event& component)
	{
		component.frame = subReader.ReadElement<int32_t>();
		component.event = subReader.ReadElement<int32_t>();
		component.type = subReader.ReadElement<int32_t>();
		component.options = subReader.ReadString(64);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Pivot& component)
	{
		ReadNestedComponent(subReader, component.position);
		component.start = subReader.ReadElement<int32_t>();
		component.end = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Sequence& component)
	{
		component.name = subReader.ReadString(32);
		component.framesPerSecond = subReader.ReadElement<float>();
		component.flags = subReader.ReadElement<uint32_t>();
		component.activity = subReader.ReadElement<int32_t>();
		component.activityWeight = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.events);
		component.frameCount = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.pivots);

		int32_t motionType = subReader.ReadElement<int32_t>();

		if ( !Conversion::IsValidMotionFlag(motionType) )
		{
			throw ValidationException(Reflection::TypeName<Sequence>(), "Sequence has invalid motion type.");
		}

		component.motionType = static_cast<CommonTypes::MotionFlag>(motionType);

		component.motionBone = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.linearMovement);
		component.automovePositionIndex = subReader.ReadElement<int32_t>();
		component.automoveAngleIndex = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.boundingBox);
		component.blendCount = subReader.ReadElement<int32_t>();
		component.animationOffset = subReader.ReadElement<int32_t>();

		int32_t blendType0 = subReader.ReadElement<uint32_t>();

		if ( !Conversion::IsValidMotionFlag(blendType0) )
		{
			throw ValidationException(Reflection::TypeName<Sequence>(), "Sequence blend 0 has invalid motion type.");
		}

		component.blendType0 = static_cast<CommonTypes::MotionFlag>(blendType0);

		int32_t blendType1 = subReader.ReadElement<uint32_t>();

		if ( !Conversion::IsValidMotionFlag(blendType1) )
		{
			throw ValidationException(Reflection::TypeName<Sequence>(), "Sequence blend 1 has invalid motion type.");
		}

		component.blendType1 = static_cast<CommonTypes::MotionFlag>(blendType1);

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
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, SequenceGroup& component)
	{
		component.label = subReader.ReadString(32);
		component.fileName = subReader.ReadString(64);
		component.cache = subReader.ReadElement<int32_t>();
		component.data = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, LevelOfDetail& component)
	{
		static constexpr size_t NUM_DISTANCES = 4;

		component.levels = subReader.ReadElement<int32_t>();
		component.distance = subReader.ReadElements<int32_t>(NUM_DISTANCES);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Texture& component)
	{
		component.materialName = subReader.ReadString(64);
		component.textureName = subReader.ReadString(64);
		component.referenceCount = subReader.ReadElement<int32_t>();
		component.unused = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, BodyGroup& component)
	{
		component.name = subReader.ReadString(64);
		component.modelCount = subReader.ReadElement<int32_t>();
		component.bodyCount = subReader.ReadElement<int32_t>();
		component.modelOffset = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Attachment& component)
	{
		component.name = subReader.ReadString(32);
		component.type = subReader.ReadElement<int32_t>();
		component.bone = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.position);
		ReadNestedComponent(subReader, component.vector1);
		ReadNestedComponent(subReader, component.vector2);
		ReadNestedComponent(subReader, component.vector3);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, SoundGroup& component)
	{
		component.name = subReader.ReadString(32);
		component.offset = subReader.ReadElement<int32_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Sounds& component)
	{
		static constexpr size_t NUM_NAMES = 10;

		component.unused = subReader.ReadElement<int32_t>();
		component.soundNames = subReader.ReadStrings(NUM_NAMES, 64);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, TriangleMap& component)
	{
		component.vertexIndex = subReader.ReadElement<uint16_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Vertex& component)
	{
		ReadNestedComponent(subReader, component.position);
		component.scale = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Normal& component)
	{
		ReadNestedComponent(subReader, component.position);
		component.scale = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, TextureCoOrdinate& component)
	{
		component.u = subReader.ReadElement<float>();
		component.v = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, BlendingScales& component)
	{
		component.val0 = subReader.ReadElement<float>();
		component.val1 = subReader.ReadElement<float>();
		component.val2 = subReader.ReadElement<float>();
		component.val3 = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Blending& component)
	{
		component.val0 = subReader.ReadElement<int8_t>();
		component.val1 = subReader.ReadElement<int8_t>();
		component.val2 = subReader.ReadElement<int8_t>();
		component.val3 = subReader.ReadElement<int8_t>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, BoneFixUp& component)
	{
		component.xScale = subReader.ReadElement<float>();
		component.xSkewY = subReader.ReadElement<float>();
		;
		component.xSkewZ = subReader.ReadElement<float>();
		component.xPosition = subReader.ReadElement<float>();
		component.ySkewX = subReader.ReadElement<float>();
		component.yScale = subReader.ReadElement<float>();
		component.ySkewZ = subReader.ReadElement<float>();
		component.yPosition = subReader.ReadElement<float>();
		component.zSkewX = subReader.ReadElement<float>();
		component.zSkewY = subReader.ReadElement<float>();
		component.zScale = subReader.ReadElement<float>();
		component.zPosition = subReader.ReadElement<float>();
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Model& component)
	{
		static constexpr size_t NUM_MODEL_INFOS = 24;

		component.name = subReader.ReadString(32);
		component.index = subReader.ReadElement<int32_t>();
		component.modelInfoOffsets = subReader.ReadElements<int32_t>(NUM_MODEL_INFOS);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, ModelInfo& component)
	{
		component.skinReference = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.meshes);
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, Mesh& component)
	{
		static constexpr size_t NUM_BONE_MAPPINGS = 24;

		component.boneMappings = subReader.ReadElements<int8_t>(NUM_BONE_MAPPINGS);
		component.triangleIndex = subReader.ReadElement<uint16_t>();
		component.triangleCount = subReader.ReadElement<uint16_t>();
		component.vertexIndexFrom = subReader.ReadElement<uint16_t>();
		component.vertexIndexTo = subReader.ReadElement<uint16_t>();
	}

	void
	ComponentReader::ReadInternal(BufferedFileReader& subReader, AnimationDataHolder& component, int32_t frameCount)
	{
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
	}

	void ComponentReader::ReadInternal(BufferedFileReader& subReader, SkinDataHolder& component)
	{
		component.Clear();

		for ( size_t skinFamilyIndex = 0; skinFamilyIndex < component.SkinFamilyCount(); ++skinFamilyIndex )
		{
			for ( size_t skinRefIndex = 0; skinRefIndex < component.SkinReferenceCount(); ++skinRefIndex )
			{
				component.GetEntry(skinFamilyIndex, skinRefIndex) = subReader.ReadElement<int16_t>();
			}
		}
	}
}  // namespace MDLv14
