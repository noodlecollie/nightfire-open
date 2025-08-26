#include "bot_commands.h"
#include "standard_includes.h"
#include "enginecallback.h"
#include "EnginePublicAPI/eiface.h"
#include "gamerules.h"
#include "botgamerulesinterface.h"
#include "botregister.h"
#include "bots/bot_cvars.h"
#include "mp_utils.h"
#include "botrix/server_plugin.h"
#include "botrix/console_commands.h"

namespace BotCommands
{
	static CHalfLifeMultiplay* GetGameRules()
	{
		return g_pGameRules ? dynamic_cast<CHalfLifeMultiplay*>(g_pGameRules) : nullptr;
	}

	static bool UsingRhoBotBackend()
	{
		return FStrEq(bot_backend.string, "rhobot");
	}

	static bool UsingBotrixBackend()
	{
		return FStrEq(bot_backend.string, "botrix");
	}

	void Initialise()
	{
		g_engfuncs.pfnAddServerCommand("botrix", &BotrixCmdEntryPoint);

		g_engfuncs.pfnAddServerCommand("bot_add", &Bot_Add);
		g_engfuncs.pfnAddServerCommand("bot_removeall", &Bot_RemoveAll);

		g_engfuncs.pfnAddServerCommand("bot_register_add", &Bot_Register_Add);
		g_engfuncs.pfnAddServerCommand("bot_register_clear", &Bot_Register_Clear);
		g_engfuncs.pfnAddServerCommand("bot_register_list", &Bot_Register_List);
	}

	void BotrixCmdEntryPoint(void)
	{
		if ( !UsingBotrixBackend() )
		{
			ALERT(at_error, "Botrix command not supported for bot backend \"%s\"\n", bot_backend.string);
			return;
		}

		CHalfLifeMultiplay* gameRules = GetGameRules();

		if ( !gameRules || !gameRules->IsMultiplayer() )
		{
			ALERT(at_error, "Cannot add bots when not in multiplayer game.\n");
			return;
		}

		CBotrixCommand* commandHandler = CBotrixServerPlugin::GetConsoleCommandHandler();

		if ( !commandHandler )
		{
			ALERT(at_error, "Botrix backend unavailable.\n");
			return;
		}

		commandHandler->AcceptServerCommand();
	}

	void Bot_Add(void)
	{
		if ( !UsingRhoBotBackend() )
		{
			ALERT(at_error, "Rho-bot command not supported for bot backend \"%s\"\n", bot_backend.string);
			return;
		}

		CHalfLifeMultiplay* gameRules = GetGameRules();

		if ( !gameRules || !gameRules->IsMultiplayer() )
		{
			ALERT(at_error, "Cannot add bots when not in multiplayer game.\n");
			return;
		}

		CBotGameRulesInterface* bgri = dynamic_cast<CBotGameRulesInterface*>(gameRules->BotGameRulesInterface());

		if ( !bgri )
		{
			ALERT(at_error, "Game rules do not support bot commands.\n");
			return;
		}

		CBotFactory& botFactory = bgri->BotFactory();

		int numArgs = CMD_ARGC();

		if ( numArgs < 2 )
		{
			// Add a new bot with a random profile.
			botFactory.CreateBots(1);
			return;
		}

		if ( numArgs == 2 )
		{
			const char* arg = CMD_ARGV(1);

			if ( arg && (*arg < '0' || *arg > '9') )
			{
				// The first character was not a digit, so treat as a profile name.
				botFactory.TryCreateBot(arg);
			}
			else
			{
				// Try to parse as a number and add some random bots.
				int botCount = atoi(arg);

				if ( botCount > 0 )
				{
					botFactory.CreateBots(botCount);
				}
				else
				{
					ALERT(at_error, "Amount of bots to add must be a positive number.\n", arg);
				}
			}

			return;
		}

		for ( int argNum = 1; argNum < numArgs; ++argNum )
		{
			// Treat each argument as a bot profile.
			botFactory.TryCreateBot(CMD_ARGV(argNum));
		}
	}

	void Bot_RemoveAll(void)
	{
		if ( !UsingRhoBotBackend() )
		{
			ALERT(at_error, "Rho-bot command not supported for bot backend \"%s\"\n", bot_backend.string);
			return;
		}

		if ( !g_pGameRules || !g_pGameRules->IsMultiplayer() )
		{
			return;
		}

		CUtlString kickCommands;

		for ( int clientIndex = 1; clientIndex <= gpGlobals->maxClients; ++clientIndex )
		{
			CBasePlayer* player = MPUtils::CBasePlayerFromIndex(clientIndex);

			if ( !player || !player->IsFakeClient() )
			{
				continue;
			}

			const char* name = STRING(player->pev->netname);
			kickCommands.AppendFormat("kick \"%s\"\n", name);
		}

		if ( !kickCommands.IsEmpty() )
		{
			SERVER_COMMAND(kickCommands.String());
		}
	}

	void Bot_Register_Add()
	{
		int numArgs = CMD_ARGC();

		if ( numArgs < 2 )
		{
			ALERT(at_console, "Usage: bot_register_add <profile name> [player name]\n");
			return;
		}

		const char* profileName = CMD_ARGV(1);
		const char* customName = CMD_ARGC() >= 3 ? CMD_ARGV(2) : nullptr;

		CBotRegister& reg = CBotRegister::StaticInstance();

		if ( !reg.Add(CUtlString(profileName), CUtlString(customName)) )
		{
			ALERT(at_error, "Could not add bot: register was full.\n");
		}
	}

	void Bot_Register_Clear()
	{
		CBotRegister::StaticInstance().Clear();
	}

	void Bot_Register_List()
	{
		CBotRegister& reg = CBotRegister::StaticInstance();

		if ( reg.Count() < 1 )
		{
			ALERT(at_console, "Bot register contains 0 entries.\n");
			return;
		}

		ALERT(at_console, "Bot register contains %u entries:\n", reg.Count());

		for ( uint32_t index = 0; index < reg.Count(); ++index )
		{
			const CUtlString profileName = reg.ProfileName(index);
			const CUtlString customName = reg.CustomName(index);

			if ( customName.IsValid() )
			{
				ALERT(at_console, "  %u: %s (\"%s\")\n", index + 1, profileName.String(), customName.String());
			}
			else
			{
				ALERT(at_console, "  %u: %s\n", index + 1, profileName.String());
			}
		}
	}
}  // namespace BotCommands
