#pragma once

#include "projectInterface/IRNG.h"

class RNG_Client : public IRNG
{
public:
	virtual ~RNG_Client()
	{
	}

	virtual float GetRandomFloat(float min, float max) override;
	virtual int32_t GetRandomInt(int32_t min, int32_t max) override;
};
