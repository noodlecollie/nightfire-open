#pragma once

#include <functional>
#include <type_traits>
#include "Common/Container.h"
#include "MDLv14/Components.h"
#include "MDLv14/SkinDataHolder.h"
#include "BufferedFileReader.h"

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
				ReadInternal(readerRef, anims, frameCount);
			}

			return anims;
		}

		SkinDataHolder ReadSkinData(BufferedFileReader& reader, size_t numSkinFamilies, size_t numSkinReferences)
		{
			SkinDataHolder skinData(numSkinFamilies, numSkinReferences);

			if ( skinData.IsValid() )
			{
				ReadNestedComponent<SkinDataHolder>(reader, skinData);
			}

			return skinData;
		}

	private:
		// Helpers:

		template<typename T, typename... ARGS>
		void ReadNestedComponent(BufferedFileReader& reader, T& component, ARGS&&... args)
		{
			BufferedFileReader subReader =
				ReadInternal(BufferedFileReader::Ref(reader), component, std::forward<ARGS>(args)...);

			subReader.EnsureAtEnd();

			const size_t delta = subReader.CurrentPosition();
			reader.SeekForward(delta);
		}

		// Per-component implementation:

		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, CountOffsetPair& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Vec3D& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, BoundingBox& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, EyePosition& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Header& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Bone& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, BoneController& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, HitBox& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Event& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Pivot& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Sequence& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, SequenceGroup& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, LevelOfDetail& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Texture& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, BodyGroup& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Attachment& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, SoundGroup& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Sounds& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, TriangleMap& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Vertex& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Normal& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, TextureCoOrdinate& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, BlendingScales& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Blending& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, BoneFixUp& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Model& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, ModelInfo& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Mesh& component);

		BufferedFileReader
		ReadInternal(BufferedFileReader::Ref ref, AnimationDataHolder& component, int32_t frameCount);

		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, SkinDataHolder& component);
	};
}  // namespace MDLv14
