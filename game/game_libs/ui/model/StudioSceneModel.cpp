#include "StudioSceneModel.h"
#include "EnginePublicAPI/com_model.h"
#include "Utils.h"

void CStudioSceneModel::Update()
{
}

const char* CStudioSceneModel::GetText(int line)
{
	if ( line < 0 || line >= m_Entities.Count() )
	{
		return "";
	}

	cl_entity_t* ent = &m_Entities[line];
	return ent->model ? ent->model->name : "";
}

int CStudioSceneModel::GetRows() const
{
	return m_Entities.Count();
}

cl_entity_t* CStudioSceneModel::AddEntData()
{
	m_Entities.AddToTail(cl_entity_t{});

	cl_entity_t* ent = &m_Entities.Tail();
	DefaultInitialiseEntData(ent);

	return ent;
}

cl_entity_t* CStudioSceneModel::GetEntData(int index)
{
	return (index >= 0 && index < m_Entities.Count()) ? &m_Entities[index] : nullptr;
}

void CStudioSceneModel::Clear()
{
	m_Entities.Purge();
}

void CStudioSceneModel::DefaultInitialiseEntData(cl_entity_t* ent)
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
