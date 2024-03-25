#include "StudioSceneModel.h"
#include "EnginePublicAPI/com_model.h"

void CStudioSceneModel::Update()
{
}

const char* CStudioSceneModel::GetText(int line)
{
	if ( line < 0 || line >= m_Entities.Count() )
	{
		return "";
	}

	cl_entity_t* ent = m_Entities[line];
	return ent ? ent->model->name : "";
}

int CStudioSceneModel::GetRows() const
{
	return m_Entities.Count();
}

cl_entity_t* CStudioSceneModel::GetEntData(int index)
{
	return (index >= 0 && index < m_Entities.Count()) ? m_Entities[index] : nullptr;
}

const cl_entity_t* CStudioSceneModel::GetEntData(int index) const
{
	return (index >= 0 && index < m_Entities.Count()) ? m_Entities[index] : nullptr;
}
