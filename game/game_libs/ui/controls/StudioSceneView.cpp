#include "extdll_menu.h"
#include "BaseMenu.h"
#include "StudioSceneView.h"
#include "MathLib/utils.h"

CMenuStudioSceneView::CMenuStudioSceneView() :
	CMenuBaseItem()
{
	memset(&m_RefDef, 0, sizeof(m_RefDef));

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

	if ( m_Model )
	{
		int numEnts = m_Model->GetRows();

		for ( int index = 0; index < numEnts; ++index )
		{
			EngFuncs::CL_CreateVisibleEntity(ET_NORMAL, m_Model->GetEntData(index));
		}
	}

	vec3_t fwd;
	AngleVectors(m_RefDef.viewangles, fwd, nullptr, nullptr);
	VectorScale(fwd, -1.0f * m_DistFromOrigin, fwd);
	VectorCopy(fwd, m_RefDef.vieworigin);

	EngFuncs::RenderScene(&m_RefDef);
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

float CMenuStudioSceneView::GetCameraDistFromOrigin() const
{
	return m_DistFromOrigin;
}

void CMenuStudioSceneView::SetCameraDistFromOrigin(float dist)
{
	m_DistFromOrigin = dist;
}

void CMenuStudioSceneView::ResetCamera()
{
	memset(m_RefDef.vieworigin, 0, sizeof(m_RefDef.vieworigin));
	memset(m_RefDef.viewangles, 0, sizeof(m_RefDef.viewangles));
	m_DistFromOrigin = 0.0f;
}

void CMenuStudioSceneView::HandleLeftMouseDragUpdate()
{
	float diffX = static_cast<float>(uiStatic.cursorX - m_PrevCursorX);

	if ( diffX )
	{
		float yaw = m_RefDef.viewangles[1];

		yaw -= diffX / uiStatic.scaleX;

		if ( yaw > 180.0f )
		{
			yaw -= 360.0f;
		}
		else if ( yaw < -180.0f )
		{
			yaw += 360.0f;
		}

		m_RefDef.viewangles[1] = yaw;
	}

	if ( m_AllowPitchRotation )
	{
		float diffY = static_cast<float>(uiStatic.cursorY - m_PrevCursorY);

		if ( diffY )
		{
			float pitch = m_RefDef.viewangles[2];

			pitch -= diffY / uiStatic.scaleY;

			if ( pitch > 180.0f )
			{
				pitch -= 360.0f;
			}
			else if ( pitch < -180.0f )
			{
				pitch += 360.0f;
			}

			m_RefDef.viewangles[2] = pitch;
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
