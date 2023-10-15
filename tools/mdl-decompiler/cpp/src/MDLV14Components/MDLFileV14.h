#pragma once

#include "MDLV14Components/ISerialisableComponent.h"
#include "MDLV14Components/HeaderV14.h"

class MDLFileV14 : public ISerialisableComponent
{
public:
	void Clear() override;
	void ReadComponentData(BufferedFileReader& data) override;

private:
	struct Data
	{
		HeaderV14 header;
	};

	Data m_Data;
};
