#ifndef BOT_CVARS_H
#define BOT_CVARS_H

#include "EnginePublicAPI/cvardef.h"

extern cvar_t bot_backend;
extern cvar_t bot_speed;
extern cvar_t bot_number;
extern cvar_t bot_skill_aa;
extern cvar_t bot_skill;
extern cvar_t bot_jump;
extern cvar_t bot_chat;

// cvars for debugging
extern cvar_t bot_dontshoot;  // does not shoot at humans or bots
extern cvar_t bot_observer;  // does not shoot at humans
extern cvar_t bot_dontmove;

extern cvar_t bot_profile_file;

void Bot_RegisterCVars(void);

#endif  // BOT_CVARS_H
