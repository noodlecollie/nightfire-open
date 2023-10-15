#include "MDLV14Components/BoundingBox.h"
#include "BufferedFileReader.h"

void BoundingBox::Clear()
{
	m_Data = Data{};
}

size_t BoundingBox::ReadComponentData(BufferedFileReader& data)
{
	Clear();

	BufferedFileReader reader = data.CreateSubReader(6 * 4);

	m_Data.minX = reader.ReadElement<float>();
	m_Data.minY = reader.ReadElement<float>();
	m_Data.minZ = reader.ReadElement<float>();
	m_Data.maxX = reader.ReadElement<float>();
	m_Data.maxY = reader.ReadElement<float>();
	m_Data.maxZ = reader.ReadElement<float>();

	reader.EnsureAtEnd();
	return reader.Length();
}
