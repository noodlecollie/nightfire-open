#include "customGeometry/logger_client.h"

namespace CustomGeometry
{
	CLogger_Client& CLogger_Client::StaticInstance()
	{
		static CLogger_Client logger;
		return logger;
	}

	CLogger_Client::CLogger_Client() :
		CDebugLogger("debug_customgeometry_client")
	{
	}
}  // namespace CustomGeometry
