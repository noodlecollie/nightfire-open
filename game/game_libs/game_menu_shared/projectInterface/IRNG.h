#pragma once

#include <cstdint>

class IRNG
{
public:
	virtual ~IRNG()
	{
	}

	virtual float GetRandomFloat(float min, float max) = 0;
	virtual int32_t GetRandomInt(int32_t min, int32_t max) = 0;
};
