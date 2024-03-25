#pragma once

#include "BaseArrayModel.h"
#include "utlvector.h"
#include "EnginePublicAPI/cl_entity.h"

class CStudioSceneModel : public CMenuBaseArrayModel
{
public:
	void Update() override;
	const char* GetText(int line) final override;
	int GetRows() const final override;

	cl_entity_t* AddEntData();
	cl_entity_t* GetEntData(int index);
	void Clear();

private:
	static void DefaultInitialiseEntData(cl_entity_t* ent);

	CUtlVector<cl_entity_t> m_Entities;
};
