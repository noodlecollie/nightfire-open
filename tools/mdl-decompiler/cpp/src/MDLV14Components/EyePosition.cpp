#include "MDLV14Components/EyePosition.h"
#include "BufferedFileReader.h"

void EyePosition::Clear()
{
	m_Data = Data{};
}

size_t EyePosition::ReadComponentData(BufferedFileReader& data)
{
	Clear();

	BufferedFileReader reader = data.CreateSubReader(3 * 4);

	// The ordering here is deliberately Y, X, Z.
	m_Data.y = reader.ReadElement<float>();
	m_Data.x = reader.ReadElement<float>();
	m_Data.z = reader.ReadElement<float>();

	reader.EnsureAtEnd();
	return reader.Length();
}
