/*
cursor_type.h - enumeration of possible mouse cursor types
Copyright (C) 2022 FWGS Team

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

typedef enum
{
	dc_user,
	dc_none,  // No cursor
	dc_arrow,  // Normal arrow pointer
	dc_ibeam,  // Text cursor
	dc_hourglass,  // "Busy" hourglass
	dc_crosshair,  // Precise crosshair
	dc_up,  // Also arrow, not used
	dc_sizenwse,  // NW to SE arrows
	dc_sizenesw,  // NE to SW arrows
	dc_sizewe,  // West to east arrows
	dc_sizens,  // North to south arrows
	dc_sizeall,  // Arrows pointing in all four compass directions
	dc_no,  // "Disallowed" sign
	dc_hand,  // Hand with pointing finger
	dc_last  // End of enum, not a cursor - do not use
} VGUI_DefaultCursor;
