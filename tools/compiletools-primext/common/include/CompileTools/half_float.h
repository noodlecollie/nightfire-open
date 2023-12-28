#pragma once

#pragma once

struct half
{
	unsigned short sh;

	half() {};
	half(const float x);
	operator float() const;
};

typedef half hvec3_t[3];
