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
#ifndef CMENUPLAYERMODELVIEW_H
#define CMENUPLAYERMODELVIEW_H

// HLSDK includes
#include "mathlib.h"
#include "EngineGamesAPI/const.h"
#include "EngineGamesAPI/keydefs.h"
#include "EngineGamesAPI/ref_params.h"
#include "EngineGamesAPI/cl_entity.h"
#include "EngineGamesAPI/com_model.h"
#include "EngineGamesAPI/entity_types.h"
// HLSDK includes end
#include "BaseItem.h"

class CMenuPlayerModelView : public CMenuBaseItem
{
public:
	CMenuPlayerModelView();
	void VidInit() override;
	void Draw() override;
	bool KeyDown(int key) override;
	bool KeyUp(int key) override;
	void CalcFov();

	HIMAGE hPlayerImage;

	ref_viewpass_t refdef;
	cl_entity_t* ent;

	bool bDrawAsPlayer;

	enum
	{
		PMV_DONTCARE = 0,
		PMV_SHOWMODEL,
		PMV_SHOWIMAGE
	} eOverrideMode;

	CColor backgroundColor;
	CColor outlineFocusColor;

private:
	cl_entity_t ent2;

	bool mouseYawControl;

	int prevCursorX, prevCursorY;
};

#endif  // CMENUPLAYERMODELVIEW_H
