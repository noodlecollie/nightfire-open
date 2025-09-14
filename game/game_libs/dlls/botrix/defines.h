#pragma once

#include <algorithm>
#include <good/log.h>
#include "botrix/engine_util.h"
#include "botrix/logbuffer.h"
#include "PlatformLib/String.h"

// Define for logging.
#define BLOG(user, level, ...) \
	do \
	{ \
		int iMin = CBotrixEngineUtil::iLogLevel; \
		if ( level >= iMin ) \
		{ \
			good::log::format(CBotrixLogBuffer::Buffer(), CBotrixLogBuffer::Size(), __VA_ARGS__); \
			PlatformLib_StrCat(CBotrixLogBuffer::Buffer(), CBotrixLogBuffer::Size(), "\n"); \
			CBotrixEngineUtil::Message(level, user, CBotrixLogBuffer::Buffer()); \
		} \
	} \
	while ( false )

// Botrix log with level. Log to server.
#define BLOG_T(...) BLOG(nullptr, good::ELogLevelTrace, __VA_ARGS__)
#define BLOG_D(...) BLOG(nullptr, good::ELogLevelDebug, __VA_ARGS__)
#define BLOG_I(...) BLOG(nullptr, good::ELogLevelInfo, __VA_ARGS__)
#define BLOG_W(...) BLOG(nullptr, good::ELogLevelWarning, __VA_ARGS__)
#define BLOG_E(...) BLOG(nullptr, good::ELogLevelError, __VA_ARGS__)

// Log to server and also log to user.
#define BULOG_T(user, ...) BLOG(user, good::ELogLevelTrace, __VA_ARGS__)
#define BULOG_D(user, ...) BLOG(user, good::ELogLevelDebug, __VA_ARGS__)
#define BULOG_I(user, ...) BLOG(user, good::ELogLevelInfo, __VA_ARGS__)
#define BULOG_W(user, ...) BLOG(user, good::ELogLevelWarning, __VA_ARGS__)
#define BULOG_E(user, ...) BLOG(user, good::ELogLevelError, __VA_ARGS__)

// Non fatal assert.
#define BASSERT(exp, ...) \
	do \
	{ \
		if ( !(exp) ) \
		{ \
			BLOG_E("Assert failed: (%s); in %s(), file %s, line %d\n", #exp, __FUNCTION__, __FILE__, __LINE__); \
			BreakDebugger(); \
			__VA_ARGS__; \
		} \
	} \
	while ( false )
