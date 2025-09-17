#include "newdllfunctions.h"
#include "botrix/server_plugin.h"

void pfnOnFreeEntPrivateData(struct edict_s* edict)
{
	Botrix::CBotrixServerPlugin::EntityFreed(edict);
}
