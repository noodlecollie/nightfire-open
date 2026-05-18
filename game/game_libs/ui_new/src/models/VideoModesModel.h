#pragma once

#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <vector>

class VideoModesModel : private BaseMenuObserver
{
public:
	VideoModesModel(BaseMenu* parentMenu);
	void Populate();

	size_t Rows() const;

	int Width(size_t row) const;
	int Height(size_t row) const;
	int VideoMode(size_t row) const;
	bool RowForDimensions(int width, int height, size_t& outRow) const;

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

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
