#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include "framework/DataBinding.h"
#include "framework/BaseTemplateBinding.h"

class MenuFrameDataBinding : public BaseTemplateBinding
{
public:
	MenuFrameDataBinding();
	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;

private:
	void SetTooltip(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList&);
	void ClearTooltip(Rml::DataModelHandle handle, Rml::Event&, const Rml::VariantList&);

	DataBinding<Rml::String> m_Tooltip;
};
