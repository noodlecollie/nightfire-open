#include "DeveloperStudioSceneView.h"

bool CMenuDeveloperStudioSceneView::GetDrawOriginMarker() const
{
	return m_DrawOriginMarker;
}

void CMenuDeveloperStudioSceneView::SetDrawOriginMarker(bool draw)
{
	m_DrawOriginMarker = draw;
}

bool CMenuDeveloperStudioSceneView::GetDrawEyePositionMarker() const
{
	return m_DrawEyePositionMarker;
}

void CMenuDeveloperStudioSceneView::SetDrawEyePositionMarker(bool draw)
{
	m_DrawEyePositionMarker = draw;
}

bool CMenuDeveloperStudioSceneView::GetDrawModelBoundingBoxes() const
{
	return m_DrawModelBoundingBoxes;
}

void CMenuDeveloperStudioSceneView::SetDrawModelBoundingBoxes(bool draw)
{
	m_DrawModelBoundingBoxes = draw;
}

bool CMenuDeveloperStudioSceneView::GetDrawSequenceBoundingBoxes() const
{
	return m_DrawSequenceBoundingBoxes;
}

void CMenuDeveloperStudioSceneView::SetDrawSequenceBoundingBoxes(bool draw)
{
	m_DrawSequenceBoundingBoxes = draw;
}

void CMenuDeveloperStudioSceneView::PreDrawModels()
{
	if ( m_DrawOriginMarker )
	{
		DrawAxisMarker(0.0f, 0.0f, 0.0f, 16.0f);
	}

	if ( m_Model )
	{
		int numEnts = m_Model->GetRows();

		for ( int index = 0; index < numEnts; ++index )
		{
			cl_entity_t* studioModel = m_Model->GetEntData(index);

			if ( m_DrawModelBoundingBoxes )
			{
				DrawModelBoundingBox(studioModel);
			}

			if ( m_DrawSequenceBoundingBoxes )
			{
				DrawSequenceBoundingBox(studioModel);
			}

			if ( m_DrawEyePositionMarker )
			{
				DrawEyePositionMarker(studioModel);
			}
		}
	}
}

void CMenuDeveloperStudioSceneView::DrawModelBoundingBox(cl_entity_t* ent)
{
	if ( !ent || !ent->model )
	{
		return;
	}

	DrawBoundingBox(ent->model->mins, ent->model->maxs, 0xFFFFFFFF);
}

void CMenuDeveloperStudioSceneView::DrawSequenceBoundingBox(cl_entity_t* ent)
{
	if ( !ent || !ent->model )
	{
		return;
	}

	vec3_t mins = { 0.0f, 0.0f, 0.0f };
	vec3_t maxs = { 0.0f, 0.0f, 0.0f };

	if ( EngFuncs::GetModelSequenceBounds(ent, ent->curstate.sequence, mins, maxs) )
	{
		DrawBoundingBox(mins, maxs, 0x00FF00FF);
	}
}

void CMenuDeveloperStudioSceneView::DrawEyePositionMarker(cl_entity_t* ent)
{
	if ( !ent || !ent->model )
	{
		return;
	}

	vec3_t pos;
	EngFuncs::GetModelEyePosition(ent, pos);
	DrawAxisMarker(pos[0], pos[1], pos[2], 4.0f);
}

void CMenuDeveloperStudioSceneView::DrawAxisMarker(float x, float y, float z, float scale)
{
	// The model axes are not the same as the world axes,
	// so we need to colour and flip them appropriately.
	// Model X = World Y
	// Model Y = World -X
	// Model Z = World Z

	static const float AXIS_DIRS[3] =
	{
		-1.0f,
		1.0f,
		1.0f,
	};

	static const uint32_t AXIS_COLOURS[3] =
	{
		0x00FF00FF,
		0xFF0000FF,
		0x0000FFFF,
	};

	for ( size_t axisIndex = 0; axisIndex < 3; ++axisIndex )
	{
		vec3_t base = { x, y, z };
		vec3_t axis = { x, y, z };
		axis[axisIndex] += AXIS_DIRS[axisIndex] * scale;

		EngFuncs::StoreLine(base, axis, AXIS_COLOURS[axisIndex]);
	}
}

void CMenuDeveloperStudioSceneView::DrawBoundingBox(const float* mins, const float* maxs, uint32_t colour)
{
	vec3_t first;
	vec3_t second;

	// Draw bottom and top rects.
	for ( size_t zRect = 0; zRect < 2; ++zRect )
	{
		first[2] = second[2] = (zRect == 0 ? mins[2] : maxs[2]);

		first[0] = mins[0];
		first[1] = mins[1];
		second[0] = mins[0];
		second[1] = maxs[1];
		EngFuncs::StoreLine(first, second, colour);

		VectorCopy(second, first);
		second[0] = maxs[0];
		second[1] = maxs[1];
		EngFuncs::StoreLine(first, second, colour);

		VectorCopy(second, first);
		second[0] = maxs[0];
		second[1] = mins[1];
		EngFuncs::StoreLine(first, second, colour);

		VectorCopy(second, first);
		second[0] = mins[0];
		second[1] = mins[1];
		EngFuncs::StoreLine(first, second, colour);
	}

	// Draw vertical edges
	first[2] = mins[2];
	second[2] = maxs[2];

	first[0] = second[0] = mins[0];
	first[1] = second[1] = mins[1];
	EngFuncs::StoreLine(first, second, colour);

	first[0] = second[0] = mins[0];
	first[1] = second[1] = maxs[1];
	EngFuncs::StoreLine(first, second, colour);

	first[0] = second[0] = maxs[0];
	first[1] = second[1] = maxs[1];
	EngFuncs::StoreLine(first, second, colour);

	first[0] = second[0] = maxs[0];
	first[1] = second[1] = mins[1];
	EngFuncs::StoreLine(first, second, colour);
}
