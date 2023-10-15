#include "MDLV14Components/MDLFileV14.h"
#include "BufferedFileReader.h"

void MDLFileV14::Clear()
{
	m_Data = Data{};
}

size_t MDLFileV14::ReadComponentData(BufferedFileReader& data)
{
	Clear();

	data.SeekForward(m_Data.header.ReadComponentData(data));

	return data.CurrentPosition();
}
