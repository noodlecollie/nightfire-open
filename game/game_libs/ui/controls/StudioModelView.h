#pragma once

#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/keydefs.h"
#include "EnginePublicAPI/ref_params.h"
#include "EnginePublicAPI/cl_entity.h"
#include "EnginePublicAPI/com_model.h"
#include "EnginePublicAPI/entity_types.h"
#include "BaseItem.h"

class CMenuStudioModelView : public CMenuBaseItem
{
public:
	CMenuStudioModelView();
	void VidInit() override;
	void Draw() override;
	bool KeyDown(int key) override;
	bool KeyUp(int key) override;
	void CalcFov();

	ref_viewpass_t refdef;
	cl_entity_t* ent;

	CColor backgroundColor;
	CColor outlineFocusColor;

	void SetModel(const char* path);
	void SetAllowPitchRotation(bool allow);
	void SetAllowRightButtonZoom(bool allow);
	void ResetOrientation();

private:
	enum class DragMode
	{
		None,
		LeftButton,
		RightButton
	};

	void HandleLeftMouseDragUpdate();
	void HandleRightMouseDragUpdate();

	int prevCursorX;
	int prevCursorY;

	DragMode dragMode = DragMode::None;
	bool allowPitchRotation = false;
	bool allowRightButtonZoom = false;
};
