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

void CMenuPlayerModelView::VidInit()
{
	CMenuStudioSceneView::VidInit();
	SetCameraDistFromCentre(45.0f / tanf(DEG2RADF(m_RefDef.fov_y / 2.0f)));
}

void CMenuPlayerModelView::Draw()
{
	if ( (eOverrideMode == PMV_DONTCARE && !ui_showmodels->value) ||  // controlled by engine cvar
		 (eOverrideMode == PMV_SHOWIMAGE) )  // controlled by menucode
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

		if ( hPlayerImage )
		{
			EngFuncs::PIC_Set(hPlayerImage, 255, 255, 255, 255);
			EngFuncs::PIC_DrawTrans(m_scPos, m_scSize);
		}
		else
		{
			UI_DrawString(font, m_scPos, m_scSize, "No preview", colorBase, m_scChSize, QM_CENTER, ETF_SHADOW);
		}

		// Don't call through to the base class.
		return;
	}

	if ( m_Model )
	{
		cl_entity_t* ent = m_Model->GetEntData(0);

		if ( ent )
		{
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

			ent->curstate.number = 1;  // IMPORTANT: always set playerindex to 1
			ent->origin[2] = ent->curstate.origin[2] = 2.0f;
			ent->angles[1] = ent->curstate.angles[1] = 180.0f;
			ent->player = true;  // yes, draw me as playermodel
		}
	}

	CMenuStudioSceneView::Draw();
}

bool CMenuPlayerModelView::KeyDown(int key)
{
	if ( UI::Key::IsEnter(key) )
	{
		if ( m_Model )
		{
			cl_entity_t* ent = m_Model->GetEntData(0);

			if ( ent )
			{
				++ent->curstate.sequence;

				if ( ent->curstate.sequence >= EngFuncs::GetModelSequenceCount(ent) )
				{
					ent->curstate.sequence = 0;
				}
			}

			return true;
		}
	}

	return CMenuStudioSceneView::KeyDown(key);
}
