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
	CMenuStudioModelView::VidInit();

	ent->curstate.number = 1;  // IMPORTANT: always set playerindex to 1
	ent->origin[0] = ent->curstate.origin[0] = 45.0f / tanf(DEG2RADF(refdef.fov_y / 2.0f));
	ent->origin[2] = ent->curstate.origin[2] = 2.0f;
	ent->angles[1] = ent->curstate.angles[1] = 180.0f;
	ent->player = true;  // yes, draw me as playermodel
}

void CMenuPlayerModelView::Draw()
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

	CMenuStudioModelView::Draw();
}
