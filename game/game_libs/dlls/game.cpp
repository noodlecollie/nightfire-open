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
#include "extdll.h"
#include "EnginePublicAPI/eiface.h"
#include "util.h"
#include "game.h"
#include "weaponregistry.h"
#include "bot_cvars.h"
#include "bot_commands.h"
#include "projectInterface/IProjectInterface.h"
#include "projectInterface_server.h"
#include "gameresources/GameResources.h"
#include "hitbox_debugging/hitbox_commands.h"
#include "customGeometry/commands.h"
#include "resources/SoundResources.h"
#include "gameplay/weaponInaccuracyCvars.h"

cvar_t displaysoundlist = CONSTRUCT_CVAR_T("displaysoundlist", 0, 0);

// multiplayer server rules
cvar_t fragsleft = CONSTRUCT_CVAR_T(
	"mp_fragsleft",
	0,
	FCVAR_SERVER | FCVAR_UNLOGGED);  // Don't spam console/log files/users with this changing
cvar_t timeleft = CONSTRUCT_CVAR_T("mp_timeleft", 0, FCVAR_SERVER | FCVAR_UNLOGGED);

// multiplayer server rules
cvar_t teamplay = CONSTRUCT_CVAR_T("mp_teamplay", 0, FCVAR_SERVER);
cvar_t fraglimit = CONSTRUCT_CVAR_T("mp_fraglimit", 0, FCVAR_SERVER);
cvar_t timelimit = CONSTRUCT_CVAR_T("mp_timelimit", 0, FCVAR_SERVER);
cvar_t friendlyfire = CONSTRUCT_CVAR_T("mp_friendlyfire", 0, FCVAR_SERVER);
cvar_t falldamage = CONSTRUCT_CVAR_T("mp_falldamage", 1, FCVAR_SERVER);
cvar_t weaponstay = CONSTRUCT_CVAR_T("mp_weaponstay", 0, FCVAR_SERVER);
cvar_t selfgauss = CONSTRUCT_CVAR_T("mp_selfgauss", 1, FCVAR_SERVER);
cvar_t satchelfix = CONSTRUCT_CVAR_T("mp_satchelfix", 0, FCVAR_SERVER);
cvar_t forcerespawn = CONSTRUCT_CVAR_T("mp_forcerespawn", 1, FCVAR_SERVER);
cvar_t flashlight = CONSTRUCT_CVAR_T("mp_flashlight", 0, FCVAR_SERVER);
cvar_t aimcrosshair = CONSTRUCT_CVAR_T("mp_autocrosshair", 1, FCVAR_SERVER);
cvar_t decalfrequency = CONSTRUCT_CVAR_T("decalfrequency", 30, FCVAR_SERVER);
cvar_t teamlist = {(char*)"mp_teamlist", (char*)"mi6;phoenix", FCVAR_SERVER, 0.0f, nullptr};
cvar_t teamoverride = CONSTRUCT_CVAR_T("mp_teamoverride", 1, 0);
cvar_t defaultteam = CONSTRUCT_CVAR_T("mp_defaultteam", 0, 0);
cvar_t allowmonsters = CONSTRUCT_CVAR_T("mp_allowmonsters", 0, FCVAR_SERVER);
cvar_t bhopcap = CONSTRUCT_CVAR_T("mp_bhopcap", 1, FCVAR_SERVER);

// Avoid respawning players closer to where they last died than this.
cvar_t mp_respawn_avoid_radius = CONSTRUCT_CVAR_T("mp_respawn_avoid_radius", 192, FCVAR_SERVER);
cvar_t mp_corpse_show_time = CONSTRUCT_CVAR_T("mp_corpse_show_time", 30, FCVAR_SERVER);

cvar_t allow_spectators =
	CONSTRUCT_CVAR_T("allow_spectators", 1, FCVAR_SERVER);  // 0 prevents players from being spectators
cvar_t multibyte_only = CONSTRUCT_CVAR_T("mp_multibyte_only", 0, FCVAR_SERVER);

cvar_t mp_chattime = CONSTRUCT_CVAR_T("mp_chattime", 10, FCVAR_SERVER);

// Engine Cvars
cvar_t* g_psv_gravity = NULL;
cvar_t* g_psv_aim = NULL;
cvar_t* g_footsteps = NULL;

// CVARS FOR SKILL LEVEL SETTINGS

// Turret
cvar_t sk_turret_health1 = CONSTRUCT_CVAR_T("sk_turret_health1", 0, 0);
cvar_t sk_turret_health2 = CONSTRUCT_CVAR_T("sk_turret_health2", 0, 0);
cvar_t sk_turret_health3 = CONSTRUCT_CVAR_T("sk_turret_health3", 0, 0);

// MiniTurret
cvar_t sk_miniturret_health1 = CONSTRUCT_CVAR_T("sk_miniturret_health1", 0, 0);
cvar_t sk_miniturret_health2 = CONSTRUCT_CVAR_T("sk_miniturret_health2", 0, 0);
cvar_t sk_miniturret_health3 = CONSTRUCT_CVAR_T("sk_miniturret_health3", 0, 0);

// Sentry Turret
cvar_t sk_sentry_health1 = CONSTRUCT_CVAR_T("sk_sentry_health1", 0, 0);
cvar_t sk_sentry_health2 = CONSTRUCT_CVAR_T("sk_sentry_health2", 0, 0);
cvar_t sk_sentry_health3 = CONSTRUCT_CVAR_T("sk_sentry_health3", 0, 0);

// HEALTH/CHARGE
cvar_t sk_suitcharger1 = CONSTRUCT_CVAR_T("sk_suitcharger1", 0, 0);
cvar_t sk_suitcharger2 = CONSTRUCT_CVAR_T("sk_suitcharger2", 0, 0);
cvar_t sk_suitcharger3 = CONSTRUCT_CVAR_T("sk_suitcharger3", 0, 0);

cvar_t sk_battery1 = CONSTRUCT_CVAR_T("sk_battery1", 0, 0);
cvar_t sk_battery2 = CONSTRUCT_CVAR_T("sk_battery2", 0, 0);
cvar_t sk_battery3 = CONSTRUCT_CVAR_T("sk_battery3", 0, 0);

cvar_t sk_healthcharger1 = CONSTRUCT_CVAR_T("sk_healthcharger1", 0, 0);
cvar_t sk_healthcharger2 = CONSTRUCT_CVAR_T("sk_healthcharger2", 0, 0);
cvar_t sk_healthcharger3 = CONSTRUCT_CVAR_T("sk_healthcharger3", 0, 0);

cvar_t sk_healthkit1 = CONSTRUCT_CVAR_T("sk_healthkit1", 0, 0);
cvar_t sk_healthkit2 = CONSTRUCT_CVAR_T("sk_healthkit2", 0, 0);
cvar_t sk_healthkit3 = CONSTRUCT_CVAR_T("sk_healthkit3", 0, 0);

cvar_t sk_scientist_heal1 = CONSTRUCT_CVAR_T("sk_scientist_heal1", 0, 0);
cvar_t sk_scientist_heal2 = CONSTRUCT_CVAR_T("sk_scientist_heal2", 0, 0);
cvar_t sk_scientist_heal3 = CONSTRUCT_CVAR_T("sk_scientist_heal3", 0, 0);

// monster damage adjusters
cvar_t sk_monster_head1 = CONSTRUCT_CVAR_T("sk_monster_head1", 2, 0);
cvar_t sk_monster_head2 = CONSTRUCT_CVAR_T("sk_monster_head2", 2, 0);
cvar_t sk_monster_head3 = CONSTRUCT_CVAR_T("sk_monster_head3", 2, 0);

cvar_t sk_monster_chest1 = CONSTRUCT_CVAR_T("sk_monster_chest1", 1, 0);
cvar_t sk_monster_chest2 = CONSTRUCT_CVAR_T("sk_monster_chest2", 1, 0);
cvar_t sk_monster_chest3 = CONSTRUCT_CVAR_T("sk_monster_chest3", 1, 0);

cvar_t sk_monster_stomach1 = CONSTRUCT_CVAR_T("sk_monster_stomach1", 1, 0);
cvar_t sk_monster_stomach2 = CONSTRUCT_CVAR_T("sk_monster_stomach2", 1, 0);
cvar_t sk_monster_stomach3 = CONSTRUCT_CVAR_T("sk_monster_stomach3", 1, 0);

cvar_t sk_monster_arm1 = CONSTRUCT_CVAR_T("sk_monster_arm1", 1, 0);
cvar_t sk_monster_arm2 = CONSTRUCT_CVAR_T("sk_monster_arm2", 1, 0);
cvar_t sk_monster_arm3 = CONSTRUCT_CVAR_T("sk_monster_arm3", 1, 0);

cvar_t sk_monster_leg1 = CONSTRUCT_CVAR_T("sk_monster_leg1", 1, 0);
cvar_t sk_monster_leg2 = CONSTRUCT_CVAR_T("sk_monster_leg2", 1, 0);
cvar_t sk_monster_leg3 = CONSTRUCT_CVAR_T("sk_monster_leg3", 1, 0);

// player damage adjusters
cvar_t sk_player_head1 = CONSTRUCT_CVAR_T("sk_player_head1", 2, 0);
cvar_t sk_player_head2 = CONSTRUCT_CVAR_T("sk_player_head2", 2, 0);
cvar_t sk_player_head3 = CONSTRUCT_CVAR_T("sk_player_head3", 2, 0);

cvar_t sk_player_chest1 = CONSTRUCT_CVAR_T("sk_player_chest1", 1, 0);
cvar_t sk_player_chest2 = CONSTRUCT_CVAR_T("sk_player_chest2", 1, 0);
cvar_t sk_player_chest3 = CONSTRUCT_CVAR_T("sk_player_chest3", 1, 0);

cvar_t sk_player_stomach1 = CONSTRUCT_CVAR_T("sk_player_stomach1", 1, 0);
cvar_t sk_player_stomach2 = CONSTRUCT_CVAR_T("sk_player_stomach2", 1, 0);
cvar_t sk_player_stomach3 = CONSTRUCT_CVAR_T("sk_player_stomach3", 1, 0);

cvar_t sk_player_arm1 = CONSTRUCT_CVAR_T("sk_player_arm1", 1, 0);
cvar_t sk_player_arm2 = CONSTRUCT_CVAR_T("sk_player_arm2", 1, 0);
cvar_t sk_player_arm3 = CONSTRUCT_CVAR_T("sk_player_arm3", 1, 0);

cvar_t sk_player_leg1 = CONSTRUCT_CVAR_T("sk_player_leg1", 1, 0);
cvar_t sk_player_leg2 = CONSTRUCT_CVAR_T("sk_player_leg2", 1, 0);
cvar_t sk_player_leg3 = CONSTRUCT_CVAR_T("sk_player_leg3", 1, 0);

// END Cvars for Skill Level settings

// Register your console variables here
// This gets called one time when the game is initialied
void GameDLLInit(void)
{
	IProjectInterface::SetProjectInterfaceImpl(ProjectInterface_Server::StaticInstance());
	CGameResources::StaticInstance().Initialise();

	// Register cvars here:

	g_psv_gravity = CVAR_GET_POINTER("sv_gravity");
	g_psv_aim = CVAR_GET_POINTER("sv_aim");
	g_footsteps = CVAR_GET_POINTER("mp_footsteps");

	CVAR_REGISTER(&displaysoundlist);
	CVAR_REGISTER(&allow_spectators);

	CVAR_REGISTER(&teamplay);
	CVAR_REGISTER(&fraglimit);
	CVAR_REGISTER(&timelimit);

	CVAR_REGISTER(&fragsleft);
	CVAR_REGISTER(&timeleft);

	CVAR_REGISTER(&friendlyfire);
	CVAR_REGISTER(&falldamage);
	CVAR_REGISTER(&weaponstay);
	CVAR_REGISTER(&selfgauss);
	CVAR_REGISTER(&satchelfix);
	CVAR_REGISTER(&forcerespawn);
	CVAR_REGISTER(&flashlight);
	CVAR_REGISTER(&aimcrosshair);
	CVAR_REGISTER(&decalfrequency);
	CVAR_REGISTER(&teamlist);
	CVAR_REGISTER(&teamoverride);
	CVAR_REGISTER(&defaultteam);
	CVAR_REGISTER(&allowmonsters);
	CVAR_REGISTER(&bhopcap);
	CVAR_REGISTER(&multibyte_only);

	CVAR_REGISTER(&mp_chattime);

	// REGISTER CVARS FOR SKILL LEVEL STUFF

	// Turret
	CVAR_REGISTER(&sk_turret_health1);  // {"sk_turret_health1","0");
	CVAR_REGISTER(&sk_turret_health2);  // {"sk_turret_health2","0");
	CVAR_REGISTER(&sk_turret_health3);  // {"sk_turret_health3","0");

	// MiniTurret
	CVAR_REGISTER(&sk_miniturret_health1);  // {"sk_miniturret_health1","0");
	CVAR_REGISTER(&sk_miniturret_health2);  // {"sk_miniturret_health2","0");
	CVAR_REGISTER(&sk_miniturret_health3);  // {"sk_miniturret_health3","0");

	// Sentry Turret
	CVAR_REGISTER(&sk_sentry_health1);  // {"sk_sentry_health1","0");
	CVAR_REGISTER(&sk_sentry_health2);  // {"sk_sentry_health2","0");
	CVAR_REGISTER(&sk_sentry_health3);  // {"sk_sentry_health3","0");

	// HEALTH/SUIT CHARGE DISTRIBUTION
	CVAR_REGISTER(&sk_suitcharger1);
	CVAR_REGISTER(&sk_suitcharger2);
	CVAR_REGISTER(&sk_suitcharger3);

	CVAR_REGISTER(&sk_battery1);
	CVAR_REGISTER(&sk_battery2);
	CVAR_REGISTER(&sk_battery3);

	CVAR_REGISTER(&sk_healthcharger1);
	CVAR_REGISTER(&sk_healthcharger2);
	CVAR_REGISTER(&sk_healthcharger3);

	CVAR_REGISTER(&sk_healthkit1);
	CVAR_REGISTER(&sk_healthkit2);
	CVAR_REGISTER(&sk_healthkit3);

	CVAR_REGISTER(&sk_scientist_heal1);
	CVAR_REGISTER(&sk_scientist_heal2);
	CVAR_REGISTER(&sk_scientist_heal3);

	// monster damage adjusters
	CVAR_REGISTER(&sk_monster_head1);
	CVAR_REGISTER(&sk_monster_head2);
	CVAR_REGISTER(&sk_monster_head3);

	CVAR_REGISTER(&sk_monster_chest1);
	CVAR_REGISTER(&sk_monster_chest2);
	CVAR_REGISTER(&sk_monster_chest3);

	CVAR_REGISTER(&sk_monster_stomach1);
	CVAR_REGISTER(&sk_monster_stomach2);
	CVAR_REGISTER(&sk_monster_stomach3);

	CVAR_REGISTER(&sk_monster_arm1);
	CVAR_REGISTER(&sk_monster_arm2);
	CVAR_REGISTER(&sk_monster_arm3);

	CVAR_REGISTER(&sk_monster_leg1);
	CVAR_REGISTER(&sk_monster_leg2);
	CVAR_REGISTER(&sk_monster_leg3);

	// player damage adjusters
	CVAR_REGISTER(&sk_player_head1);
	CVAR_REGISTER(&sk_player_head2);
	CVAR_REGISTER(&sk_player_head3);

	CVAR_REGISTER(&sk_player_chest1);
	CVAR_REGISTER(&sk_player_chest2);
	CVAR_REGISTER(&sk_player_chest3);

	CVAR_REGISTER(&sk_player_stomach1);
	CVAR_REGISTER(&sk_player_stomach2);
	CVAR_REGISTER(&sk_player_stomach3);

	CVAR_REGISTER(&sk_player_arm1);
	CVAR_REGISTER(&sk_player_arm2);
	CVAR_REGISTER(&sk_player_arm3);

	CVAR_REGISTER(&sk_player_leg1);
	CVAR_REGISTER(&sk_player_leg2);
	CVAR_REGISTER(&sk_player_leg3);

	CVAR_REGISTER(&mp_respawn_avoid_radius);
	CVAR_REGISTER(&mp_corpse_show_time);

	CWeaponRegistry::StaticInstance().RegisterCvars();

	// END REGISTER CVARS FOR SKILL LEVEL STUFF

	WeaponInaccuracyCvars::Init();
	Bot_RegisterCVars();
	BotCommands::Initialise();
	CustomGeometry::InitialiseCommands();
	HitboxDebugging::Initialise();

	SERVER_COMMAND("exec skill.cfg\n");
}
