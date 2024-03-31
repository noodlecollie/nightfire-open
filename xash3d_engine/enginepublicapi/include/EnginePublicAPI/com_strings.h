/*
com_strings.h - all paths to external resources that hardcoded into engine
Copyright (C) 2018 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once

// end game final default message
#define DEFAULT_ENDGAME_MESSAGE "The End"

// path to the hash-pak that contain custom player decals
#define CUSTOM_RES_PATH "custom.hpk"

// path to default playermodel in GoldSrc
#define DEFAULT_PLAYER_PATH_HALFLIFE "models/player.mdl"

// path to default playermodel in Quake
#define DEFAULT_PLAYER_PATH_QUAKE "progs/player.mdl"

#define PLAYER_MODEL_PATH "models/player/_fallback.mdl"

// debug beams
#define DEFAULT_LASERBEAM_PATH "sprites/laserbeam.spr"

#define DEFAULT_INTERNAL_PALETTE "gfx/palette.lmp"

#define DEFAULT_EXTERNAL_PALETTE "gfx/palette.pal"

// path to folders where placed all sounds
#define DEFAULT_SOUNDPATH "sound/"

// path store saved games
#define DEFAULT_SAVE_DIRECTORY "save/"

// fallback to this skybox
#define DEFAULT_SKYBOX_PATH "gfx/env/desert"

// playlist for startup videos
#define DEFAULT_VIDEOLIST_PATH "media/StartupVids.txt"

#define DEFAULT_TEXTURE_DIR_PATH "textures/"

#define CVAR_GLCONFIG_DESCRIPTION "enable or disable %s"

#define DEFAULT_UPDATE_PAGE "https://github.com/noodlecollie/nightfire-open/releases"
