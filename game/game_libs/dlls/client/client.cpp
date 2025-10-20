/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
// Robin, 4-22-98: Moved set_suicide_frame() here from player.cpp to allow us to
//				   have one without a hardcoded player.mdl in tf_client.cpp

/*

===== client.cpp ========================================================

  client/server game specific stuff

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "saverestore.h"
#include "player.h"
#include "spectator.h"
#include "client.h"
#include "soundent.h"
#include "gamerules.h"
#include "game.h"
#include "EnginePublicAPI/customentity.h"
#include "weapons.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "EnginePublicAPI/usercmd.h"
#include "EnginePublicAPI/netadr.h"
#include "pm_shared.h"
#include "gameresources/GameResources.h"
#include "prop_playercorpse.h"
#include "gameplay/gameplaySystems.h"
#include "resources/SoundResources.h"
#include "EnginePublicAPI/com_strings.h"
#include "PlatformLib/String.h"
#include "client/clientCommandRegister.h"

extern DLL_GLOBAL ULONG g_ulModelIndexPlayer;
extern DLL_GLOBAL BOOL g_fGameOver;
extern DLL_GLOBAL int g_iSkillLevel;
extern DLL_GLOBAL ULONG g_ulFrameCount;

extern int gmsgSayText;
extern int gmsgBhopcap;

extern cvar_t allow_spectators;
extern cvar_t multibyte_only;

extern int g_teamplay;

extern cvar_t bhopcap;
extern "C" int g_bhopcap;

static CClientCommandRegister g_ClientCommandRegister;

void LinkUserMessages(void);

/*
 * used by kill command and disconnect command
 * ROBIN: Moved here from player.cpp, to allow multiple player models
 */
void set_suicide_frame(entvars_t* pev)
{
	if ( !FStrEq(STRING(pev->model), PLAYER_MODEL_PATH) )
	{
		return;  // allready gibbed
	}

	// pev->frame = $deatha11;
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_TOSS;
	pev->deadflag = DEAD_DEAD;
	pev->nextthink = -1;
}

/*
===========
ClientConnect

called when a player connects to a server
============
*/
BOOL ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	return g_pGameRules->ClientConnected(pEntity, pszName, pszAddress, szRejectReason);

	// a client connecting during an intermission can cause problems
	//	if( intermission_running )
	//		ExitIntermission();
}

/*
===========
ClientDisconnect

called when a player disconnects from a server

GLOBALS ASSUMED SET:  g_fGameOver
============
*/
void ClientDisconnect(edict_t* pEntity)
{
	if ( g_fGameOver )
	{
		return;
	}

	char text[256] = "";
	if ( pEntity->v.netname )
		PlatformLib_SNPrintF(text, sizeof(text), "- %s has left the game\n", STRING(pEntity->v.netname));
	MESSAGE_BEGIN(MSG_ALL, gmsgSayText, NULL);
	WRITE_BYTE(ENTINDEX(pEntity));
	WRITE_STRING(text);
	MESSAGE_END();

	CSound* pSound = CSoundEnt::SoundPointerForIndex(CSoundEnt::ClientSoundIndex(pEntity));

	// since this client isn't around to think anymore, reset their sound.
	if ( pSound )
	{
		pSound->Reset();
	}

	// since the edict doesn't get deleted, fix it so it doesn't interfere.
	pEntity->v.takedamage = DAMAGE_NO;  // don't attract autoaim
	pEntity->v.solid = SOLID_NOT;  // nonsolid
	pEntity->v.effects = EF_NODRAW;  // clear any effects
	UTIL_SetOrigin(&pEntity->v, pEntity->v.origin);

	g_pGameRules->ClientDisconnected(pEntity);

	// Ensure that the client flags are removed.
	pEntity->v.flags &= ~(FL_CLIENT | FL_FAKECLIENT);
}

// called by ClientKill and DeadThink
void respawn(entvars_t* pev, BOOL fCopyCorpse)
{
	if ( gpGlobals->coop || gpGlobals->deathmatch )
	{
		if ( fCopyCorpse )
		{
			// make a copy of the dead body for appearances sake
			CPropPlayerCorpse::Create(pev);
		}

		// respawn player
		GetClassPtr<CBasePlayer>(pev)->Spawn();
	}
	else
	{  // restart the entire server
		SERVER_COMMAND("reload\n");
	}
}

/*
============
ClientKill

Player entered the suicide command

GLOBALS ASSUMED SET:  g_ulModelIndexPlayer
============
*/
void ClientKill(edict_t* pEntity)
{
	entvars_t* pev = &pEntity->v;

	CBasePlayer* pl = (CBasePlayer*)CBasePlayer::Instance(pev);

	if ( pl->m_fNextSuicideTime > gpGlobals->time )
		return;  // prevent suiciding too ofter

	pl->m_fNextSuicideTime = gpGlobals->time + 1;  // don't let them suicide for 5 seconds after suiciding

	// have the player kill themself
	float oldHealth = pev->health;
	pev->health = 0;
	pl->Killed(pev, GIB_NEVER, DMG_GENERIC, oldHealth, oldHealth);

	// pev->modelindex = g_ulModelIndexPlayer;
	// pev->frags -= 2;		// extra penalty
	// respawn( pev );
}

/*
===========
ClientPutInServer

called each time a player is spawned
============
*/
void ClientPutInServer(edict_t* pEntity)
{
	if ( g_pGameRules )
	{
		g_pGameRules->ClientPutInServer(pEntity);
	}

	CBasePlayer* pPlayer;

	entvars_t* pev = &pEntity->v;

	pPlayer = GetClassPtr<CBasePlayer>(pev);
	pPlayer->SetCustomDecalFrames(-1);  // Assume none;

	// Allocate a CBasePlayer for pev, and call spawn
	pPlayer->Spawn();

	// Reset interpolation during first frame
	pPlayer->pev->effects |= EF_NOINTERP;

	pPlayer->pev->iuser1 = 0;
	pPlayer->pev->iuser2 = 0;
}

#ifndef NO_VOICEGAMEMGR
#include "voice_gamemgr.h"
extern CVoiceGameMgr g_VoiceGameMgr;
#endif

//-----------------------------------------------------------------------------
// Purpose: determine if a uchar32 represents a valid Unicode code point
//-----------------------------------------------------------------------------
bool Q_IsValidUChar32(unsigned int uVal)
{
	// Values > 0x10FFFF are explicitly invalid; ditto for UTF-16 surrogate halves,
	// values ending in FFFE or FFFF, or values in the 0x00FDD0-0x00FDEF reserved range
	return ((uVal - 0x0u) < 0x110000u) && ((uVal - 0x00D800u) > 0x7FFu) && ((uVal & 0xFFFFu) < 0xFFFEu) &&
		((uVal - 0x00FDD0u) > 0x1Fu);
}

// Decode one character from a UTF-8 encoded string. Treats 6-byte CESU-8 sequences
// as a single character, as if they were a correctly-encoded 4-byte UTF-8 sequence.
int Q_UTF8ToUChar32(const char* pUTF8_, unsigned int& uValueOut, bool& bErrorOut)
{
	const unsigned char* pUTF8 = (const unsigned char*)pUTF8_;

	int nBytes = 1;
	unsigned int uValue = pUTF8[0];
	unsigned int uMinValue = 0;

	// 0....... single byte
	if ( uValue < 0x80 )
		goto decodeFinishedNoCheck;

	// Expecting at least a two-byte sequence with 0xC0 <= first <= 0xF7 (110...... and 11110...)
	if ( (uValue - 0xC0u) > 0x37u || (pUTF8[1] & 0xC0) != 0x80 )
		goto decodeError;

	uValue = (uValue << 6) - (0xC0 << 6) + pUTF8[1] - 0x80;
	nBytes = 2;
	uMinValue = 0x80;

	// 110..... two-byte lead byte
	if ( !(uValue & (0x20 << 6)) )
		goto decodeFinished;

	// Expecting at least a three-byte sequence
	if ( (pUTF8[2] & 0xC0) != 0x80 )
		goto decodeError;

	uValue = (uValue << 6) - (0x20 << 12) + pUTF8[2] - 0x80;
	nBytes = 3;
	uMinValue = 0x800;

	// 1110.... three-byte lead byte
decodeFinished:
	if ( uValue >= uMinValue && Q_IsValidUChar32(uValue) )
	{
	decodeFinishedNoCheck:
		uValueOut = uValue;
		bErrorOut = false;
		return nBytes;
	}
decodeError:
	uValueOut = '?';
	bErrorOut = true;
	return nBytes;

	// No idea what's going on here, this function with gotos is an ugly mess.
	// This is supposed to be C++, not assembly...
	// This section was unreachable, so it's been properly taken out.
#if 0
	// Do we have a full UTF-16 surrogate pair that's been UTF-8 encoded afterwards?
	// That is, do we have 0xD800-0xDBFF followed by 0xDC00-0xDFFF? If so, decode it all.
	if ( (uValue - 0xD800u) < 0x400u && pUTF8[3] == 0xED && (unsigned char)(pUTF8[4] - 0xB0) < 0x10 &&
		 (pUTF8[5] & 0xC0) == 0x80 )
	{
		uValue = 0x10000 + ((uValue - 0xD800u) << 10) + ((unsigned char)(pUTF8[4] - 0xB0) << 6) + pUTF8[5] - 0x80;
		nBytes = 6;
		uMinValue = 0x10000;
	}
	goto decodeFinished;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Returns true if UTF-8 string contains invalid sequences.
//-----------------------------------------------------------------------------
bool Q_UnicodeValidate(const char* pUTF8)
{
	bool bError = false;

	if ( !multibyte_only.value )
		return true;

	while ( *pUTF8 )
	{
		unsigned int uVal;
		// Our UTF-8 decoder silently fixes up 6-byte CESU-8 (improperly re-encoded UTF-16) sequences.
		// However, these are technically not valid UTF-8. So if we eat 6 bytes at once, it's an error.
		int nCharSize = Q_UTF8ToUChar32(pUTF8, uVal, bError);
		if ( bError || nCharSize == 6 )
			return false;
		pUTF8 += nCharSize;
	}
	return true;
}

//// HOST_SAY
// String comes in as
// say blah blah blah
// or as
// blah blah blah
//
void Host_Say(edict_t* pEntity, int teamonly)
{
	CBasePlayer* client;
	int j;
	char* p;
	char text[128];
	char szTemp[256];
	const char* cpSay = "say";
	const char* cpSayTeam = "say_team";
	const char* pcmd = CMD_ARGV(0);

	// We can get a raw string now, without the "say " prepended
	if ( CMD_ARGC() == 0 )
		return;

	entvars_t* pev = &pEntity->v;
	CBasePlayer* player = GetClassPtr<CBasePlayer>(pev);

	// Not yet.
	if ( player->m_flNextChatTime > gpGlobals->time )
		return;

	if ( !PlatformLib_StrCaseCmp(pcmd, cpSay) || !PlatformLib_StrCaseCmp(pcmd, cpSayTeam) )
	{
		if ( CMD_ARGC() >= 2 )
		{
			p = (char*)CMD_ARGS();
		}
		else
		{
			// say with a blank message, nothing to do
			return;
		}
	}
	else  // Raw text, need to prepend argv[0]
	{
		if ( CMD_ARGC() >= 2 )
		{
			PlatformLib_SNPrintF(szTemp, sizeof(szTemp), "%s %s", (char*)pcmd, (char*)CMD_ARGS());
		}
		else
		{
			// Just a one word command, use the first word...sigh
			PlatformLib_SNPrintF(szTemp, sizeof(szTemp), "%s", (char*)pcmd);
		}
		p = szTemp;
	}

	// remove quotes if present
	if ( p && *p == '"' )
	{
		p++;
		p[strlen(p) - 1] = 0;
	}

	if ( !p || !p[0] || !Q_UnicodeValidate(p) )
		return;  // no character found, so say nothing

	// turn on color set 2  (color on,  no sound)
	if ( player->IsObserver() && (teamonly) )
	{
		PlatformLib_SNPrintF(text, sizeof(text), "%c(SPEC) %s: ", 2, STRING(pEntity->v.netname));
	}
	else if ( teamonly )
	{
		PlatformLib_SNPrintF(text, sizeof(text), "%c(TEAM) %s: ", 2, STRING(pEntity->v.netname));
	}
	else
	{
		PlatformLib_SNPrintF(text, sizeof(text), "%c%s: ", 2, STRING(pEntity->v.netname));
	}

	j = static_cast<int>(sizeof(text) - 2 - strlen(text));  // -2 for /n and null terminator
	if ( (int)strlen(p) > j )
		p[j] = 0;

	PlatformLib_StrCat(text, sizeof(text), p);
	PlatformLib_StrCat(text, sizeof(text), "\n");

	player->m_flNextChatTime = gpGlobals->time + CHAT_INTERVAL;

	// loop through all players
	// Start with the first player.
	// This may return the world in single player if the client types something between levels or during spawn
	// so check it, or it will infinite loop

	client = NULL;
	while ( ((client = (CBasePlayer*)UTIL_FindEntityByClassname(client, "player")) != NULL) &&
			(!FNullEnt(client->edict())) )
	{
		if ( !client->pev )
			continue;

		if ( client->edict() == pEntity )
			continue;

		if ( !(client->IsNetClient()) )  // Not a client ? (should never be true)
			continue;
#ifndef NO_VOICEGAMEMGR
		// can the receiver hear the sender? or has he muted him?
		if ( g_VoiceGameMgr.PlayerHasBlockedPlayer(client, player) )
			continue;
#endif
		if ( !player->IsObserver() && teamonly &&
			 g_pGameRules->PlayerRelationship(client, CBaseEntity::Instance(pEntity)) != GR_TEAMMATE )
			continue;

		// Spectators can only talk to other specs
		if ( player->IsObserver() && teamonly )
			if ( !client->IsObserver() )
				continue;

		MESSAGE_BEGIN(MSG_ONE, gmsgSayText, NULL, client->pev);
		WRITE_BYTE(ENTINDEX(pEntity));
		WRITE_STRING(text);
		MESSAGE_END();
	}

	// print to the sending client
	MESSAGE_BEGIN(MSG_ONE, gmsgSayText, NULL, &pEntity->v);
	WRITE_BYTE(ENTINDEX(pEntity));
	WRITE_STRING(text);
	MESSAGE_END();

	// echo to server console
	g_engfuncs.pfnServerPrint(text);

	const char* temp;
	if ( teamonly )
		temp = "say_team";
	else
		temp = "say";

	// team match?
	if ( g_teamplay )
	{
		UTIL_LogPrintf(
			"\"%s<%i><%s><%s>\" %s \"%s\"\n",
			STRING(pEntity->v.netname),
			GETPLAYERUSERID(pEntity),
			GETPLAYERAUTHID(pEntity),
			g_engfuncs.pfnInfoKeyValue(g_engfuncs.pfnGetInfoKeyBuffer(pEntity), "model"),
			temp,
			p
		);
	}
	else
	{
		UTIL_LogPrintf(
			"\"%s<%i><%s><%i>\" %s \"%s\"\n",
			STRING(pEntity->v.netname),
			GETPLAYERUSERID(pEntity),
			GETPLAYERAUTHID(pEntity),
			GETPLAYERUSERID(pEntity),
			temp,
			p
		);
	}
}

/*
===========
ClientCommand
called each time a player uses a "cmd" command
============
*/
extern float g_flWeaponCheat;

// Use CMD_ARGV,  CMD_ARGV, and CMD_ARGC to get pointers the character string command.
void ClientCommand(edict_t* pEntity)
{
	const char* pcmd = CMD_ARGV(0);
	const char* pstr;

	// Is the client spawned yet?
	if ( !pEntity->pvPrivateData )
	{
		return;
	}

	entvars_t* pev = &pEntity->v;
	CBasePlayer* player = GetClassPtr<CBasePlayer>(pev);

	if ( g_ClientCommandRegister.ExecuteCommand(pcmd, player) )
	{
		// Command was handled.
		return;
	}

	// This command is special because it works on a prefix.
	if ( ((pstr = strstr(pcmd, "weapon_")) != NULL) && (pstr == pcmd) )
	{
		player->SelectItem(pcmd);
		return;
	}

	// Allow any unhandled command to be passed to the gamerules.
	if ( g_pGameRules->ClientCommand(player, pcmd) )
	{
		// MenuSelect returns true only if the command is properly handled, so don't print a warning
		return;
	}

	char msg[192];
	PlatformLib_SNPrintF(msg, sizeof(msg), "Unknown command: %s\n", pcmd);

	// tell the user they entered an unknown command
	ClientPrint(pev, HUD_PRINTCONSOLE, msg);
}

static CharacterGender InferGenderFromModel(char* infobuffer)
{
	const char* modelName = g_engfuncs.pfnInfoKeyValue(infobuffer, "model");

	if ( !modelName || !(*modelName) )
	{
		return CharacterGender::MALE;
	}

	int modelIndex = CGameResources::StaticInstance().ModelIndex(CUtlString(modelName));

	if ( modelIndex < 0 )
	{
		return CharacterGender::MALE;
	}

	int numTags = g_engfuncs.pfnModelGetNumTags(modelIndex);

	for ( int tagIndex = 0; tagIndex < numTags; ++tagIndex )
	{
		char tag[32];
		g_engfuncs.pfnModelGetTag(modelIndex, tagIndex, tag, sizeof(tag));

		// TODO: Define this tag name somewhere?
		if ( strcmp(tag, "female_character") == 0 )
		{
			return CharacterGender::FEMALE;
		}
	}

	return CharacterGender::MALE;
}

/*
========================
ClientUserInfoChanged

called after the player changes
userinfo - gives dll a chance to modify it before
it gets sent into the rest of the engine.
========================
*/
void ClientUserInfoChanged(edict_t* pEntity, char* infobuffer)
{
	// Is the client spawned yet?
	if ( !pEntity->pvPrivateData )
		return;

	// msg everyone if someone changes their name,  and it isn't the first time (changing no name to current name)
	if ( pEntity->v.netname && (STRING(pEntity->v.netname))[0] != 0 &&
		 !FStrEq(STRING(pEntity->v.netname), g_engfuncs.pfnInfoKeyValue(infobuffer, "name")) )
	{
		char sName[256];
		const char* pName = g_engfuncs.pfnInfoKeyValue(infobuffer, "name");
		PlatformLib_SNPrintF(sName, sizeof(sName), pName, sizeof(sName) - 1);
		sName[sizeof(sName) - 1] = '\0';

		// First parse the name and remove any %'s
		for ( char* chPercent = sName; chPercent != NULL && *chPercent != 0; ++chPercent )
		{
			// Replace it with a space
			if ( *chPercent == '%' )
			{
				*chPercent = ' ';
			}
		}

		// Set the name
		g_engfuncs.pfnSetClientKeyValue(ENTINDEX(pEntity), infobuffer, "name", sName);

		if ( gpGlobals->maxClients > 1 )
		{
			char text[256];
			PlatformLib_SNPrintF(
				text,
				256,
				"* %s changed name to %s\n",
				STRING(pEntity->v.netname),
				g_engfuncs.pfnInfoKeyValue(infobuffer, "name")
			);
			MESSAGE_BEGIN(MSG_ALL, gmsgSayText, NULL);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_STRING(text);
			MESSAGE_END();
		}

		// team match?
		if ( g_teamplay )
		{
			UTIL_LogPrintf(
				"\"%s<%i><%s><%s>\" changed name to \"%s\"\n",
				STRING(pEntity->v.netname),
				GETPLAYERUSERID(pEntity),
				GETPLAYERAUTHID(pEntity),
				g_engfuncs.pfnInfoKeyValue(infobuffer, "model"),
				g_engfuncs.pfnInfoKeyValue(infobuffer, "name")
			);
		}
		else
		{
			UTIL_LogPrintf(
				"\"%s<%i><%s><%i>\" changed name to \"%s\"\n",
				STRING(pEntity->v.netname),
				GETPLAYERUSERID(pEntity),
				GETPLAYERAUTHID(pEntity),
				GETPLAYERUSERID(pEntity),
				g_engfuncs.pfnInfoKeyValue(infobuffer, "name")
			);
		}
	}

	CBasePlayer* playerEnt = GetClassPtrFromEdict<CBasePlayer>(pEntity);
	playerEnt->m_Gender = InferGenderFromModel(infobuffer);

	g_pGameRules->ClientUserInfoChanged(GetClassPtrFromEdict<CBasePlayer>(pEntity), infobuffer);
}

static int g_serveractive = 0;

void ServerDeactivate(void)
{
	// ALERT( at_console, "ServerDeactivate()\n" );

	// It's possible that the engine will call this function more times than is necessary
	//  Therefore, only run it one time for each call to ServerActivate
	if ( g_serveractive != 1 )
	{
		return;
	}

	g_serveractive = 0;

	if ( g_pGameRules )
	{
		g_pGameRules->ServerDeactivate();
	}

	GameplaySystems::NotifyServerDeactivated();
	GameplaySystems::Destroy();

	g_ClientCommandRegister.Clear();
}

static void RegisterClientCommands()
{
	g_ClientCommandRegister.AddCommand(
		"say",
		[](CBasePlayer* player)
		{
			Host_Say(player->edict(), 0);
		}
	);

	g_ClientCommandRegister.AddCommand(
		"say_team",
		[](CBasePlayer* player)
		{
			Host_Say(player->edict(), 1);
		}
	);

	g_ClientCommandRegister.AddCommand(
		"fullupdate",
		[](CBasePlayer* player)
		{
			player->ForceClientDllUpdate();
		}
	);

	g_ClientCommandRegister.AddCommand(
		"give",
		[](CBasePlayer* player)
		{
			if ( g_flWeaponCheat != 0.0 )
			{
				int iszItem = ALLOC_STRING(CMD_ARGV(1));  // Make a copy of the classname
				player->GiveNamedItem(STRING(iszItem));
			}
		}
	);

	g_ClientCommandRegister.AddCommand(
		"fire",
		[](CBasePlayer* player)
		{
			if ( g_flWeaponCheat != 0.0 )
			{
				if ( CMD_ARGC() > 1 )
				{
					FireTargets(CMD_ARGV(1), player, player, USE_TOGGLE, 0);
				}
				else
				{
					entvars_t* pev = &player->edict()->v;
					TraceResult tr;
					UTIL_MakeVectors(pev->v_angle);
					UTIL_TraceLine(
						Vector(pev->origin) + Vector(pev->view_ofs),
						Vector(pev->origin) + Vector(pev->view_ofs) + Vector(gpGlobals->v_forward) * 1000,
						dont_ignore_monsters,
						player->edict(),
						&tr
					);

					if ( tr.pHit )
					{
						CBaseEntity* pHitEnt = CBaseEntity::Instance(tr.pHit);
						if ( pHitEnt )
						{
							pHitEnt->Use(player, player, USE_TOGGLE, 0);
							ClientPrint(
								pev,
								HUD_PRINTCONSOLE,
								UTIL_VarArgs(
									"Fired %s \"%s\"\n",
									STRING(pHitEnt->pev->classname),
									STRING(pHitEnt->pev->targetname)
								)
							);
						}
					}
				}
			}
		}
	);

	g_ClientCommandRegister.AddCommand(
		"drop",
		[](CBasePlayer* player)
		{
			player->DropPlayerItem(CMD_ARGV(1));
		}
	);

	g_ClientCommandRegister.AddCommand(
		"fov",
		[](CBasePlayer* player)
		{
			if ( g_flWeaponCheat && CMD_ARGC() > 1 )
			{
				player->m_iFOV = atoi(CMD_ARGV(1));
			}
			else
			{
				CLIENT_PRINTF(player->edict(), print_console, UTIL_VarArgs("\"fov\" is \"%d\"\n", (int)player->m_iFOV));
			}
		}
	);

	g_ClientCommandRegister.AddCommand(
		"fov",
		[](CBasePlayer* player)
		{
			player->SelectItem(CMD_ARGV(1));
		}
	);

	g_ClientCommandRegister.AddCommand(
		"lastinv",
		[](CBasePlayer* player)
		{
			player->SelectLastItem();
		}
	);

	g_ClientCommandRegister.AddCommand(
		"spectate",
		[](CBasePlayer* player)
		{
			entvars_t* pev = &player->edict()->v;

			if ( !player->IsObserver() )
			{
				// always allow proxies to become a spectator
				if ( (pev->flags & FL_PROXY) || allow_spectators.value )
				{
					edict_t* pentSpawnSpot = g_pGameRules->GetPlayerSpawnSpot(player);
					player->StartObserver(pev->origin, VARS(pentSpawnSpot)->angles);

					// notify other clients of player switching to spectator mode
					UTIL_ClientPrintAll(
						HUD_PRINTNOTIFY,
						UTIL_VarArgs(
							"%s switched to spectator mode\n",
							(pev->netname && (STRING(pev->netname))[0] != 0) ? STRING(pev->netname) : "unconnected"
						)
					);
				}
				else
				{
					ClientPrint(pev, HUD_PRINTCONSOLE, "Spectator mode is disabled.\n");
				}
			}
			else
			{
				player->StopObserver();

				// notify other clients of player left spectators
				UTIL_ClientPrintAll(
					HUD_PRINTNOTIFY,
					UTIL_VarArgs(
						"%s has left spectator mode\n",
						(pev->netname && (STRING(pev->netname))[0] != 0) ? STRING(pev->netname) : "unconnected"
					)
				);
			}
		}
	);

	g_ClientCommandRegister.AddCommand(
		"specmode",
		[](CBasePlayer* player)
		{
			if ( player->IsObserver() )
			{
				player->Observer_SetMode(atoi(CMD_ARGV(1)));
			}
		}
	);

	g_ClientCommandRegister.AddCommand(
		"closemenus",
		[](CBasePlayer*)
		{
			// just ignore it
		}
	);

	g_ClientCommandRegister.AddCommand(
		"follownext",
		[](CBasePlayer* player)
		{
			if ( player->IsObserver() )
			{
				player->Observer_FindNextPlayer(atoi(CMD_ARGV(1)) ? true : false);
			}
		}
	);

	g_ClientCommandRegister.AddCommand(
		"VModEnable",
		[](CBasePlayer*)
		{
			// clear 'Unknown command: VModEnable' in singleplayer
		}
	);
}

void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	int i;
	CBaseEntity* pClass;

	RegisterClientCommands();

	// ALERT( at_console, "ServerActivate()\n" );

	// Every call to ServerActivate should be matched by a call to ServerDeactivate
	g_serveractive = 1;

	// Clients have not been initialized yet
	for ( i = 0; i < edictCount; i++ )
	{
		if ( pEdictList[i].free )
		{
			continue;
		}

		// Clients aren't necessarily initialized until ClientPutInServer()
		if ( i < clientMax || !pEdictList[i].pvPrivateData )
		{
			continue;
		}

		pClass = CBaseEntity::Instance(&pEdictList[i]);
		// Activate this entity if it's got a class & isn't dormant
		if ( pClass && !(pClass->pev->flags & FL_DORMANT) )
		{
			pClass->Activate();
		}
		else
		{
			ALERT(at_console, "Can't instance %s\n", STRING(pEdictList[i].v.classname));
		}
	}

	// Link user messages here to make sure first client can get them...
	LinkUserMessages();

	if ( g_pGameRules )
	{
		GameplaySystems::Create(g_pGameRules->IsMultiplayer());
		GameplaySystems::NotifyServerActivated();

		g_pGameRules->ServerActivate();
	}
}

/*
================
PlayerPreThink

Called every frame before physics are run
================
*/
void PlayerPreThink(edict_t* pEntity)
{
	// ALERT( at_console, "PreThink( %g, frametime %g )\n", gpGlobals->time, gpGlobals->frametime );

	CBasePlayer* pPlayer = (CBasePlayer*)GET_PRIVATE(pEntity);

	if ( pPlayer )
	{
		pPlayer->PreThink();
	}
}

/*
================
PlayerPostThink

Called every frame after physics are run
================
*/
void PlayerPostThink(edict_t* pEntity)
{
	// ALERT( at_console, "PostThink( %g, frametime %g )\n", gpGlobals->time, gpGlobals->frametime );

	CBasePlayer* pPlayer = (CBasePlayer*)GET_PRIVATE(pEntity);

	if ( pPlayer )
	{
		pPlayer->PostThink();
	}
}

void ParmsNewLevel(void)
{
}

void ParmsChangeLevel(void)
{
	// retrieve the pointer to the save data
	SAVERESTOREDATA* pSaveData = (SAVERESTOREDATA*)gpGlobals->pSaveData;

	if ( pSaveData )
	{
		pSaveData->connectionCount = BuildChangeList(pSaveData->levelList, MAX_LEVEL_CONNECTIONS);
	}
}

//
// GLOBALS ASSUMED SET:  g_ulFrameCount
//
void StartFrame(void)
{
	// ALERT( at_console, "SV_Physics( %g, frametime %g )\n", gpGlobals->time, gpGlobals->frametime );

	if ( g_pGameRules )
	{
		g_pGameRules->Think();
	}

	if ( g_fGameOver )
	{
		return;
	}

	gpGlobals->teamplay = teamplay.value;
	g_ulFrameCount++;

	int oldBhopcap = g_bhopcap;
	g_bhopcap = (g_pGameRules->IsMultiplayer() && bhopcap.value != 0.0f) ? 1 : 0;
	if ( g_bhopcap != oldBhopcap )
	{
		MESSAGE_BEGIN(MSG_ALL, gmsgBhopcap, NULL);
		WRITE_BYTE(g_bhopcap);
		MESSAGE_END();
	}
}

void ClientPrecache(void)
{
	// setup precaches always needed
	PRECACHE_SOUND("player/sprayer.wav");  // spray paint sound for PreAlpha

	PRECACHE_SOUND(SOUND_FLASHLIGHT_ON);
	PRECACHE_SOUND(SOUND_FLASHLIGHT_OFF);

	// player gib sounds
	PRECACHE_SOUND("common/bodysplat.wav");

	PRECACHE_MODEL(PLAYER_MODEL_PATH);

	// hud sounds
	PRECACHE_SOUND("common/wpn_hudoff.wav");
	PRECACHE_SOUND("common/wpn_hudon.wav");
	PRECACHE_SOUND("common/wpn_moveselect.wav");
	PRECACHE_SOUND("common/wpn_select.wav");
	PRECACHE_SOUND("common/wpn_denyselect.wav");

	// geiger sounds
	PRECACHE_SOUND("player/geiger6.wav");
	PRECACHE_SOUND("player/geiger5.wav");
	PRECACHE_SOUND("player/geiger4.wav");
	PRECACHE_SOUND("player/geiger3.wav");
	PRECACHE_SOUND("player/geiger2.wav");
	PRECACHE_SOUND("player/geiger1.wav");
}

/*
===============
GetGameDescription

Returns the descriptive name of this .dll.  E.g., Half-Life, or Team Fortress 2
===============
*/
const char* GetGameDescription()
{
	if ( g_pGameRules )  // this function may be called before the world has spawned, and the game rules initialized
	{
		return g_pGameRules->GetGameDescription();
	}
	else
	{
		return "Nightfire Open";
	}
}

/*
================
Sys_Error

Engine is going to shut down, allows setting a breakpoint in game .dll to catch that occasion
================
*/
void Sys_Error(const char*)
{
	// Default case, do nothing.  MOD AUTHORS:  Add code ( e.g., _asm { int 3 }; here to cause a breakpoint for
	// debugging your game .dlls
}

/*
================
PlayerCustomization

A new player customization has been registered on the server
UNDONE:  This only sets the # of frames of the spray can logo
animation right now.
================
*/
void PlayerCustomization(edict_t* pEntity, customization_t* pCust)
{
	CBasePlayer* pPlayer = (CBasePlayer*)GET_PRIVATE(pEntity);

	if ( !pPlayer )
	{
		ALERT(at_console, "PlayerCustomization:  Couldn't get player!\n");
		return;
	}

	if ( !pCust )
	{
		ALERT(at_console, "PlayerCustomization:  NULL customization!\n");
		return;
	}

	switch ( pCust->resource.type )
	{
		case t_decal:
			pPlayer->SetCustomDecalFrames(pCust->nUserData2);  // Second int is max # of frames.
			break;
		case t_sound:
		case t_skin:
		case t_model:
			// Ignore for now.
			break;
		default:
			ALERT(at_console, "PlayerCustomization:  Unknown customization type!\n");
			break;
	}
}

/*
================
SpectatorConnect

A spectator has joined the game
================
*/
void SpectatorConnect(edict_t* pEntity)
{
	CBaseSpectator* pPlayer = (CBaseSpectator*)GET_PRIVATE(pEntity);

	if ( pPlayer )
		pPlayer->SpectatorConnect();
}

/*
================
SpectatorConnect

A spectator has left the game
================
*/
void SpectatorDisconnect(edict_t* pEntity)
{
	CBaseSpectator* pPlayer = (CBaseSpectator*)GET_PRIVATE(pEntity);

	if ( pPlayer )
		pPlayer->SpectatorDisconnect();
}

/*
================
SpectatorConnect

A spectator has sent a usercmd
================
*/
void SpectatorThink(edict_t* pEntity)
{
	CBaseSpectator* pPlayer = (CBaseSpectator*)GET_PRIVATE(pEntity);

	if ( pPlayer )
		pPlayer->SpectatorThink();
}

////////////////////////////////////////////////////////
// PAS and PVS routines for client messaging
//

/*
================
SetupVisibility

A client can have a separate "view entity" indicating that his/her view should depend on the origin of that
view entity.  If that's the case, then pViewEntity will be non-NULL and will be used.  Otherwise, the current
entity's origin is used.  Either is offset by the view_ofs to get the eye position.

From the eye position, we set up the PAS and PVS to use for filtering network messages to the client.  At this point, we
could override the actual PAS or PVS values, or use a different origin.

NOTE:  Do not cache the values of pas and pvs, as they depend on reusable memory in the engine, they are only good for
this one frame
================
*/
void SetupVisibility(edict_t* pViewEntity, edict_t* pClient, unsigned char** pvs, unsigned char** pas)
{
	Vector org;
	edict_t* pView = pClient;

	// Find the client's PVS
	if ( pViewEntity )
	{
		pView = pViewEntity;
	}

	if ( pClient->v.flags & FL_PROXY )
	{
		*pvs = NULL;  // the spectator proxy sees
		*pas = NULL;  // and hears everything
		return;
	}

	if ( pView->v.effects & EF_MERGE_VISIBILITY )
	{
		if ( FClassnameIs(pView, "env_sky") )
		{
			org = pView->v.origin;
		}
		else
		{
			return;  // don't merge pvs
		}
	}
	else
	{
		org = Vector(pView->v.origin) + Vector(pView->v.view_ofs);

		if ( pView->v.flags & FL_DUCKING )
		{
			org = org + (VEC_HULL_MIN - VEC_DUCK_HULL_MIN);
		}
	}

	*pvs = ENGINE_SET_PVS((float*)&org);
	*pas = ENGINE_SET_PAS((float*)&org);
}

#include "EnginePublicAPI/entity_state.h"

/*
AddToFullPack

Return 1 if the entity state has been filled in for the ent and the entity will be propagated to the client, 0 otherwise

state is the server maintained copy of the state info that is transmitted to the client
a MOD could alter values copied into state to send the "host" a different look for a particular entity update, etc.
e and ent are the entity that is being added to the update, if 1 is returned
host is the player's edict of the player whom we are sending the update to
player is 1 if the ent/e is a player and 0 otherwise
pSet is either the PAS or PVS that we previous set up.  We can use it to ask the engine to filter the entity against the
PAS or PVS. we could also use the pas/ pvs that we set in SetupVisibility, if we wanted to.  Caching the value is valid
in that case, but still only for the current frame
*/
int AddToFullPack(
	struct entity_state_s* state,
	int e,
	edict_t* ent,
	edict_t* host,
	int hostflags,
	int player,
	unsigned char* pSet
)
{
	int i;

	// don't send if flagged for NODRAW and it's not the host getting the message
	if ( (ent->v.effects & EF_NODRAW) && (ent != host) )
		return 0;

	// Ignore ents without valid / visible models
	if ( !ent->v.modelindex || !STRING(ent->v.model) )
		return 0;

	// Don't send spectators to other players
	if ( (ent->v.flags & FL_SPECTATOR) && (ent != host) )
	{
		return 0;
	}

	// Ignore if not the host and not touching a PVS/PAS leaf
	// If pSet is NULL, then the test will always succeed and the entity will be added to the update
	if ( ent != host )
	{
		if ( !ENGINE_CHECK_VISIBILITY((const struct edict_s*)ent, pSet) )
		{
			// env_sky is visible always
			if ( !FClassnameIs(ent, "env_sky") )
			{
				return 0;
			}
		}
	}

	// Don't send entity to local client if the client says it's predicting the entity itself.
	if ( ent->v.flags & FL_SKIPLOCALHOST )
	{
		if ( hostflags & 4 )
			return 0;  // it's a portal pass

		if ( (hostflags & 1) && (ent->v.owner == host) )
			return 0;
	}

	if ( host->v.groupinfo )
	{
		UTIL_SetGroupTrace(host->v.groupinfo, GROUP_OP_AND);

		// Should always be set, of course
		if ( ent->v.groupinfo )
		{
			if ( g_groupop == GROUP_OP_AND )
			{
				if ( !(ent->v.groupinfo & host->v.groupinfo) )
					return 0;
			}
			else if ( g_groupop == GROUP_OP_NAND )
			{
				if ( ent->v.groupinfo & host->v.groupinfo )
					return 0;
			}
		}

		UTIL_UnsetGroupTrace();
	}

	*state = entity_state_s {};

	// Assign index so we can track this entity from frame to frame and
	//  delta from it.
	state->number = e;
	state->entityType = ENTITY_NORMAL;

	// Flag custom entities.
	if ( ent->v.flags & FL_CUSTOMENTITY )
	{
		state->entityType = ENTITY_BEAM;
	}

	//
	// Copy state data
	//

	// Round animtime to nearest millisecond
	state->animtime = (int)(1000.0f * ent->v.animtime) / 1000.0f;

	memcpy(state->origin, ent->v.origin, 3 * sizeof(float));
	memcpy(state->angles, ent->v.angles, 3 * sizeof(float));
	memcpy(state->mins, ent->v.mins, 3 * sizeof(float));
	memcpy(state->maxs, ent->v.maxs, 3 * sizeof(float));

	memcpy(state->startpos, ent->v.startpos, 3 * sizeof(float));
	memcpy(state->endpos, ent->v.endpos, 3 * sizeof(float));
	memcpy(state->velocity, ent->v.velocity, 3 * sizeof(float));

	state->impacttime = ent->v.impacttime;
	state->starttime = ent->v.starttime;

	state->modelindex = ent->v.modelindex;

	state->frame = ent->v.frame;

	state->skin = static_cast<short>(ent->v.skin);
	state->effects = ent->v.effects;

	// This non-player entity is being moved by the game .dll and not the physics simulation system
	//  make sure that we interpolate it's position on the client if it moves
	if ( !player && ent->v.animtime && ent->v.velocity[0] == 0 && ent->v.velocity[1] == 0 && ent->v.velocity[2] == 0 )
	{
		state->eflags |= EFLAG_SLERP;
	}

	state->scale = ent->v.scale;
	state->solid = static_cast<short>(ent->v.solid);
	state->colormap = ent->v.colormap;

	state->movetype = ent->v.movetype;
	state->sequence = ent->v.sequence;
	state->framerate = ent->v.framerate;
	state->body = ent->v.body;

	for ( i = 0; i < 4; i++ )
	{
		state->controller[i] = ent->v.controller[i];
	}

	for ( i = 0; i < 2; i++ )
	{
		state->blending[i] = ent->v.blending[i];
	}

	state->rendermode = ent->v.rendermode;
	state->renderamt = (int)ent->v.renderamt;
	state->renderfx = ent->v.renderfx;
	state->rendercolor.r = (byte)ent->v.rendercolor[0];
	state->rendercolor.g = (byte)ent->v.rendercolor[1];
	state->rendercolor.b = (byte)ent->v.rendercolor[2];

	state->aiment = 0;
	if ( ent->v.aiment )
	{
		state->aiment = ENTINDEX(ent->v.aiment);
	}

	state->owner = 0;
	if ( ent->v.owner )
	{
		int owner = ENTINDEX(ent->v.owner);

		// Only care if owned by a player
		if ( owner >= 1 && owner <= gpGlobals->maxClients )
		{
			state->owner = owner;
		}
	}

	state->onground = 0;
	if ( ent->v.groundentity )
	{
		state->onground = ENTINDEX(ent->v.groundentity);
	}

	// HACK:  Somewhat...
	// Class is overridden for non-players to signify a breakable glass object ( sort of a class? )
	if ( !player )
	{
		state->playerclass = ent->v.playerclass;
	}

	// Special stuff for players only
	if ( player )
	{
		memcpy(state->basevelocity, ent->v.basevelocity, 3 * sizeof(float));

		state->weaponmodel = MODEL_INDEX(STRING(ent->v.weaponmodel));
		state->gaitsequence = ent->v.gaitsequence;
		state->spectator = ent->v.flags & FL_SPECTATOR;
		state->friction = ent->v.friction;

		state->gravity = ent->v.gravity;
		// state->team		= ent->v.team;

		state->usehull = (ent->v.flags & FL_DUCKING) ? 1 : 0;
		state->health = (int)ent->v.health;
	}

	return 1;
}

// defaults for clientinfo messages
#define DEFAULT_VIEWHEIGHT 28

/*
===================
CreateBaseline

Creates baselines used for network encoding, especially for player data since players are not spawned until connect
time.
===================
*/
void CreateBaseline(
	int player,
	int eindex,
	struct entity_state_s* baseline,
	struct edict_s* entity,
	int playermodelindex,
	const vec3_t player_mins,
	const vec3_t player_maxs
)
{
	VectorCopy(entity->v.origin, baseline->origin);
	VectorCopy(entity->v.angles, baseline->angles);
	baseline->frame = entity->v.frame;
	baseline->skin = (short)entity->v.skin;

	// render information
	baseline->rendermode = (byte)entity->v.rendermode;
	baseline->renderamt = (byte)entity->v.renderamt;
	baseline->rendercolor.r = (byte)entity->v.rendercolor[0];
	baseline->rendercolor.g = (byte)entity->v.rendercolor[1];
	baseline->rendercolor.b = (byte)entity->v.rendercolor[2];
	baseline->renderfx = (byte)entity->v.renderfx;

	if ( player )
	{
		VectorCopy(player_mins, baseline->mins);
		VectorCopy(player_maxs, baseline->maxs);

		baseline->colormap = eindex;
		baseline->modelindex = playermodelindex;
		baseline->friction = 1.0;
		baseline->movetype = MOVETYPE_WALK;

		baseline->scale = entity->v.scale;
		baseline->solid = SOLID_SLIDEBOX;
		baseline->framerate = 1.0;
		baseline->gravity = 1.0;
	}
	else
	{
		VectorCopy(entity->v.mins, baseline->mins);
		VectorCopy(entity->v.maxs, baseline->maxs);

		baseline->colormap = 0;
		baseline->modelindex = entity->v.modelindex;  // SV_ModelIndex(pr_strings + entity->v.model);
		baseline->movetype = entity->v.movetype;

		baseline->scale = entity->v.scale;
		baseline->solid = static_cast<short>(entity->v.solid);
		baseline->framerate = entity->v.framerate;
		baseline->gravity = entity->v.gravity;
	}
}

typedef struct
{
	char name[32];
	int field;
} entity_field_alias_t;

#define FIELD_ORIGIN0 0
#define FIELD_ORIGIN1 1
#define FIELD_ORIGIN2 2
#define FIELD_ANGLES0 3
#define FIELD_ANGLES1 4
#define FIELD_ANGLES2 5

static entity_field_alias_t entity_field_alias[] = {
	{"origin[0]", 0},
	{"origin[1]", 0},
	{"origin[2]", 0},
	{"angles[0]", 0},
	{"angles[1]", 0},
	{"angles[2]", 0},
};

void Entity_FieldInit(struct delta_s* pFields)
{
	entity_field_alias[FIELD_ORIGIN0].field = DELTA_FINDFIELD(pFields, entity_field_alias[FIELD_ORIGIN0].name);
	entity_field_alias[FIELD_ORIGIN1].field = DELTA_FINDFIELD(pFields, entity_field_alias[FIELD_ORIGIN1].name);
	entity_field_alias[FIELD_ORIGIN2].field = DELTA_FINDFIELD(pFields, entity_field_alias[FIELD_ORIGIN2].name);
	entity_field_alias[FIELD_ANGLES0].field = DELTA_FINDFIELD(pFields, entity_field_alias[FIELD_ANGLES0].name);
	entity_field_alias[FIELD_ANGLES1].field = DELTA_FINDFIELD(pFields, entity_field_alias[FIELD_ANGLES1].name);
	entity_field_alias[FIELD_ANGLES2].field = DELTA_FINDFIELD(pFields, entity_field_alias[FIELD_ANGLES2].name);
}

/*
==================
Entity_Encode

Callback for sending entity_state_t info over network.
FIXME:  Move to script
==================
*/
void Entity_Encode(struct delta_s* pFields, const unsigned char* from, const unsigned char* to)
{
	entity_state_t *f, *t;
	int localplayer = 0;
	static int initialized = 0;

	if ( !initialized )
	{
		Entity_FieldInit(pFields);
		initialized = 1;
	}

	f = (entity_state_t*)from;
	t = (entity_state_t*)to;

	// Never send origin to local player, it's sent with more resolution in clientdata_t structure
	localplayer = (t->number - 1) == ENGINE_CURRENT_PLAYER();
	if ( localplayer )
	{
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN0].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN1].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN2].field);
	}

	if ( (t->impacttime != 0) && (t->starttime != 0) )
	{
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN0].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN1].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN2].field);

		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ANGLES0].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ANGLES1].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ANGLES2].field);
	}

	if ( (t->movetype == MOVETYPE_FOLLOW) && (t->aiment != 0) )
	{
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN0].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN1].field);
		DELTA_UNSETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN2].field);
	}
	else if ( t->aiment != f->aiment )
	{
		DELTA_SETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN0].field);
		DELTA_SETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN1].field);
		DELTA_SETBYINDEX(pFields, entity_field_alias[FIELD_ORIGIN2].field);
	}
}

static entity_field_alias_t player_field_alias[] = {
	{"origin[0]", 0},
	{"origin[1]", 0},
	{"origin[2]", 0},
};

void Player_FieldInit(struct delta_s* pFields)
{
	player_field_alias[FIELD_ORIGIN0].field = DELTA_FINDFIELD(pFields, player_field_alias[FIELD_ORIGIN0].name);
	player_field_alias[FIELD_ORIGIN1].field = DELTA_FINDFIELD(pFields, player_field_alias[FIELD_ORIGIN1].name);
	player_field_alias[FIELD_ORIGIN2].field = DELTA_FINDFIELD(pFields, player_field_alias[FIELD_ORIGIN2].name);
}

/*
==================
Player_Encode

Callback for sending entity_state_t for players info over network.
==================
*/
void Player_Encode(struct delta_s* pFields, const unsigned char* from, const unsigned char* to)
{
	entity_state_t *f, *t;
	int localplayer = 0;
	static int initialized = 0;

	if ( !initialized )
	{
		Player_FieldInit(pFields);
		initialized = 1;
	}

	f = (entity_state_t*)from;
	t = (entity_state_t*)to;

	// Never send origin to local player, it's sent with more resolution in clientdata_t structure
	localplayer = (t->number - 1) == ENGINE_CURRENT_PLAYER();
	if ( localplayer )
	{
		DELTA_UNSETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN0].field);
		DELTA_UNSETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN1].field);
		DELTA_UNSETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN2].field);
	}

	if ( (t->movetype == MOVETYPE_FOLLOW) && (t->aiment != 0) )
	{
		DELTA_UNSETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN0].field);
		DELTA_UNSETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN1].field);
		DELTA_UNSETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN2].field);
	}
	else if ( t->aiment != f->aiment )
	{
		DELTA_SETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN0].field);
		DELTA_SETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN1].field);
		DELTA_SETBYINDEX(pFields, player_field_alias[FIELD_ORIGIN2].field);
	}
}

#define CUSTOMFIELD_ORIGIN0 0
#define CUSTOMFIELD_ORIGIN1 1
#define CUSTOMFIELD_ORIGIN2 2
#define CUSTOMFIELD_ANGLES0 3
#define CUSTOMFIELD_ANGLES1 4
#define CUSTOMFIELD_ANGLES2 5
#define CUSTOMFIELD_SKIN 6
#define CUSTOMFIELD_SEQUENCE 7
#define CUSTOMFIELD_ANIMTIME 8

entity_field_alias_t custom_entity_field_alias[] = {
	{"origin[0]", 0},
	{"origin[1]", 0},
	{"origin[2]", 0},
	{"angles[0]", 0},
	{"angles[1]", 0},
	{"angles[2]", 0},
	{"skin", 0},
	{"sequence", 0},
	{"animtime", 0},
};

void Custom_Entity_FieldInit(struct delta_s* pFields)
{
	custom_entity_field_alias[CUSTOMFIELD_ORIGIN0].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ORIGIN0].name);
	custom_entity_field_alias[CUSTOMFIELD_ORIGIN1].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ORIGIN1].name);
	custom_entity_field_alias[CUSTOMFIELD_ORIGIN2].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ORIGIN2].name);
	custom_entity_field_alias[CUSTOMFIELD_ANGLES0].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ANGLES0].name);
	custom_entity_field_alias[CUSTOMFIELD_ANGLES1].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ANGLES1].name);
	custom_entity_field_alias[CUSTOMFIELD_ANGLES2].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ANGLES2].name);
	custom_entity_field_alias[CUSTOMFIELD_SKIN].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_SKIN].name);
	custom_entity_field_alias[CUSTOMFIELD_SEQUENCE].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_SEQUENCE].name);
	custom_entity_field_alias[CUSTOMFIELD_ANIMTIME].field =
		DELTA_FINDFIELD(pFields, custom_entity_field_alias[CUSTOMFIELD_ANIMTIME].name);
}

/*
==================
Custom_Encode

Callback for sending entity_state_t info ( for custom entities ) over network.
FIXME:  Move to script
==================
*/
void Custom_Encode(struct delta_s* pFields, const unsigned char* from, const unsigned char* to)
{
	entity_state_t *f, *t;
	int beamType;
	static int initialized = 0;

	if ( !initialized )
	{
		Custom_Entity_FieldInit(pFields);
		initialized = 1;
	}

	f = (entity_state_t*)from;
	t = (entity_state_t*)to;

	beamType = t->rendermode & 0x0f;

	if ( beamType != BEAM_POINTS && beamType != BEAM_ENTPOINT )
	{
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ORIGIN0].field);
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ORIGIN1].field);
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ORIGIN2].field);
	}

	if ( beamType != BEAM_POINTS )
	{
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ANGLES0].field);
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ANGLES1].field);
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ANGLES2].field);
	}

	if ( beamType != BEAM_ENTS && beamType != BEAM_ENTPOINT )
	{
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_SKIN].field);
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_SEQUENCE].field);
	}

	// animtime is compared by rounding first
	// see if we really shouldn't actually send it
	if ( (int)f->animtime == (int)t->animtime )
	{
		DELTA_UNSETBYINDEX(pFields, custom_entity_field_alias[CUSTOMFIELD_ANIMTIME].field);
	}
}

/*
=================
RegisterEncoders

Allows game .dll to override network encoding of certain types of entities and tweak values, etc.
=================
*/
void RegisterEncoders(void)
{
	DELTA_ADDENCODER("Entity_Encode", Entity_Encode);
	DELTA_ADDENCODER("Custom_Encode", Custom_Encode);
	DELTA_ADDENCODER("Player_Encode", Player_Encode);
}

int GetWeaponData(struct edict_s* player, struct weapon_data_s* info)
{
#if defined(CLIENT_WEAPONS)
	int i;
	weapon_data_t* item;
	entvars_t* pev = &player->v;
	CBasePlayer* pl = (CBasePlayer*)CBasePlayer::Instance(pev);
	CBasePlayerWeapon* gun;

	memset(info, 0, MAX_LOCAL_WEAPONS * sizeof(weapon_data_t));

	if ( !pl )
		return 1;

	// go through all of the weapons and make a list of the ones to pack
	for ( i = 0; i < MAX_ITEM_TYPES; i++ )
	{
		if ( pl->m_rgpPlayerItems[i] )
		{
			// there's a weapon here. Should I pack it?
			CBasePlayerItem* pPlayerItem = pl->m_rgpPlayerItems[i];

			while ( pPlayerItem )
			{
				gun = (CBasePlayerWeapon*)pPlayerItem->GetWeaponPtr();
				if ( gun && gun->UseDecrement() )
				{
					ItemInfo II = {};
					// Get The ID.
					gun->GetItemInfo(&II);

					if ( II.iId >= 0 && II.iId < MAX_LOCAL_WEAPONS )
					{
						item = &info[II.iId];

						item->m_iId = II.iId;
						item->m_iClip = gun->m_iClip;

						item->m_flTimeWeaponIdle = Q_max(gun->m_flTimeWeaponIdle, -0.001f);
						item->m_flEnqueuedMechanicInvocationTime =
							Q_max(gun->m_flEnqueuedMechanicInvocationTime, -0.001f);
						item->m_flNextPrimaryAttack = Q_max(gun->m_flNextPrimaryAttack, -0.001f);
						item->m_flNextSecondaryAttack = Q_max(gun->m_flNextSecondaryAttack, -0.001f);
						item->m_flLastPrimaryAttack = Q_max(gun->m_flLastPrimaryAttack, -10.0f);
						item->m_flLastSecondaryAttack = Q_max(gun->m_flLastSecondaryAttack, -10.0f);
						item->m_fInReload = gun->m_fInReload;
						item->m_fInSpecialReload = gun->m_fInSpecialReload;

						gun->WritePredictionData(item);
						item->tuser1 = Q_max(item->tuser1, -0.001f);
						item->tuser2 = Q_max(item->tuser2, -0.001f);
						item->tuser3 = Q_max(item->tuser3, -0.001f);
						item->tuser4 = Q_max(item->tuser4, -0.001f);
					}
				}
				pPlayerItem = pPlayerItem->m_pNext;
			}
		}
	}
#else
	memset(info, 0, MAX_LOCAL_WEAPONS * sizeof(weapon_data_t));
#endif
	return 1;
}

/*
=================
UpdateClientData

Data sent to current client only
engine sets cd to 0 before calling.
=================
*/
void UpdateClientData(const struct edict_s* ent, int sendweapons, struct clientdata_s* cd)
{
	if ( !ent || !ent->pvPrivateData )
		return;
	entvars_t* pev = (entvars_t*)&ent->v;
	CBasePlayer* pl = (CBasePlayer*)(CBasePlayer::Instance(pev));
	entvars_t* pevOrg = NULL;

	// if user is spectating different player in First person, override some vars
	if ( pl && pl->pev->iuser1 == OBS_IN_EYE )
	{
		if ( pl->m_hObserverTarget )
		{
			pevOrg = pev;
			pev = pl->m_hObserverTarget->pev;
			pl = (CBasePlayer*)(CBasePlayer::Instance(pev));
		}
	}

	cd->flags = pev->flags;
	cd->health = pev->health;

	cd->viewmodel = MODEL_INDEX(STRING(pev->viewmodel));

	cd->waterlevel = pev->waterlevel;
	cd->watertype = pev->watertype;
	cd->weapons = pev->weapons;

	// Vectors
	VectorCopy(pev->origin, cd->origin);
	VectorCopy(pev->velocity, cd->velocity);
	VectorCopy(pev->view_ofs, cd->view_ofs);
	VectorCopy(pev->punchangle, cd->punchangle);

	cd->bInDuck = pev->bInDuck;
	cd->flTimeStepSound = pev->flTimeStepSound;
	cd->flDuckTime = pev->flDuckTime;
	cd->flSwimTime = pev->flSwimTime;
	cd->waterjumptime = static_cast<int>(pev->teleport_time);

	PlatformLib_SNPrintF(cd->physinfo, sizeof(cd->physinfo), ENGINE_GETPHYSINFO(ent));

	cd->maxspeed = pev->maxspeed;
	cd->fov = pev->fov;
	cd->weaponanim = pev->weaponanim;
	cd->weaponScreenOverlay = pl->m_iWeaponScreenOverlay;
	cd->weaponInaccuracy = pl->m_flWeaponInaccuracy;

	cd->pushmsec = pev->pushmsec;

	// Spectator mode
	if ( pevOrg != NULL )
	{
		// don't use spec vars from chased player
		cd->iuser1 = pevOrg->iuser1;
		cd->iuser2 = pevOrg->iuser2;
	}
	else
	{
		cd->iuser1 = pev->iuser1;
		cd->iuser2 = pev->iuser2;
	}
#if defined(CLIENT_WEAPONS)
	if ( sendweapons )
	{
		if ( pl )
		{
			cd->m_flNextAttack = pl->m_flNextAttack;
			cd->fuser2 = pl->m_flNextAmmoBurn;
			cd->fuser3 = pl->m_flAmmoStartCharge;

			if ( pl->m_pActiveItem )
			{
				CBasePlayerWeapon* gun;
				gun = (CBasePlayerWeapon*)pl->m_pActiveItem->GetWeaponPtr();
				if ( gun && gun->UseDecrement() )
				{
					ItemInfo II = {};
					gun->GetItemInfo(&II);

					cd->m_iId = II.iId;

					cd->vuser3[2] = static_cast<float>(gun->m_iSecondaryAmmoType);
					cd->vuser4[0] = static_cast<float>(gun->m_iPrimaryAmmoType);
					cd->vuser4[1] = static_cast<float>(pl->m_rgAmmo[gun->m_iPrimaryAmmoType]);
					cd->vuser4[2] = static_cast<float>(pl->m_rgAmmo[gun->m_iSecondaryAmmoType]);

#ifdef HL_CONTENT
					if ( pl->m_pActiveItem->m_iId == WEAPON_RPG )
					{
						cd->vuser2[1] = static_cast<float>(((CRpg*)pl->m_pActiveItem)->m_fSpotActive);
						cd->vuser2[2] = static_cast<float>(((CRpg*)pl->m_pActiveItem)->m_cActiveRockets);
					}
#endif  // HL_CONTENT
				}
			}
		}
	}
#endif
}

/*
=================
CmdStart

We're about to run this usercmd for the specified player.  We can set up groupinfo and masking here, etc.
This is the time to examine the usercmd for anything extra.  This call happens even if think does not.
=================
*/
void CmdStart(const edict_t* player, const struct usercmd_s*, unsigned int random_seed)
{
	entvars_t* pev = (entvars_t*)&player->v;
	CBasePlayer* pl = (CBasePlayer*)CBasePlayer::Instance(pev);

	if ( !pl )
		return;

	if ( pl->pev->groupinfo != 0 )
	{
		UTIL_SetGroupTrace(pl->pev->groupinfo, GROUP_OP_AND);
	}

	pl->random_seed = random_seed;
}

/*
=================
CmdEnd

Each cmdstart is exactly matched with a cmd end, clean up any group trace flags, etc. here
=================
*/
void CmdEnd(const edict_t* player)
{
	entvars_t* pev = (entvars_t*)&player->v;
	CBasePlayer* pl = (CBasePlayer*)CBasePlayer::Instance(pev);

	if ( !pl )
		return;
	if ( pl->pev->groupinfo != 0 )
	{
		UTIL_UnsetGroupTrace();
	}
}

/*
================================
ConnectionlessPacket

 Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds
the max size of the response_buffer, so you must zero it out if you choose not to respond.
================================
*/
int ConnectionlessPacket(const struct netadr_s*, const char*, char*, int* response_buffer_size)
{
	// Parse stuff from args
	// int max_buffer_size = *response_buffer_size;

	// Zero it out since we aren't going to respond.
	// If we wanted to response, we'd write data into response_buffer
	*response_buffer_size = 0;

	// Since we don't listen for anything here, just respond that it's a bogus message
	// If we didn't reject the message, we'd return 1 for success instead.
	return 0;
}

/*
================================
GetHullBounds

  Engine calls this to enumerate player collision hulls, for prediction.  Return 0 if the hullnumber doesn't exist.
================================
*/
int GetHullBounds(int hullnumber, float* mins, float* maxs)
{
	int iret = 0;

	switch ( hullnumber )
	{
		case 0:  // Normal player
			VectorCopy(VEC_HULL_MIN, mins);
			VectorCopy(VEC_HULL_MAX, maxs);
			iret = 1;
			break;
		case 1:  // Crouched player
			VectorCopy(VEC_DUCK_HULL_MIN, mins);
			VectorCopy(VEC_DUCK_HULL_MAX, maxs);
			iret = 1;
			break;
		case 2:  // Point based hull
			VectorClear(mins);
			VectorClear(maxs);
			iret = 1;
			break;
	}

	return iret;
}

/*
================================
CreateInstancedBaselines

Create pseudo-baselines for items that aren't placed in the map at spawn time, but which are likely
to be created during play ( e.g., grenades, ammo packs, projectiles, corpses, etc. )
================================
*/
void CreateInstancedBaselines(void)
{
	/*int iret = 0;
	entity_state_t state;

	memset( &state, 0, sizeof(state) );*/

	// Create any additional baselines here for things like grendates, etc.
	// iret = ENGINE_INSTANCE_BASELINE( pc->pev->classname, &state );

	// Destroy objects.
	// UTIL_Remove( pc );
}

/*
================================
InconsistentFile

One of the ENGINE_FORCE_UNMODIFIED files failed the consistency check for the specified player
 Return 0 to allow the client to continue, 1 to force immediate disconnection ( with an optional disconnect message of
up to 256 characters )
================================
*/
int InconsistentFile(const edict_t*, const char* filename, char* disconnect_message, size_t disconnectBufferSize)
{
	// Server doesn't care?
	if ( CVAR_GET_FLOAT("mp_consistency") != 1 )
		return 0;

	// Default behavior is to kick the player
	PlatformLib_SNPrintF(
		disconnect_message,
		disconnectBufferSize,
		"Server is enforcing file consistency for %s\n",
		filename
	);

	// Kick now with specified disconnect message.
	return 1;
}

/*
================================
AllowLagCompensation

 The game .dll should return 1 if lag compensation should be allowed ( could also just set
  the sv_unlag cvar.
 Most games right now should return 0, until client-side weapon prediction code is written
  and tested for them ( note you can predict weapons, but not do lag compensation, too,
  if you want.
================================
*/
int AllowLagCompensation(void)
{
	return 1;
}

const char* GetRandomWaterTransitionSound(void)
{
	return SoundResources::FootstepSounds.RandomResourcePath(FootstepSoundId::DeepWater);
}
