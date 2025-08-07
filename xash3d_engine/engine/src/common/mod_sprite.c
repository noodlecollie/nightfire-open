/*
mod_sprite.c - sprite loading
Copyright (C) 2010 Uncle Mike
Copyright (C) 2019 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common/common.h"
#include "EngineInternalAPI/sprite.h"
#include "EnginePublicAPI/studio.h"
#include "common/engine_mempool.h"

#if !XASH_DEDICATED()
#include "client/ref_common.h"
#endif  // XASH_DEDICATED()

/*
====================
Mod_LoadSpriteModel

load sprite model
====================
*/
void Mod_LoadSpriteModel(model_t* mod, const void* buffer, qboolean* loaded, uint texFlags)
{
	dsprite_q1_t* pinq1;
	dsprite_hl_t* pinhl;
	dsprite_t* pin;
	msprite_t* psprite;
	char poolname[MAX_VA_STRING];
	int i, size;

	(void)texFlags;

	if ( loaded )
		*loaded = false;
	pin = (dsprite_t*)buffer;
	mod->type = mod_sprite;
	i = pin->version;

	if ( pin->ident != IDSPRITEHEADER )
	{
		Con_DPrintf(S_ERROR "%s has wrong id (%x should be %x)\n", mod->name, pin->ident, IDSPRITEHEADER);
		return;
	}

	if ( i != SPRITE_VERSION_Q1 && i != SPRITE_VERSION_HL && i != SPRITE_VERSION_32 )
	{
		Con_DPrintf(
			S_ERROR "%s has wrong version number (%i should be %i or %i)\n",
			mod->name,
			i,
			SPRITE_VERSION_Q1,
			SPRITE_VERSION_HL);
		return;
	}

	Q_snprintf(poolname, sizeof(poolname), "^2%s^7", mod->name);
	mod->mempool = Mem_AllocPool(poolname);

	if ( i == SPRITE_VERSION_Q1 || i == SPRITE_VERSION_32 )
	{
		pinq1 = (dsprite_q1_t*)buffer;
		size = sizeof(msprite_t) + (pinq1->numframes - 1) * sizeof(psprite->frames);
		psprite = Mem_Calloc(mod->mempool, size);
		mod->cache.data = psprite;  // make link to extradata

		psprite->type = (short)pinq1->type;
		psprite->texFormat = SPR_ADDITIVE;  // SPR_ALPHTEST;
		psprite->numframes = mod->numframes = pinq1->numframes;
		psprite->facecull = SPR_CULL_FRONT;
		psprite->radius = (int)pinq1->boundingradius;
		psprite->synctype = pinq1->synctype;

		// LordHavoc: hack to allow sprites to be non-fullbright
		// TODO: This is a bad way of doing this - we should check
		// the first character of the file name, not any character
		// in the entire path!
		for ( size_t index = 0; index < sizeof(mod->name) && mod->name[index]; index++ )
		{
			if ( mod->name[index] == '!' )
			{
				psprite->texFormat = SPR_ALPHTEST;
			}
		}

		mod->mins[0] = mod->mins[1] = -pinq1->bounds[0] * 0.5f;
		mod->maxs[0] = mod->maxs[1] = pinq1->bounds[0] * 0.5f;
		mod->mins[2] = -pinq1->bounds[1] * 0.5f;
		mod->maxs[2] = pinq1->bounds[1] * 0.5f;
	}
	else  // if( i == SPRITE_VERSION_HL )
	{
		pinhl = (dsprite_hl_t*)buffer;
		size = sizeof(msprite_t) + (pinhl->numframes - 1) * sizeof(psprite->frames);
		psprite = Mem_Calloc(mod->mempool, size);
		mod->cache.data = psprite;  // make link to extradata

		psprite->type = (short)pinhl->type;
		psprite->texFormat = (short)pinhl->texFormat;
		psprite->numframes = mod->numframes = pinhl->numframes;
		psprite->facecull = pinhl->facetype;
		psprite->radius = pinhl->boundingradius;
		psprite->synctype = pinhl->synctype;

		mod->mins[0] = mod->mins[1] = -pinhl->bounds[0] * 0.5f;
		mod->maxs[0] = mod->maxs[1] = pinhl->bounds[0] * 0.5f;
		mod->mins[2] = -pinhl->bounds[1] * 0.5f;
		mod->maxs[2] = pinhl->bounds[1] * 0.5f;
	}

	if ( loaded )
	{
		*loaded = true;  // done
	}

	if ( Host_IsDedicated() )
	{
		// skip frames loading
		psprite->numframes = 0;
		return;
	}
}
