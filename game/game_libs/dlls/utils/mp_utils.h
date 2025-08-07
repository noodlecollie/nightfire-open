#pragma once

#include <functional>
#include "utlstring.h"

class CBasePlayer;

// Utility functions that are useful in multiplayer games.
namespace MPUtils
{
	// Callback should return true to continue iteration or false to stop.
	// CBasePlayer* argument is the player, int argument is their entity index.
	void ForEachPlayer(const std::function<bool(CBasePlayer*, int)>& callback);

	CBasePlayer* CBasePlayerFromIndex(int playerIndex);
	CBasePlayer* CBasePlayerFromUserId(int userId);
	CBasePlayer* CBasePlayerFromNetName(const char* netName);

	// Format:
	// - If string begins with '#', translates the rest of the string to a user ID and looks up a player
	//   by this ID.
	// - Otherwise, looks up a player by net name according to the passed string.
	CBasePlayer* CBasePlayerFromStringLookup(const char* string);

	const char* PlayerNetName(CBasePlayer* player);
	CUtlString SanitisePlayerNetName(const CUtlString& name);
	CUtlString PlayerModelPath(CBasePlayer* player);
}  // namespace MPUtils
