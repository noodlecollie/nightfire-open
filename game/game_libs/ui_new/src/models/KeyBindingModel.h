#pragma once

#include "framework/BaseTableModel.h"
#include "framework/DataBinding.h"
#include <RmlUi/Core/Types.h>
#include <vector>

class KeyBindingModel : public BaseTableModel
{
public:
	enum ColumnIndex
	{
		Description = 0,
		ConsoleCommand,
		PrimaryBinding,
		SecondaryBinding,

		TotalColumns
	};

	struct Entry
	{
		Rml::String description;
		Rml::String consoleCommand;
		Rml::String primaryBinding;
		Rml::String secondaryBinding;
	};

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;
	size_t Columns() const override;
	Rml::String DisplayString(size_t row, size_t column) const override;
	void Reset() override;

private:
	std::vector<Entry> m_Entries;
	Rml::DataModelHandle m_ModelHandle;
};
