#include "bot_cvars.h"
#include "extdll.h"
#include "enginecallback.h"

cvar_t bot_speed = {"bot_speed","300" };
cvar_t bot_number = { "bot_number" , "3" };
cvar_t bot_skill = { "bot_skill" , "50" };
cvar_t bot_skill_aa = { "bot_skill_aa" , "0" };
cvar_t bot_jump = { "bot_jump" , "50" };
cvar_t bot_chat = { "bot_chat" , "30" };
cvar_t bot_dontshoot = { "bot_dontshoot" , "0" };
cvar_t bot_observer = { "bot_observer" , "0" };
cvar_t bot_dontmove = { "bot_dontmove", "0" };
cvar_t bot_profile_file = { "bot_profile_file", "bot_profiles.json" };

void Bot_RegisterCVars(void)
{
	// Shame there isn't a more iterative way to do this, but I think
	// the cvars are too embedded in the existing code to go changing
	// them up for something that trivial.

	CVAR_REGISTER ( &bot_speed );
	CVAR_REGISTER ( &bot_number );
	CVAR_REGISTER ( &bot_skill );
	CVAR_REGISTER ( &bot_skill_aa );
	CVAR_REGISTER ( &bot_jump );
	CVAR_REGISTER ( &bot_chat );
	CVAR_REGISTER ( &bot_dontshoot );
	CVAR_REGISTER ( &bot_observer );
	CVAR_REGISTER ( &bot_dontmove );
	CVAR_REGISTER ( &bot_profile_file );
}
