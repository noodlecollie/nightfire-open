#pragma once

// Required before math.h:
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <math.h>

#ifndef M_PI2
#define M_PI2 ((double)(2.0 * M_PI))
#endif

#define M_PI_F ((float)(M_PI))
#define M_PI2_F ((float)(M_PI2))

#define MATH_FLOAT_EQUAL_EPSILON 0.001f

#define SIDE_FRONT 0
#define SIDE_BACK 1
#define SIDE_ON 2
#define SIDE_CROSS -2

#define RAD_TO_STUDIO (32768.0 / M_PI)
#define STUDIO_TO_RAD (M_PI / 32768.0)
#define INV127F (1.0f / 127.0f)
#define INV255F (1.0f / 255.0f)
#define MAKE_SIGNED(x) (((x) * INV127F) - 1.0f)

#define RAD2DEG(x) ((double)(x) * (double)(180.0 / M_PI))
#define DEG2RAD(x) ((double)(x) * (double)(M_PI / 180.0))
#define RAD2DEGF(x) ((float)(x) * (float)(180.0f / M_PI_F))
#define DEG2RADF(x) ((float)(x) * (float)(M_PI_F / 180.0f))

#define Q_min(a, b) (((a) < (b)) ? (a) : (b))
#define Q_max(a, b) (((a) > (b)) ? (a) : (b))
#define Q_equal(a, b) (((a) > ((b)-MATH_FLOAT_EQUAL_EPSILON)) && ((a) < ((b) + MATH_FLOAT_EQUAL_EPSILON)))
#define Q_recip(a) ((float)(1.0f / (float)(a)))
#define Q_floor(a) ((float)(int)(a))
#define Q_ceil(a) ((float)(int)((a) + 1))
#define Q_round(x, y) (floorf(x / y + 0.5f) * y)
#define Q_roundsimple(a) (floorf((a) + 0.5f))
#define Q_rint(x) ((x) < 0.0f ? ((int)((x)-0.5f)) : ((int)((x) + 0.5f)))
#define Q_rounddn(x, y) (floorf(x / y + 0.5f) * y)
#define Q_roundup(x, y) (ceilf(x / y) * y)

#ifdef isnan  // check for C99 isnan
#define IS_NAN isnan
#else
#define IS_NAN(x) (((*(int*)&x) & (255 << 23)) == (255 << 23))
#endif
