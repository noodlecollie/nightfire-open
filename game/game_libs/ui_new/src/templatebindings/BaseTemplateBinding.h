#pragma once

namespace Rml
{
	class Context;
	class DataModelConstructor;
}  // namespace Rml

class BaseTemplateBinding
{
public:
	virtual ~BaseTemplateBinding() = default;

	virtual bool SetUpDataBindings(Rml::Context* context, Rml::DataModelConstructor& constructor) = 0;
};
