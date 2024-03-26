#pragma once

#include "StudioSceneView.h"

class CMenuDeveloperStudioSceneView : public CMenuStudioSceneView
{
public:
	bool DrawOriginMarker() const;
	void SetDrawOriginMarker(bool draw);

protected:
	void PreDrawModels() override;

private:
	bool m_DrawOriginMarker = true;
};
