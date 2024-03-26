#pragma once

#include "StudioSceneView.h"

class CMenuDeveloperStudioSceneView : public CMenuStudioSceneView
{
public:
	bool GetDrawOriginMarker() const;
	void SetDrawOriginMarker(bool draw);

	bool GetDrawModelBoundingBoxes() const;
	void SetDrawModelBoundingBoxes(bool draw);

	bool GetDrawSequenceBoundingBoxes() const;
	void SetDrawSequenceBoundingBoxes(bool draw);

protected:
	void PreDrawModels() override;

private:
	static void DrawModelBoundingBox(cl_entity_t* ent);
	static void DrawSequenceBoundingBox(cl_entity_t* ent);
	static void DrawBoundingBox(const float* mins, const float* maxs, uint32_t colour);

	bool m_DrawOriginMarker = true;
	bool m_DrawModelBoundingBoxes = false;
	bool m_DrawSequenceBoundingBoxes = false;
};
