#include <string.h>
#include "BuildPlatform/Typedefs.h"
#include "CRTLib/bitdefs.h"
#include "client/cl_cliententities.h"
#include "client/client.h"

static void SetUpModelBaseState(entity_state_t* state, const mclientents_model_t* staticModel)
{
	if ( !state )
	{
		return;
	}

	memset(state, 0, sizeof(*state));

	if ( !staticModel )
	{
		return;
	}

	VectorCopy(staticModel->origin, state->origin);
	VectorCopy(staticModel->angles, state->angles);

	state->sequence = (int)staticModel->animation;
	state->skin = (short)staticModel->skin;
}

// Most of this is based on CL_ParseStaticEntity.
qboolean CreateModel(const mclientents_model_t* staticModel)
{
	if ( clgame.numStatics >= MAX_STATIC_ENTITIES )
	{
		return false;
	}

	cl_entity_t* ent = &clgame.static_entities[clgame.numStatics++];

	entity_state_t state;
	SetUpModelBaseState(&state, staticModel);

	memset(ent, 0, sizeof(*ent));

	ent->baseline = state;
	ent->prevstate = state;
	ent->curstate = state;
	ent->index = 0;
	ent->model = Mod_ForName(staticModel->modelName, false, true);
	VectorCopy(ent->curstate.origin, ent->origin);
	VectorCopy(ent->curstate.angles, ent->angles);
	ent->curstate.framerate = 1.0f;
	CL_ResetLatchedVars(ent, true);

	if ( ent->curstate.rendermode == kRenderNormal && ent->model != NULL )
	{
		if ( FBitSet(ent->model->flags, MODEL_TRANSPARENT) && Host_IsQuakeCompatible() )
		{
			ent->curstate.rendermode = kRenderTransAlpha;
			ent->curstate.renderamt = 255;
		}
	}

	R_AddEfrags(ent);

	return true;
}

void ClientEntities_LoadForBSP(model_t* model)
{
	if ( !model )
	{
		Con_Printf("StaticEntities_LoadForBSP: Passed BSP model was null\n");
		return;
	}

	if ( model->type != mod_brush )
	{
		Con_Printf("StaticEntities_LoadForBSP: Passed model was not a BSP\n");
		return;
	}

	if ( !model->clientEntities )
	{
		// No client entities to load, so just quit.
		return;
	}

	if ( model->clientEntities->models )
	{
		for ( size_t index = 0; index < model->clientEntities->modelCount; ++index )
		{
			if ( !CreateModel(&model->clientEntities->models[index]) )
			{
				Con_Printf(
					S_ERROR
					"ClientEntities_LoadForBSP: MAX_STATIC_ENTITIES limit of %d exceeded. %zu client-side models will "
					"not be loaded.\n",
					MAX_STATIC_ENTITIES,
					model->clientEntities->modelCount - index);

				break;
			}
		}
	}
}
