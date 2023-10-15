#pragma once

#include <cstdint>
#include <string>
#include "MDLV14Components/ISerialisableComponent.h"

class EyePosition : public ISerialisableComponent
{
public:
	void Clear() override;
	size_t ReadComponentData(BufferedFileReader& data) override;

private:
	struct Data
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	Data m_Data;
};
