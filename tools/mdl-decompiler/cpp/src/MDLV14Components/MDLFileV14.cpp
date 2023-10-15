#include "MDLV14Components/MDLFileV14.h"
#include "BufferedFileReader.h"

void MDLFileV14::Clear()
{
	m_Data = Data{};
}

void MDLFileV14::ReadComponentData(BufferedFileReader& data)
{
	m_Data.header.ReadComponentData(data);
}
