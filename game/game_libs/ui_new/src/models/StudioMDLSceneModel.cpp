#include "models/StudioMDLSceneModel.h"
#include "udll_int.h"

size_t StudioMDLSceneModel::GetNumEnts() const
{
	return m_Entities.size();
}

cl_entity_t* StudioMDLSceneModel::AddEntData()
{
	m_Entities.push_back(cl_entity_t {});

	cl_entity_t* ent = &m_Entities.back();
	DefaultInitialiseEntData(ent);

	return ent;
}

cl_entity_t* StudioMDLSceneModel::GetEntData(size_t index)
{
	return index < m_Entities.size() ? &m_Entities[index] : nullptr;
}

void StudioMDLSceneModel::Clear()
{
	m_Entities.clear();
}

void StudioMDLSceneModel::DefaultInitialiseEntData(cl_entity_t* ent)
{
	if ( !ent )
	{
		return;
	}

	memset(ent, 0, sizeof(*ent));

	ent->index = 0;
	ent->curstate.body = 0;
	ent->curstate.animtime = gpGlobals->time;  // start animation
	ent->curstate.sequence = 1;
	ent->curstate.scale = 1.0f;
	ent->curstate.frame = 0.0f;
	ent->curstate.framerate = 1.0f;
	ent->curstate.effects |= EF_FULLBRIGHT;
	ent->curstate.controller[0] = 127;
	ent->curstate.controller[1] = 127;
	ent->curstate.controller[2] = 127;
	ent->curstate.controller[3] = 127;
	ent->latched.prevcontroller[0] = 127;
	ent->latched.prevcontroller[1] = 127;
	ent->latched.prevcontroller[2] = 127;
	ent->latched.prevcontroller[3] = 127;
	ent->player = false;
}
