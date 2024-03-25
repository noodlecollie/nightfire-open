#pragma once

#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/keydefs.h"
#include "EnginePublicAPI/ref_params.h"
#include "EnginePublicAPI/cl_entity.h"
#include "EnginePublicAPI/com_model.h"
#include "EnginePublicAPI/entity_types.h"
#include "BaseItem.h"
#include "StudioSceneModel.h"

class CMenuStudioSceneView : public CMenuBaseItem
{
public:
	CMenuStudioSceneView();
	void VidInit() override;
	void Draw() override;
	bool KeyDown(int key) override;
	bool KeyUp(int key) override;
	void CalcFov();

	CColor backgroundColor;
	CColor outlineFocusColor;

	CStudioSceneModel* GetModel() const;
	void SetModel(CStudioSceneModel* model);

	void SetAllowPitchRotation(bool allow);
	void SetAllowRightButtonZoom(bool allow);
	void ResetCamera();

private:
	enum class DragMode
	{
		None,
		LeftButton,
		RightButton
	};

	void HandleLeftMouseDragUpdate();
	void HandleRightMouseDragUpdate();

	CStudioSceneModel* m_Model = nullptr;
	ref_viewpass_t m_RefDef {};
	int m_PrevCursorX = 0;
	int m_PrevCursorY = 0;

	DragMode m_DragMode = DragMode::None;
	bool m_AllowPitchRotation = false;
	bool m_AllowRightButtonZoom = false;
	float m_DistFromOrigin = 0.0f;
};
