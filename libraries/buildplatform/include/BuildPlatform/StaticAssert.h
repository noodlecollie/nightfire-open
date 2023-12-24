#pragma once

#if defined(static_assert)  // C11 static_assert
#define STATIC_ASSERT static_assert
#else
#define STATIC_ASSERT(x, y) extern int _static_assert_##__LINE__[(x) ? 1 : -1]
#endif
