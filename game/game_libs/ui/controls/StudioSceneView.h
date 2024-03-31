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
	enum class CameraCentre
	{
		ORIGIN = 0,
		EYES
	};

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

	const float* GetCameraCentre() const;
	void SetCameraCentre(float x, float y, float z);
	void SetCameraCentre(const float* centre);

	float GetCameraPitch() const;
	void SetCameraPitch(float degrees);
	float GetCameraYaw() const;
	void SetCameraYaw(float degrees);

	float GetCameraDistFromCentre() const;
	void SetCameraDistFromCentre(float dist);

	// Resets centre to origin, and resets angles and distance.
	void ResetCamera();

protected:
	virtual void PreDrawModels();

	ref_viewpass_t m_RefDef {};
	CStudioSceneModel* m_Model = nullptr;

private:
	enum class DragMode
	{
		None,
		LeftButton,
		RightButton
	};

	void HandleLeftMouseDragUpdate();
	void HandleRightMouseDragUpdate();

	int m_PrevCursorX = 0;
	int m_PrevCursorY = 0;

	DragMode m_DragMode = DragMode::None;
	bool m_AllowPitchRotation = false;
	bool m_AllowRightButtonZoom = false;
	float m_DistFromOrigin = 0.0f;
	vec3_t m_CameraCentre;
};
