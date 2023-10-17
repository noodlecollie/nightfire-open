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

		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Vec3D& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, BoundingBox& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, EyePosition& component);
		BufferedFileReader ReadInternal(BufferedFileReader::Ref ref, Header& component);
	};
}  // namespace MDLv14
