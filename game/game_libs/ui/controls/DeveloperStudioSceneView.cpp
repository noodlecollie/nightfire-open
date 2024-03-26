#include "DeveloperStudioSceneView.h"

bool CMenuDeveloperStudioSceneView::GetDrawOriginMarker() const
{
	return m_DrawOriginMarker;
}

void CMenuDeveloperStudioSceneView::SetDrawOriginMarker(bool draw)
{
	m_DrawOriginMarker = draw;
}

bool CMenuDeveloperStudioSceneView::GetDrawBoundingBoxes() const
{
	return m_DrawBoundingBoxes;
}

void CMenuDeveloperStudioSceneView::SetDrawBoundingBoxes(bool draw)
{
	m_DrawBoundingBoxes = draw;
}

void CMenuDeveloperStudioSceneView::PreDrawModels()
{
	if ( m_DrawOriginMarker )
	{
		for ( size_t axisIndex = 0; axisIndex < 3; ++axisIndex )
		{
			vec3_t axis = { 0.0f, 0.0f, 0.0f };
			axis[axisIndex] = 16.0f;

			uint32_t colourMask = 0x0000FF00;
			colourMask <<= (2 - axisIndex) * 8;

			// Alpha should always be 255.
			colourMask |= 0x000000FF;

			EngFuncs::StoreLine(vec3_origin, axis, colourMask);
		}
	}

	if ( m_DrawBoundingBoxes && m_Model )
	{
		int numEnts = m_Model->GetRows();

		for ( int index = 0; index < numEnts; ++index )
		{
			DrawBoundingBox(m_Model->GetEntData(index));
		}
	}
}

void CMenuDeveloperStudioSceneView::DrawBoundingBox(cl_entity_t* ent)
{
	static constexpr uint32_t COL_WHITE = 0xFFFFFFFF;

	if ( !ent || !ent->model )
	{
		return;
	}

	const float* mins = ent->model->mins;
	const float* maxs = ent->model->maxs;

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
		EngFuncs::StoreLine(first, second, COL_WHITE);

		VectorCopy(second, first);
		second[0] = maxs[0];
		second[1] = maxs[1];
		EngFuncs::StoreLine(first, second, COL_WHITE);

		VectorCopy(second, first);
		second[0] = maxs[0];
		second[1] = mins[1];
		EngFuncs::StoreLine(first, second, COL_WHITE);

		VectorCopy(second, first);
		second[0] = mins[0];
		second[1] = mins[1];
		EngFuncs::StoreLine(first, second, COL_WHITE);
	}

	// Draw vertical edges
	first[2] = mins[2];
	second[2] = maxs[2];

	first[0] = second[0] = mins[0];
	first[1] = second[1] = mins[1];
	EngFuncs::StoreLine(first, second, COL_WHITE);

	first[0] = second[0] = mins[0];
	first[1] = second[1] = maxs[1];
	EngFuncs::StoreLine(first, second, COL_WHITE);

	first[0] = second[0] = maxs[0];
	first[1] = second[1] = maxs[1];
	EngFuncs::StoreLine(first, second, COL_WHITE);

	first[0] = second[0] = maxs[0];
	first[1] = second[1] = mins[1];
	EngFuncs::StoreLine(first, second, COL_WHITE);
}
