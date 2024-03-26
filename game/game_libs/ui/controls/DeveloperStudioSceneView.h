#pragma once

#include "StudioSceneView.h"

class CMenuDeveloperStudioSceneView : public CMenuStudioSceneView
{
public:
	bool GetDrawOriginMarker() const;
	void SetDrawOriginMarker(bool draw);

protected:
	void PreDrawModels() override;

private:
	bool m_DrawOriginMarker = true;
};
