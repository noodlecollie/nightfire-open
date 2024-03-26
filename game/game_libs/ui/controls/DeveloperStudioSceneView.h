#pragma once

#include "StudioSceneView.h"

class CMenuDeveloperStudioSceneView : public CMenuStudioSceneView
{
public:
	bool GetDrawOriginMarker() const;
	void SetDrawOriginMarker(bool draw);

	bool GetDrawBoundingBoxes() const;
	void SetDrawBoundingBoxes(bool draw);

protected:
	void PreDrawModels() override;

private:
	void DrawBoundingBox(cl_entity_t* ent);

	bool m_DrawOriginMarker = true;
	bool m_DrawBoundingBoxes = false;
};
