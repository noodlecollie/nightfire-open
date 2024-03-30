#pragma once

#include "StudioSceneView.h"

class CMenuDeveloperStudioSceneView : public CMenuStudioSceneView
{
public:
	bool GetDrawOriginMarker() const;
	void SetDrawOriginMarker(bool draw);

	bool GetDrawEyePositionMarker() const;
	void SetDrawEyePositionMarker(bool draw);

	bool GetDrawModelBoundingBoxes() const;
	void SetDrawModelBoundingBoxes(bool draw);

	bool GetDrawSequenceBoundingBoxes() const;
	void SetDrawSequenceBoundingBoxes(bool draw);
protected:
	void PreDrawModels() override;

private:
	static void DrawModelBoundingBox(cl_entity_t* ent);
	static void DrawSequenceBoundingBox(cl_entity_t* ent);
	static void DrawEyePositionMarker(cl_entity_t* ent);
	static void DrawAxisMarker(float x, float y, float z, float scale);
	static void DrawBoundingBox(const float* mins, const float* maxs, uint32_t colour);

	bool m_DrawOriginMarker = true;
	bool m_DrawEyePositionMarker = false;
	bool m_DrawModelBoundingBoxes = false;
	bool m_DrawSequenceBoundingBoxes = false;
};
