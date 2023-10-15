#include "MDLV14Components/HeaderV14.h"
#include "BufferedFileReader.h"

void HeaderV14::Clear()
{
	m_Data = Data{};
}

void HeaderV14::ReadComponentData(BufferedFileReader& data)
{
	Clear();

	m_Data.identifier = data.ReadString(4);
}
