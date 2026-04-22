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
};
