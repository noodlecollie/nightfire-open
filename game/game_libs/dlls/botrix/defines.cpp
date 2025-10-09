#include "botrix/defines.h"
#include "botrix/server_plugin.h"

namespace Botrix
{
	bool ShouldLogToConsole(int level)
	{
		return CBotrixServerPlugin::ShouldLogToConsole(level);
	}
}  // namespace Botrix
