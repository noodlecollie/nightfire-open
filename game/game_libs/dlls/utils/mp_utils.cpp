#include "mp_utils.h"
#include "standard_includes.h"
#include "gameresources/GameResources.h"

namespace MPUtils
{
	void ForEachPlayer(const std::function<bool(CBasePlayer*, int)>& callback)
	{
		for ( int index = 1; index < gpGlobals->maxClients; ++index )
		{
			CBasePlayer* player = CBasePlayerFromIndex(index);

			if ( !player )
			{
				continue;
			}

			if ( !callback(player, index) )
			{
				break;
			}
		}
	}

	CBasePlayer* CBasePlayerFromIndex(int playerIndex)
	{
		if ( playerIndex < 1 || playerIndex > gpGlobals->maxClients )
		{
			return NULL;
		}

		edict_t* pPlayerEdict = INDEXENT(playerIndex);

		if ( !pPlayerEdict || pPlayerEdict->free || (pPlayerEdict->v.flags & (FL_FAKECLIENT | FL_CLIENT)) == 0 )
		{
			return NULL;
		}

		return GetClassPtrFromEdict<CBasePlayer>(pPlayerEdict);
	}

	CBasePlayer* CBasePlayerFromUserId(int userId)
	{
		for ( int clientIndex = 1; clientIndex <= gpGlobals->maxClients; ++clientIndex )
		{
			CBasePlayer* player = CBasePlayerFromIndex(clientIndex);

			if ( !player )
			{
				continue;
			}

			int playerUserId = g_engfuncs.pfnGetPlayerUserId(player->edict());

			if ( playerUserId == userId )
			{
				return player;
			}
		}

		return nullptr;
	}

	CBasePlayer* CBasePlayerFromNetName(const char* netName)
	{
		for ( int clientIndex = 1; clientIndex <= gpGlobals->maxClients; ++clientIndex )
		{
			CBasePlayer* player = CBasePlayerFromIndex(clientIndex);

			if ( !player )
			{
				continue;
			}

			if ( strcmp(PlayerNetName(player), netName) == 0 )
			{
				return player;
			}
		}

		return nullptr;
	}

	CBasePlayer* CBasePlayerFromStringLookup(const char* string)
	{
		if ( string[0] == '#' && string[1] != '\0' && isdigit(string[1]) )
		{
			return MPUtils::CBasePlayerFromUserId(atoi(++string));
		}
		else
		{
			return MPUtils::CBasePlayerFromNetName(string);
		}
	}

	const char* PlayerNetName(CBasePlayer* player)
	{
		return player ? STRING(player->pev->netname) : NULL;
	}

	CUtlString SanitisePlayerNetName(const CUtlString& name)
	{
		if ( name.IsEmpty() )
		{
			return CUtlString("Player");
		}

		CUtlString out;
		out.AppendRepeat('\0', name.Length() + 1);

		char* buffer = out.Access();
		bool lastCharacterWasInvalid = false;

		for ( uint32_t index = 0; index < name.Length(); ++index )
		{
			const char ch = name[index];
			const bool characterValid = ch >= ' ' && ch <= '~' && ch != '"';

			if ( characterValid )
			{
				*(buffer++) = ch;
				lastCharacterWasInvalid = false;
			}
			else if ( !lastCharacterWasInvalid )
			{
				*(buffer++) = ' ';
				lastCharacterWasInvalid = true;
			}
		}

		out.TrimTrailingWhitespace();
		return out;
	}

	CUtlString PlayerModelPath(CBasePlayer* player)
	{
		if ( !player )
		{
			return CUtlString();
		}

		char* info = g_engfuncs.pfnGetInfoKeyBuffer(player->edict());
		const char* modelName = g_engfuncs.pfnInfoKeyValue(info, "model");
		return CGameResources::MultiplayerModelFullPath(CUtlString(modelName));
	}
}  // namespace MPUtils
