#ifndef NET_BYTESWAP_H
#define NET_BYTESWAP_H

#include "BuildPlatform/PlatformID.h"

#if XASH_BIG_ENDIAN()
#define LittleLong(x) \
	(((int)(((x)&255) << 24)) + ((int)((((x) >> 8) & 255) << 16)) + ((int)(((x) >> 16) & 255) << 8) + \
	 (((x) >> 24) & 255))
#define LittleLongSW(x) (x = LittleLong(x))
#define LittleShort(x) ((short)((((short)(x) >> 8) & 255) + (((short)(x)&255) << 8)))
#define LittleShortSW(x) (x = LittleShort(x))
static inline float LittleFloat(float f)
{
	union
	{
		float f;
		unsigned char b[4];
	} dat1, dat2;

	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];

	return dat2.f;
}
#else
#define LittleLong(x) (x)
#define LittleLongSW(x)
#define LittleShort(x) (x)
#define LittleShortSW(x)
#define LittleFloat(x) (x)
#endif

#endif // NET_BYTESWAP_H
