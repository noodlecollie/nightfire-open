#pragma once

#include "EnginePublicAPI/cl_entity.h"
#include <vector>

class StudioMDLSceneModel
{
public:
	size_t GetNumEnts() const;
	cl_entity_t* AddEntData();
	cl_entity_t* GetEntData(size_t index);
	void Clear();

private:
	static void DefaultInitialiseEntData(cl_entity_t* ent);

	std::vector<cl_entity_t> m_Entities;
};
