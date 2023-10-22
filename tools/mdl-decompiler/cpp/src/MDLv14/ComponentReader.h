#pragma once

#include <functional>
#include <type_traits>
#include "Common/Container.h"
#include "MDLv14/Components.h"
#include "MDLv14/ComponentReflection.h"
#include "MDLv14/SkinDataHolder.h"
#include "BufferedFileReader.h"
#include "Traits/Reflection.h"

namespace MDLv14
{
	class ComponentReader
	{
	public:
		template<typename T>
		T ReadComponent(BufferedFileReader& reader)
		{
			T component {};
			ReadNestedComponent(reader, component);

			return component;
		}

		template<typename T>
		Container<T> ReadComponentArray(BufferedFileReader& reader, size_t elementCount)
		{
			Container<T> out;

			for ( size_t index = 0; index < elementCount; ++index )
			{
				out.emplace_back(ReadComponent<T>(reader));
			}

			return out;
		}

		template<typename T>
		Container<T> ReadComponentArray(BufferedFileReader& reader, size_t elementCount, size_t beginByteOffset)
		{
			reader.SeekFromBeginning(beginByteOffset);
			return ReadComponentArray<T>(reader, elementCount);
		}

		// The outer reader is not modified here, since the data access is fairly random.
		// Make sure to manually seek the outer reader forward after calling this function.
		AnimationDataHolder
		ReadAnimationData(BufferedFileReader::Ref readerRef, size_t blendCount, size_t boneCount, int32_t frameCount)
		{
			AnimationDataHolder anims(blendCount, boneCount);

			if ( anims.IsValid() )
			{
				BufferedFileReader subReader = readerRef.CreateSubReader();
				ReadInternal(subReader, anims, frameCount);
			}

			return anims;
		}

		SkinDataHolder ReadSkinData(BufferedFileReader& reader, size_t numSkinFamilies, size_t numSkinReferences)
		{
			SkinDataHolder skinData(numSkinFamilies, numSkinReferences);

			if ( skinData.IsValid() )
			{
				ReadInternal(reader, skinData);
			}

			return skinData;
		}

	private:
		// Helpers:

		template<typename T, typename... ARGS>
		void ReadNestedComponent(BufferedFileReader& reader, T& component, ARGS&&... args)
		{
			BufferedFileReader subReader = reader.CreateSubReader(Reflection::ReadSize<T>());
			ReadInternal(subReader, component, std::forward<ARGS>(args)...);
			subReader.EnsureAtEnd();

			const size_t delta = subReader.CurrentPosition();
			reader.SeekForward(delta);
		}

		// Per-component implementation:

		void ReadInternal(BufferedFileReader& subReader, CountOffsetPair& component);
		void ReadInternal(BufferedFileReader& subReader, Vec3D& component);
		void ReadInternal(BufferedFileReader& subReader, BoundingBox& component);
		void ReadInternal(BufferedFileReader& subReader, EyePosition& component);
		void ReadInternal(BufferedFileReader& subReader, Header& component);
		void ReadInternal(BufferedFileReader& subReader, Bone& component);
		void ReadInternal(BufferedFileReader& subReader, BoneController& component);
		void ReadInternal(BufferedFileReader& subReader, HitBox& component);
		void ReadInternal(BufferedFileReader& subReader, Event& component);
		void ReadInternal(BufferedFileReader& subReader, Pivot& component);
		void ReadInternal(BufferedFileReader& subReader, Sequence& component);
		void ReadInternal(BufferedFileReader& subReader, SequenceGroup& component);
		void ReadInternal(BufferedFileReader& subReader, LevelOfDetail& component);
		void ReadInternal(BufferedFileReader& subReader, Texture& component);
		void ReadInternal(BufferedFileReader& subReader, BodyGroup& component);
		void ReadInternal(BufferedFileReader& subReader, Attachment& component);
		void ReadInternal(BufferedFileReader& subReader, SoundGroup& component);
		void ReadInternal(BufferedFileReader& subReader, Sounds& component);
		void ReadInternal(BufferedFileReader& subReader, TriangleMap& component);
		void ReadInternal(BufferedFileReader& subReader, Vertex& component);
		void ReadInternal(BufferedFileReader& subReader, Normal& component);
		void ReadInternal(BufferedFileReader& subReader, TextureCoOrdinate& component);
		void ReadInternal(BufferedFileReader& subReader, BlendingScales& component);
		void ReadInternal(BufferedFileReader& subReader, Blending& component);
		void ReadInternal(BufferedFileReader& subReader, BoneFixUp& component);
		void ReadInternal(BufferedFileReader& subReader, Model& component);
		void ReadInternal(BufferedFileReader& subReader, ModelInfo& component);
		void ReadInternal(BufferedFileReader& subReader, Mesh& component);
		void ReadInternal(BufferedFileReader& subReader, AnimationDataHolder& component, int32_t frameCount);
		void ReadInternal(BufferedFileReader& subReader, SkinDataHolder& component);
	};
}  // namespace MDLv14
