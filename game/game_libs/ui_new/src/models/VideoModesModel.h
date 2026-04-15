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
	size_t Columns() const override;
	Rml::String DisplayString(size_t row, size_t column) const override;

private:
	std::vector<Rml::String> m_VidModes;
	Rml::DataModelHandle m_ModelHandle;
};
