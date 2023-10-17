#pragma once

#include <functional>
#include <type_traits>
#include "MDLv14/Components.h"
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
		std::vector<T> ReadComponentArray(BufferedFileReader& reader, size_t elementCount)
		{
			std::vector<T> out;

			for ( size_t index = 0; index < elementCount; ++index )
			{
				out.emplace_back(ReadComponent<T>(reader));
			}

			return out;
		}

		template<typename T>
		std::vector<T> ReadComponentArray(BufferedFileReader& reader, size_t elementCount, size_t beginByteOffset)
		{
			reader.SeekFromBeginning(beginByteOffset);
			return ReadComponentArray<T>(reader, elementCount);
		}

	private:
		// Helpers:

		template<typename T>
		void ReadNestedComponent(BufferedFileReader& reader, T& component, bool mustReadEntireChunk = true)
		{
			BufferedFileReader subReader = ReadInternal(BufferedFileReader::Ref(reader), component);

			if ( mustReadEntireChunk )
			{
				subReader.EnsureAtEnd();
			}

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
	};
}  // namespace MDLv14
