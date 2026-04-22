#pragma once

#include "framework/BaseTableModel.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <vector>

class VideoModesModel : public BaseTableModel
{
public:
	void Populate();

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;

	int Width(size_t row) const;
	int Height(size_t row) const;
	int VideoMode(size_t row) const;
	bool RowForDimensions(int width, int height, size_t& outRow) const;

private:
	struct Entry
	{
		Rml::String label;
		int index = 0;
		int width = 0;
		int height = 0;
	};

	std::vector<Entry> m_VidModes;
	Rml::DataModelHandle m_ModelHandle;
};
