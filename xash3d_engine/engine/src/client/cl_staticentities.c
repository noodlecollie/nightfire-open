#include <string.h>
#include "CRTLib/bitdefs.h"
#include "client/cl_staticentities.h"
#include "client/client.h"

static void SetUpModelBaseState(entity_state_t* state, const mclientents_model_t* staticModel)
{
	if ( !state )
	{
		return;
	}

	memset(&state, 0, sizeof(state));

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
qboolean StaticEntities_CreateModel(const mclientents_model_t* staticModel)
{
	if ( !staticModel )
	{
		return false;
	}

	int index = clgame.numStatics;

	if ( index >= MAX_STATIC_ENTITIES )
	{
		Con_Printf(
			S_ERROR "StaticEntities_CreateModel: MAX_STATIC_ENTITIES limit of %d exceeded\n",
			MAX_STATIC_ENTITIES);

		return false;
	}

	cl_entity_t* ent = &clgame.static_entities[index];
	clgame.numStatics++;

	entity_state_t state;
	SetUpModelBaseState(&state, staticModel);

	memset(ent, 0, sizeof(*ent));

	ent->baseline = state;
	ent->prevstate = state;
	ent->curstate = state;
	ent->index = 0;
	ent->model = staticModel->modelName
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
