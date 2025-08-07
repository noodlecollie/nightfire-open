/*
BaseModel.h -- base model for simple MVC
Copyright (C) 2017-2018 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#ifndef BASE_MODEL_H
#define BASE_MODEL_H

#include "extdll_menu.h"
#include "Primitive.h"

enum ECellType
{
	CELL_TEXT = 0,
	CELL_IMAGE_DEFAULT,
	CELL_IMAGE_ADDITIVE,
	CELL_IMAGE_TRANS,
	CELL_IMAGE_HOLES,
	// CELL_ITEM,
};

class CMenuBaseModel
{
public:
	virtual ~CMenuBaseModel()
	{
	}

	// every model must implement these methods
	virtual void Update() = 0;
	virtual int GetColumns() const = 0;
	virtual int GetRows() const = 0;
	virtual const char* GetCellText(int line, int column) = 0;

	// customization
	virtual void OnDeleteEntry(int)
	{
	}

	virtual void OnActivateEntry(int)
	{
	}

	virtual unsigned int GetAlignmentForColumn(int) const
	{
		return QM_LEFT;
	}

	virtual ECellType GetCellType(int, int)
	{
		return CELL_TEXT;
	}

	virtual bool GetLineColor(int, unsigned int&, bool&) const
	{
		return false;
	}

	virtual bool GetCellColors(int, int, unsigned int&, bool&) const
	{
		return false;
	}

	virtual bool IsCellTextWrapped(int, int)
	{
		return true;
	}

	// sorting
	// false means no sorting support for column
	virtual bool Sort(int, bool)
	{
		return false;
	}
};

#endif  // BASE_MODEL_H
