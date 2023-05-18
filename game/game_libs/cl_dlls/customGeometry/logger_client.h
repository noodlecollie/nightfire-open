#pragma once

#include "util/debugLogger.h"

namespace CustomGeometry
{
	class CLogger_Client : public CDebugLogger
	{
	public:
		static CLogger_Client& StaticInstance();

		CLogger_Client();
	};

	static inline CLogger_Client CL_LOG()
	{
		return CLogger_Client::StaticInstance();
	}
}  // namespace CustomGeometry
