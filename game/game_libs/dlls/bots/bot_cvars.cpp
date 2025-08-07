#include "bot_cvars.h"
#include "extdll.h"
#include "enginecallback.h"

cvar_t bot_backend = {(char*)"bot_backend", (char*)"botrix", 0, 0.0f, nullptr};
cvar_t bot_speed = {(char*)"bot_speed", (char*)"300", 0, 0.0f, nullptr};
cvar_t bot_number = {(char*)"bot_number", (char*)"3", 0, 0.0f, nullptr};
cvar_t bot_skill = {(char*)"bot_skill", (char*)"50", 0, 0.0f, nullptr};
cvar_t bot_skill_aa = {(char*)"bot_skill_aa", (char*)"0", 0, 0.0f, nullptr};
cvar_t bot_jump = {(char*)"bot_jump", (char*)"50", 0, 0.0f, nullptr};
cvar_t bot_chat = {(char*)"bot_chat", (char*)"30", 0, 0.0f, nullptr};
cvar_t bot_dontshoot = {(char*)"bot_dontshoot", (char*)"0", 0, 0.0f, nullptr};
cvar_t bot_observer = {(char*)"bot_observer", (char*)"0", 0, 0.0f, nullptr};
cvar_t bot_dontmove = {(char*)"bot_dontmove", (char*)"0", 0, 0.0f, nullptr};
cvar_t bot_profile_file = {(char*)"bot_profile_file", (char*)"bot_profiles.json", 0, 0.0f, nullptr};

void Bot_RegisterCVars(void)
{
	// Shame there isn't a more iterative way to do this, but I think
	// the cvars are too embedded in the existing code to go changing
	// them up for something that trivial.

	CVAR_REGISTER(&bot_backend);
	CVAR_REGISTER(&bot_speed);
	CVAR_REGISTER(&bot_number);
	CVAR_REGISTER(&bot_skill);
	CVAR_REGISTER(&bot_skill_aa);
	CVAR_REGISTER(&bot_jump);
	CVAR_REGISTER(&bot_chat);
	CVAR_REGISTER(&bot_dontshoot);
	CVAR_REGISTER(&bot_observer);
	CVAR_REGISTER(&bot_dontmove);
	CVAR_REGISTER(&bot_profile_file);
}
