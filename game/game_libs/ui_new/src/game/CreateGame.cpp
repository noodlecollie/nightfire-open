#include "game/CreateGame.h"
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/StringUtilities.h>
#include "game/Utils.h"
#include "udll_int.h"

void CreateMultiplayerGame(const Rml::String& mapName)
{
	if ( !gEngfuncs.pfnIsMapValid(mapName.c_str()) )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Map \"%s\" is not valid", mapName.c_str());
		return;
	}

	Rml::Log::Message(Rml::Log::Type::LT_INFO, "Starting new multiplayer game on %s", mapName.c_str());

	if ( gEngfuncs.pfnGetCvarFloat("host_serverstate") )
	{
		const char* msg = gEngfuncs.pfnGetCvarFloat("maxplayers") < 2.0 ? "end of the game" : "starting new server";
		gEngfuncs.pfnHostEndGame(msg);
	}

	gEngfuncs.pfnPlayBackgroundTrack(nullptr, nullptr);

	// Dirty listenserver config from old xash may rewrite maxplayers.
	// Not sure what to do about this, but the old code just re-wrote the cvar.
	const int maxClients = static_cast<int>(gEngfuncs.pfnGetCvarFloat("maxplayers"));

	const char* cfgFileName = gEngfuncs.pfnGetCvarString("lservercfgfile");
	gEngfuncs.pfnWriteServerConfig(cfgFileName);

	gEngfuncs.pfnClientCmd(true, Rml::CreateString("exec %s", cfgFileName).c_str());
	gEngfuncs.pfnCvarSetValue("maxplayers", static_cast<float>(maxClients));

	const Rml::String escapedMapName = EscapeStringForConsoleCommand(mapName);

	// Hack: wait three frames allowing server to completely shutdown, reapply maxplayers and start new map.
	// NFTODO: This incantation should probably be a function within the engine that manages all of these things.
	gEngfuncs.pfnClientCmd(
		false,
		Rml::CreateString(
			"endgame;localserver;wait;wait;wait;maxplayers %d;map %s\n",
			maxClients,
			escapedMapName.c_str()
		)
			.c_str()
	);

	// TODO: Add bots
}
