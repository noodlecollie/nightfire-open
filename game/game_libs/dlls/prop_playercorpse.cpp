#include "prop_playercorpse.h"
#include "gameresources/GameResources.h"
#include "game.h"
#include "miniutl.h"
#include "mp_utils.h"

static const float THINK_INTERVAL_SECS = 0.1f;

LINK_ENTITY_TO_CLASS(prop_playercorpse, CPropPlayerCorpse);

void CPropPlayerCorpse::Create(entvars_t* other)
{
	// NOTE: This is done in a *very* specific order, because my previous attempt at doing this
	// caused there to be a frame between the player's body being hidden and the corpse being shown
	// where neither model was rendered.

	string_t istrClassname = MAKE_STRING("prop_playercorpse");

	edict_t* edict = CREATE_NAMED_ENTITY(istrClassname);
	entvars_t* pev = VARS(edict);
	CPropPlayerCorpse* ent = GetClassPtr<CPropPlayerCorpse>(pev);

	ent->Spawn();
	ent->Initialise(other);
}

void CPropPlayerCorpse::Spawn()
{
	CBaseAnimating::Spawn();

	pev->classname = MAKE_STRING("prop_playercorpse");

	SetThink(&CPropPlayerCorpse::Think);
	pev->nextthink = gpGlobals->time + THINK_INTERVAL_SECS;
}

void CPropPlayerCorpse::Initialise(entvars_t* other)
{
	if ( other && !(other->effects & EF_NODRAW) )
	{
		SetMultiplayerModel(other);

		// TODO: Movement, solidity, collision ignore with anything but the world (in spawn function?)
		VectorCopy(other->angles, pev->angles);
		pev->frame = other->frame;
		pev->colormap = other->colormap;
		pev->movetype = MOVETYPE_TOSS;
		VectorCopy(other->velocity, pev->velocity);
		VectorCopy(other->avelocity, pev->avelocity);
		pev->flags = 0;
		pev->deadflag = other->deadflag;

		pev->effects = other->effects & ~(EF_NOINTERP | EF_NODRAW);

		pev->sequence = other->sequence;
		pev->animtime = other->animtime;

		// This must be set manually, since when a player dies
		// their own framerate is set to zero.
		pev->framerate = 1.0f;

		UTIL_SetOrigin(pev, other->origin);
		UTIL_SetSize(pev, other->mins, other->maxs);

		InitBoneControllers();
		ResetSequenceInfo();
	}
	else
	{
		pev->effects |= EF_NODRAW;
	}

	m_flInitTime = gpGlobals->time;
}

void CPropPlayerCorpse::Think()
{
	// TODO: Set render mode and fade out once persist time has passed.
	float persistTime = Max(mp_corpse_show_time.value, 0.0f);
	if ( m_flInitTime == 0.0f || gpGlobals->time - m_flInitTime >= persistTime )
	{
		UTIL_Remove(this);
		return;
	}

	StudioFrameAdvance();
	pev->nextthink = gpGlobals->time + THINK_INTERVAL_SECS;
}

void CPropPlayerCorpse::SetMultiplayerModel(entvars_t* other)
{
	CUtlString fullModelPath = MPUtils::PlayerModelPath(GetClassPtr<CBasePlayer>(other));

	if ( fullModelPath.IsValid() )
	{
		SET_MODEL(ENT(pev), fullModelPath.String());
	}
}
