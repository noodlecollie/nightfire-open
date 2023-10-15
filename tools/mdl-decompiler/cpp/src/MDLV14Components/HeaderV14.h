#pragma once

#include <cstdint>
#include <string>
#include "MDLV14Components/ISerialisableComponent.h"
#include "MDLV14Components/EyePosition.h"
#include "MDLV14Components/BoundingBox.h"

class HeaderV14 : public ISerialisableComponent
{
public:
	void Clear() override;
	size_t ReadComponentData(BufferedFileReader& data) override;

private:
	struct Data
	{
		std::string identifier;
		int32_t version = 0;
		std::string name;
		int32_t length = 0;
		EyePosition eyePosition;
		BoundingBox boundingBox;
		BoundingBox clippingBox;
		int32_t typeFlags;
	};

	Data m_Data;
};
