#include "extdll_menu.h"
#include "BaseMenu.h"
#include "StudioSceneView.h"
#include "MathLib/utils.h"

static float BoundPitch(float pitch)
{
	return bound(-89.0f, pitch, 89.0f);
}

static float BoundYaw(float yaw)
{
	if ( yaw < 0 )
	{
		yaw = 360.0f - fmodf(-1.0f * yaw, 360.0f);
	}

	return fmodf((yaw + 180.0f), 360.0f) - 180.0f;
}

CMenuStudioSceneView::CMenuStudioSceneView() :
	CMenuBaseItem()
{
	memset(&m_RefDef, 0, sizeof(m_RefDef));
	VectorCopy(vec3_origin, m_CameraCentre);

	eFocusAnimation = QM_HIGHLIGHTIFFOCUS;

	m_RefDef.fov_x = 40.0f;
}

void CMenuStudioSceneView::VidInit()
{
	backgroundColor.SetDefault(uiColorHelp);
	colorStroke.SetDefault(uiInputFgColor);
	colorFocus.SetDefault(uiInputTextColor);

	if ( iStrokeWidth == 0 )
	{
		iStrokeWidth = uiStatic.outlineWidth;
	}

	CMenuBaseItem::VidInit();

	m_RefDef.viewport[0] = m_scPos.x;
	m_RefDef.viewport[1] = m_scPos.y;
	m_RefDef.viewport[2] = m_scSize.w;
	m_RefDef.viewport[3] = m_scSize.h;
	CalcFov();
}

bool CMenuStudioSceneView::KeyUp(int key)
{
	const bool stopDrag = (UI::Key::IsLeftMouse(key) && m_DragMode == DragMode::LeftButton) ||
		(UI::Key::IsRightMouse(key) && m_DragMode == DragMode::RightButton);

	if ( stopDrag )
	{
		m_DragMode = DragMode::None;
		return true;
	}

	return CMenuBaseItem::KeyUp(key);
}

bool CMenuStudioSceneView::KeyDown(int key)
{
	if ( UI_CursorInRect(m_scPos, m_scSize) && m_DragMode == DragMode::None )
	{
		if ( UI::Key::IsLeftMouse(key) || UI::Key::IsRightMouse(key) )
		{
			m_DragMode = UI::Key::IsLeftMouse(key) ? DragMode::LeftButton : DragMode::RightButton;
			m_PrevCursorX = uiStatic.cursorX;
			m_PrevCursorY = uiStatic.cursorY;

			return true;
		}
	}

	if ( !UI::Key::IsLeftArrow(key) && !UI::Key::IsRightArrow(key) )
	{
		return CMenuBaseItem::KeyDown(key);
	}

	float yaw = m_RefDef.viewangles[1];

	if ( UI::Key::IsLeftArrow(key) )
	{
		yaw += 10.0f;
	}
	else
	{
		yaw -= 10.0f;
	}

	if ( yaw > 180.0f )
	{
		yaw -= 360.0f;
	}
	else if ( yaw < -180.0f )
	{
		yaw += 360.0f;
	}

	m_RefDef.viewangles[1] = yaw;
	return true;
}

void CMenuStudioSceneView::Draw()
{
	// draw the background
	UI_FillRect(m_scPos, m_scSize, backgroundColor);

	// draw the rectangle
	if ( eFocusAnimation == QM_HIGHLIGHTIFFOCUS && IsCurrentSelected() )
	{
		UI_DrawRectangleExt(m_scPos, m_scSize, colorFocus, iStrokeWidth);
	}
	else
	{
		UI_DrawRectangleExt(m_scPos, m_scSize, colorStroke, iStrokeWidth);
	}

	EngFuncs::ClearScene();
	EngFuncs::UpdateScene();

	switch ( m_DragMode )
	{
		case DragMode::LeftButton:
		{
			HandleLeftMouseDragUpdate();
			break;
		}

		case DragMode::RightButton:
		{
			HandleRightMouseDragUpdate();
			break;
		}

		default:
		{
			break;
		}
	}

	m_PrevCursorX = uiStatic.cursorX;
	m_PrevCursorY = uiStatic.cursorY;

	vec3_t fwd;
	AngleVectors(m_RefDef.viewangles, fwd, nullptr, nullptr);
	VectorScale(fwd, -1.0f * m_DistFromOrigin, fwd);
	VectorAdd(m_CameraCentre, fwd, m_RefDef.vieworigin);

	PreDrawModels();

	if ( m_Model )
	{
		int numEnts = m_Model->GetRows();

		for ( int index = 0; index < numEnts; ++index )
		{
			EngFuncs::CL_CreateVisibleEntity(ET_NORMAL, m_Model->GetEntData(index));
		}
	}

	EngFuncs::RenderScene(&m_RefDef);
}

void CMenuStudioSceneView::PreDrawModels()
{
	// Nothing here - for subclasses to override.
}

CStudioSceneModel* CMenuStudioSceneView::GetModel() const
{
	return m_Model;
}

void CMenuStudioSceneView::SetModel(CStudioSceneModel* model)
{
	m_Model = model;
}

void CMenuStudioSceneView::CalcFov()
{
	float x = m_RefDef.viewport[2] / tanf(DEG2RADF(m_RefDef.fov_x) * 0.5f);
	float half_fov_y = atanf(m_RefDef.viewport[3] / x);
	m_RefDef.fov_y = RAD2DEGF(half_fov_y) * 2;
}

void CMenuStudioSceneView::SetAllowPitchRotation(bool allow)
{
	m_AllowPitchRotation = allow;
}

void CMenuStudioSceneView::SetAllowRightButtonZoom(bool allow)
{
	m_AllowRightButtonZoom = allow;
}

const float* CMenuStudioSceneView::GetCameraCentre() const
{
	return m_CameraCentre;
}

void CMenuStudioSceneView::SetCameraCentre(float x, float y, float z)
{
	m_CameraCentre[0] = x;
	m_CameraCentre[1] = y;
	m_CameraCentre[2] = z;
}

void CMenuStudioSceneView::SetCameraCentre(const float* centre)
{
	if ( centre )
	{
		SetCameraCentre(centre[0], centre[1], centre[2]);
	}
	else
	{
		SetCameraCentre(0.0f, 0.0f, 0.0f);
	}
}

float CMenuStudioSceneView::GetCameraPitch() const
{
	return m_RefDef.viewangles[PITCH];
}

void CMenuStudioSceneView::SetCameraPitch(float degrees)
{
	m_RefDef.viewangles[PITCH] = BoundPitch(degrees);
}

float CMenuStudioSceneView::GetCameraYaw() const
{
	return m_RefDef.viewangles[YAW];
}

void CMenuStudioSceneView::SetCameraYaw(float degrees)
{
	m_RefDef.viewangles[YAW] = BoundYaw(degrees);
}

float CMenuStudioSceneView::GetCameraDistFromCentre() const
{
	return m_DistFromOrigin;
}

void CMenuStudioSceneView::SetCameraDistFromCentre(float dist)
{
	m_DistFromOrigin = dist;
}

void CMenuStudioSceneView::ResetCamera()
{
	VectorCopy(vec3_origin, m_RefDef.vieworigin);
	VectorCopy(vec3_origin, m_RefDef.viewangles);
	VectorCopy(vec3_origin, m_CameraCentre);
	m_DistFromOrigin = 0.0f;
}

void CMenuStudioSceneView::HandleLeftMouseDragUpdate()
{
	int diffX = uiStatic.cursorX - m_PrevCursorX;

	if ( diffX )
	{
		float yaw = m_RefDef.viewangles[YAW];
		yaw -= static_cast<float>(diffX) / uiStatic.scaleX;
		m_RefDef.viewangles[YAW] = BoundYaw(yaw);
	}

	if ( m_AllowPitchRotation )
	{
		int diffY = uiStatic.cursorY - m_PrevCursorY;

		if ( diffY )
		{
			float pitch = m_RefDef.viewangles[PITCH];
			pitch += static_cast<float>(diffY) / uiStatic.scaleY;
			m_RefDef.viewangles[PITCH] = BoundPitch(pitch);
		}
	}
}

void CMenuStudioSceneView::HandleRightMouseDragUpdate()
{
	if ( m_AllowRightButtonZoom )
	{
		float diffY = static_cast<float>(uiStatic.cursorY - m_PrevCursorY);
		m_DistFromOrigin += diffY;

		if ( m_DistFromOrigin < 0.0f )
		{
			m_DistFromOrigin = 0.0f;
		}
	}
}
