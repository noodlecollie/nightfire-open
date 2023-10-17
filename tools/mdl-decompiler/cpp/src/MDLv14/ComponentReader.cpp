#include "MDLv14/ComponentReader.h"
#include "BufferedFileReader.h"

namespace MDLv14
{
	static constexpr size_t VEC3D_READ_SIZE = 3 * 4;
	static constexpr size_t EYEPOSITION_READ_SIZE = 3 * 4;
	static constexpr size_t BOUNDINGBOX_READ_SIZE = 2 * VEC3D_READ_SIZE;

	// TODO: Update to make this less opaque
	static constexpr size_t HEADER_READ_SIZE = 484; // From original code

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
		BufferedFileReader subReader = ref.CreateSubReader(HEADER_READ_SIZE);

		component.identifier = subReader.ReadString(4);
		component.version = subReader.ReadElement<int32_t>();
		component.name = subReader.ReadString(64);
		component.length = subReader.ReadElement<int32_t>();
		ReadNestedComponent(subReader, component.eyePosition);
		ReadNestedComponent(subReader, component.boundingBox);
		ReadNestedComponent(subReader, component.clippingBox);
		component.typeFlags = subReader.ReadElement<int32_t>();

		// TODO: More

		return subReader;
	}
}  // namespace MDLv14
