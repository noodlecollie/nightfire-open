#include "MDLV14Components/HeaderV14.h"
#include "BufferedFileReader.h"

void HeaderV14::Clear()
{
	m_Data = Data{};
}

size_t HeaderV14::ReadComponentData(BufferedFileReader& data)
{
	Clear();

	BufferedFileReader reader = data.CreateSubReader(484);

	m_Data.identifier = data.ReadString(4);
	m_Data.version = data.ReadElement<int32_t>();
	m_Data.name = data.ReadString(64);
	m_Data.length = data.ReadElement<int32_t>();

	reader.SeekForward(m_Data.eyePosition.ReadComponentData(reader));
	reader.SeekForward(m_Data.boundingBox.ReadComponentData(reader));
	reader.SeekForward(m_Data.clippingBox.ReadComponentData(reader));

	m_Data.typeFlags = reader.ReadElement<int32_t>();

	reader.EnsureAtEnd();
	return reader.Length();
}
