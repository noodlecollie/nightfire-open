#pragma once

#include <string>
#include "MDLV14Components/ISerialisableComponent.h"

class HeaderV14 : public ISerialisableComponent
{
public:
	void Clear() override;
	void ReadComponentData(BufferedFileReader& data) override;

private:
	struct Data
	{
		std::string identifier;
	};

	Data m_Data;
};
