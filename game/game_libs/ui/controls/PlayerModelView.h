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

#include "StudioModelView.h"

class CMenuPlayerModelView : public CMenuStudioModelView
{
public:
	void VidInit() override;
	void Draw() override;

	HIMAGE hPlayerImage;

	enum
	{
		PMV_DONTCARE = 0,
		PMV_SHOWMODEL,
		PMV_SHOWIMAGE
	} eOverrideMode;
};
