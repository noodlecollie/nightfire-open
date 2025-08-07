#include <string.h>
#include "BuildPlatform/Typedefs.h"
#include "CRTLib/bitdefs.h"
#include "client/cl_cliententities.h"
#include "client/client.h"
#include "MathLib/angles.h"

static void SetUpModelBaseState(entity_state_t* state, const mclientents_model_t* staticModel, model_t* model)
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

	// I genuinely don't know if this is the best place to put this.
	// For some reason the pitch of the model in game is opposite to
	// what it should be according to the map editor. I'm not sure
	// whether the compile tools or the engine should be rectifying
	// this, but for now I've just copied the code that exists
	// elsewhere in the engine to flip the pitch.
	if ( !FBitSet(host.features, ENGINE_COMPENSATE_QUAKE_BUG) )
	{
		state->angles[PITCH] = -state->angles[PITCH];
	}

	state->body = staticModel->body;
	state->skin = (short)staticModel->skin;

	state->rendermode = kRenderNormal;
	state->renderamt = 255;
	state->rendercolor.r = staticModel->fixedLightColour[0];
	state->rendercolor.g = staticModel->fixedLightColour[1];
	state->rendercolor.b = staticModel->fixedLightColour[2];

	// Use velocity for the lighting offset
	VectorCopy(staticModel->lightingOriginOffset, state->velocity);

	if ( model && COM_StringIsTerminated(staticModel->sequenceName, sizeof(staticModel->sequenceName)) )
	{
		int sequenceIndex = Mod_LookUpStudioSequence(model, staticModel->sequenceName);

		if ( sequenceIndex < 0 )
		{
			sequenceIndex = 0;
		}

		state->sequence = sequenceIndex;
	}
}

// Most of this is based on CL_ParseStaticEntity.
static qboolean CreateModel(const mclientents_model_t* staticModel)
{
	if ( clgame.numStatics >= MAX_STATIC_ENTITIES )
	{
		return false;
	}

	cl_entity_t* ent = &clgame.static_entities[clgame.numStatics++];
	memset(ent, 0, sizeof(*ent));

	ent->model = Mod_ForName(staticModel->modelName, false, true);

	entity_state_t state;
	SetUpModelBaseState(&state, staticModel, ent->model);

	ent->baseline = state;
	ent->prevstate = state;
	ent->curstate = state;

	ent->index = 0;
	VectorCopy(ent->curstate.origin, ent->origin);
	VectorCopy(ent->curstate.angles, ent->angles);
	ent->curstate.framerate = 1.0f;
	CL_ResetLatchedVars(ent, true);

	if ( ent->curstate.rendermode == kRenderNormal && ent->model )
	{
		if ( FBitSet(ent->model->flags, MODEL_TRANSPARENT) && Host_IsQuakeCompatible() )
		{
			ent->curstate.rendermode = kRenderTransAlpha;
			ent->curstate.renderamt = 255;
		}
	}

	// statics may be respawned in game e.g. for demo recording
	if ( cls.state == ca_connected || cls.state == ca_validate )
	{
		ent->trivial_accept = INVALID_HANDLE;
	}

	R_AddEfrags(ent);

	return true;
}

qboolean CreateSound(const mclientents_sound_t* staticSound)
{
	// TODO: This will overflow the number of static sound channels rather quickly
	// if the client-side sounds are used in earnest for soundscapes and the like,
	// which is what they're intended for. We need a better implementation that
	// would support this properly. For now, we just do nothing. This should be
	// properly implemented later. The original code is left as a reference below.

#if 0
	if ( !staticSound )
	{
		return false;
	}

	sound_t soundHandle = S_RegisterSound(staticSound->sound);

	if ( soundHandle < 0 )
	{
		Con_Printf(S_ERROR "Could not load client-side sound %s\n", staticSound->sound);
		return false;
	}

	S_AmbientSound(staticSound->origin, 0, soundHandle, (float)staticSound->volume / 255.0f, 0.5f, 100, 0);
	return true;
#else
	(void)staticSound;
	return false;
#endif
}

void ClientEntities_LoadForBSP(model_t* model)
{
	if ( !model )
	{
		Con_Printf(S_ERROR "StaticEntities_LoadForBSP: Passed BSP model was null\n");
		return;
	}

	if ( model->type != mod_brush )
	{
		Con_Printf(S_ERROR "StaticEntities_LoadForBSP: Passed model was not a BSP\n");
		return;
	}

	if ( !model->clientEntities )
	{
		// No client entities to load, so just quit.
		return;
	}

	size_t modelsLoaded = 0;
	size_t soundsLoaded = 0;

	if ( model->clientEntities->models )
	{
		for ( size_t index = 0; index < model->clientEntities->modelCount; ++index )
		{
			const mclientents_model_t* staticModel = &model->clientEntities->models[index];

			if ( !COM_StringIsTerminated(staticModel->modelName, sizeof(staticModel->modelName)) )
			{
				Con_Printf(
					S_WARN "ClientEntities_LoadForBSP: Client-side model at index %zu had invalid model name\n",
					index);

				continue;
			}

			if ( !CreateModel(staticModel) )
			{
				Con_Printf(
					S_ERROR
					"ClientEntities_LoadForBSP: MAX_STATIC_ENTITIES limit of %d exceeded. %zu client-side models will "
					"not be loaded.\n",
					MAX_STATIC_ENTITIES,
					model->clientEntities->modelCount - index);

				break;
			}

			++modelsLoaded;
		}
	}

	if ( model->clientEntities->sounds )
	{
		for ( size_t index = 0; index < model->clientEntities->soundCount; ++index )
		{
			const mclientents_sound_t* staticSound = &model->clientEntities->sounds[index];

			if ( !COM_StringIsTerminated(staticSound->sound, sizeof(staticSound->sound)) )
			{
				Con_Printf(
					S_WARN "ClientEntities_LoadForBSP: Client-side sound at index %zu had invalid sound name\n",
					index);

				continue;
			}

			if ( CreateSound(staticSound) )
			{
				++soundsLoaded;
			}
		}
	}

	Con_DPrintf("ClientEntities_LoadForBSP: Loaded %zu models and %zu sounds\n", modelsLoaded, soundsLoaded);
}
