/*
PlayerModelView.cpp -- player model view
Copyright (C) 2018 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "extdll_menu.h"
#include "BaseMenu.h"
#include "PlayerModelView.h"

CMenuPlayerModelView::CMenuPlayerModelView() :
	CMenuBaseItem()
{
	memset(&refdef, 0, sizeof(refdef));

	ent = NULL;
	prevCursorX = 0;
	prevCursorY = 0;
	hPlayerImage = 0;
	eFocusAnimation = QM_HIGHLIGHTIFFOCUS;

	eOverrideMode = PMV_DONTCARE;
	refdef.fov_x = 40.0f;
	bDrawAsPlayer = true;
}

void CMenuPlayerModelView::VidInit()
{
	backgroundColor.SetDefault(uiColorHelp);
	colorStroke.SetDefault(uiInputFgColor);
	colorFocus.SetDefault(uiInputTextColor);

	if ( iStrokeWidth == 0 )
		iStrokeWidth = uiStatic.outlineWidth;

	CMenuBaseItem::VidInit();

	refdef.viewport[0] = m_scPos.x;
	refdef.viewport[1] = m_scPos.y;
	refdef.viewport[2] = m_scSize.w;
	refdef.viewport[3] = m_scSize.h;
	CalcFov();

	ent = EngFuncs::GetModel();

	memset(ent, 0, sizeof(cl_entity_t));

	// adjust entity params
	ent->index = 0;
	ent->curstate.body = 0;
	ent->curstate.number = 1;  // IMPORTANT: always set playerindex to 1
	ent->curstate.animtime = gpGlobals->time;  // start animation
	ent->curstate.sequence = 1;
	ent->curstate.scale = 1.0f;
	ent->curstate.frame = 0.0f;
	ent->curstate.framerate = 1.0f;
	ent->curstate.effects |= EF_FULLBRIGHT;
	ent->curstate.controller[0] = 127;
	ent->curstate.controller[1] = 127;
	ent->curstate.controller[2] = 127;
	ent->curstate.controller[3] = 127;
	ent->latched.prevcontroller[0] = 127;
	ent->latched.prevcontroller[1] = 127;
	ent->latched.prevcontroller[2] = 127;
	ent->latched.prevcontroller[3] = 127;
	ent->origin[0] = ent->curstate.origin[0] = 45.0f / tanf(DEG2RADF(refdef.fov_y / 2.0f));
	ent->origin[2] = ent->curstate.origin[2] = 2.0f;
	ent->angles[1] = ent->curstate.angles[1] = 180.0f;

	ent->player = bDrawAsPlayer;  // yes, draw me as playermodel
}

bool CMenuPlayerModelView::KeyUp(int key)
{
	if ( !ent )
	{
		return true;
	}

	const bool stopDrag = (UI::Key::IsLeftMouse(key) && dragMode == DragMode::LeftButton) ||
		(UI::Key::IsRightMouse(key) && dragMode == DragMode::RightButton);

	if ( stopDrag )
	{
		dragMode = DragMode::None;
	}

	return false;
}

bool CMenuPlayerModelView::KeyDown(int key)
{
	if ( !ent )
	{
		return true;
	}

	if ( UI_CursorInRect(m_scPos, m_scSize) && dragMode == DragMode::None )
	{
		if ( UI::Key::IsLeftMouse(key) || UI::Key::IsRightMouse(key) )
		{
			dragMode = UI::Key::IsLeftMouse(key) ? DragMode::LeftButton : DragMode::RightButton;
			prevCursorX = uiStatic.cursorX;
			prevCursorY = uiStatic.cursorY;
		}
	}

	float yaw = ent->angles[1];

	if ( UI::Key::IsLeftArrow(key) )
	{
		yaw -= 10.0f;
	}
	else if ( UI::Key::IsRightArrow(key) )
	{
		yaw += 10.0f;
	}
	else if ( UI::Key::IsEnter(key) )
	{
		ent->curstate.sequence++;
	}
	else
	{
		return CMenuBaseItem::KeyDown(key);
	}

	if ( yaw > 180.0f )
	{
		yaw -= 360.0f;
	}
	else if ( yaw < -180.0f )
	{
		yaw += 360.0f;
	}

	ent->angles[1] = ent->curstate.angles[1] = yaw;

	PlayLocalSound(uiStatic.sounds[SND_LAUNCH]);
	return true;
}

void CMenuPlayerModelView::Draw()
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

	if ( (eOverrideMode == PMV_DONTCARE && !ui_showmodels->value) ||  // controlled by engine cvar
		 (eOverrideMode == PMV_SHOWIMAGE) )  // controlled by menucode
	{
		if ( hPlayerImage )
		{
			EngFuncs::PIC_Set(hPlayerImage, 255, 255, 255, 255);
			EngFuncs::PIC_DrawTrans(m_scPos, m_scSize);
		}
		else
		{
			UI_DrawString(font, m_scPos, m_scSize, "No preview", colorBase, m_scChSize, QM_CENTER, ETF_SHADOW);
		}

		return;
	}

	EngFuncs::ClearScene();

	if ( uiStatic.enableAlphaFactor )
	{
		ent->curstate.rendermode = kRenderTransTexture;
		ent->curstate.renderamt = static_cast<int>(uiStatic.alphaFactor * 255);
	}
	else
	{
		ent->curstate.rendermode = kRenderNormal;
		ent->curstate.renderamt = 255;
	}

	switch ( dragMode )
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

	prevCursorX = uiStatic.cursorX;
	prevCursorY = uiStatic.cursorY;

	// draw the player model
	EngFuncs::CL_CreateVisibleEntity(ET_NORMAL, ent);
	EngFuncs::RenderScene(&refdef);
}

/*
=================
UI_PlayerSetup_CalcFov

assume refdef is valid
=================
*/
void CMenuPlayerModelView::CalcFov()
{
	float x = refdef.viewport[2] / tanf(DEG2RADF(refdef.fov_x) * 0.5f);
	float half_fov_y = atanf(refdef.viewport[3] / x);
	refdef.fov_y = RAD2DEGF(half_fov_y) * 2;
}

void CMenuPlayerModelView::SetModel(const char* path)
{
	if ( !ent )
	{
		return;
	}

	if ( path && *path )
	{
		EngFuncs::SetModel(ent, path);
	}
}

void CMenuPlayerModelView::SetAllowPitchRotation(bool allow)
{
	allowPitchRotation = allow;
}

void CMenuPlayerModelView::SetAllowRightButtonZoom(bool allow)
{
	allowRightButtonZoom = allow;
}

void CMenuPlayerModelView::ResetOrientation()
{
	memset(refdef.vieworigin, 0, sizeof(refdef.vieworigin));

	if ( ent )
	{
		memset(ent->angles, 0, sizeof(ent->angles));
		memset(ent->curstate.angles, 0, sizeof(ent->curstate.angles));
	}
}

void CMenuPlayerModelView::HandleLeftMouseDragUpdate()
{
	float diffX = static_cast<float>(uiStatic.cursorX - prevCursorX);

	if ( diffX )
	{
		float yaw = ent->angles[1];

		yaw += diffX / uiStatic.scaleX;

		if ( yaw > 180.0f )
		{
			yaw -= 360.0f;
		}
		else if ( yaw < -180.0f )
		{
			yaw += 360.0f;
		}

		ent->angles[1] = ent->curstate.angles[1] = yaw;
	}

	if ( allowPitchRotation )
	{
		float diffY = static_cast<float>(uiStatic.cursorY - prevCursorY);

		if ( diffY )
		{
			float pitch = ent->angles[2];

			pitch += diffY / uiStatic.scaleY;

			if ( pitch > 180.0f )
			{
				pitch -= 360.0f;
			}
			else if ( pitch < -180.0f )
			{
				pitch += 360.0f;
			}

			ent->angles[2] = ent->curstate.angles[2] = pitch;
		}
	}
}

void CMenuPlayerModelView::HandleRightMouseDragUpdate()
{
	if ( allowRightButtonZoom )
	{
		float diffY = static_cast<float>(uiStatic.cursorY - prevCursorY);
		refdef.vieworigin[0] += diffY;
	}
}
