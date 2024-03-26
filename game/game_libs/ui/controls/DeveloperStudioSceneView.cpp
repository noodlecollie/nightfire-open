#include "DeveloperStudioSceneView.h"

bool CMenuDeveloperStudioSceneView::GetDrawOriginMarker() const
{
	return m_DrawOriginMarker;
}

void CMenuDeveloperStudioSceneView::SetDrawOriginMarker(bool draw)
{
	m_DrawOriginMarker = draw;
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
}
