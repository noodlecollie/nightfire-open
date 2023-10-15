#pragma once

#include <cstdint>
#include <string>
#include "MDLV14Components/ISerialisableComponent.h"

class BoundingBox : public ISerialisableComponent
{
public:
	void Clear() override;
	size_t ReadComponentData(BufferedFileReader& data) override;

private:
	struct Data
	{
		float minX = 0.0f;
		float minY = 0.0f;
		float minZ = 0.0f;
		float maxX = 0.0f;
		float maxY = 0.0f;
		float maxZ = 0.0f;
	};

	Data m_Data;
};
