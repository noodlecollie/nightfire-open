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

#pragma once

#include "StudioSceneView.h"

class CMenuPlayerModelView : public CMenuStudioSceneView
{
public:
	void VidInit() override;
	void Draw() override;
	bool KeyDown(int key) override;

	bool GetAllowCyclingSequences() const;
	void SetAllowCyclingSequences(bool allow);

	HIMAGE hPlayerImage;

	enum
	{
		PMV_DONTCARE = 0,
		PMV_SHOWMODEL,
		PMV_SHOWIMAGE
	} eOverrideMode;

private:
	bool m_AllowCyclingSequences = true;
};
