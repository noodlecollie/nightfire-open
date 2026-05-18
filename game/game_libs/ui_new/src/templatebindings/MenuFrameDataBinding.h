#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include "framework/DataVar.h"
#include "framework/BaseTemplateBinding.h"
#include "framework/MenuEventListenerObject.h"

class MenuFrameDataBinding : public BaseTemplateBinding
{
public:
	MenuFrameDataBinding(BaseMenu* parentMenu);

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleDocumentHide(Rml::Event& event);
	void HandleMouseEvents(Rml::Event& event);
	void SetTooltip(Rml::Event& event);
	void ClearTooltip();

	DataVar<Rml::String> m_Tooltip;
	MenuEventListenerObject m_DocumentListener;
	MenuEventListenerObject m_TooltipListener;
	Rml::DataModelHandle m_ModelHandle;
	Rml::Element* m_CurrentTooltipElement = nullptr;
};
