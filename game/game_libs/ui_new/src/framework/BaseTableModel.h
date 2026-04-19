#pragma once

#include <RmlUi/Core/Types.h>

namespace Rml
{
	class DataModelConstructor;
}

class BaseTableModel
{
public:
	virtual ~BaseTableModel() = default;

	virtual bool SetUpDataBindings(Rml::DataModelConstructor& constructor) = 0;
	virtual size_t Rows() const = 0;
	virtual size_t Columns() const = 0;
	virtual Rml::String DisplayString(size_t row, size_t column) const = 0;
};
