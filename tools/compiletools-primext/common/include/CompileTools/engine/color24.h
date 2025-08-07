/*
color24.h - simple object for handling color
Copyright (C) 2019 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once

#include "BuildPlatform/Typedefs.h"

struct color24
{
public:
	color24()
	{
	}

	color24(byte cr, byte cg, byte cb)
	{
		r = cr;
		g = cg;
		b = cb;
	}

	inline color24 operator+(const color24& v) const
	{
		return color24(r + v.r, g + v.g, b + v.b);
	}

	inline color24 operator-(const color24& v) const
	{
		return color24(r - v.r, g - v.g, b - v.b);
	}

	inline color24 operator*(float fl) const
	{
		return color24(
			static_cast<byte>(static_cast<float>(r) * fl),
			static_cast<byte>(static_cast<float>(g) * fl),
			static_cast<byte>(static_cast<float>(b) * fl));
	}

	byte r;
	byte g;
	byte b;
};
