#pragma once

#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/DataVar.h"
#include "framework/MenuEventListenerObject.h"

class MenuFrameDataBinding : private BaseMenuObserver
{
public:
	MenuFrameDataBinding(BaseMenu* parentMenu);

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleDocumentHide(Rml::Event& event);
	void HandleMouseEvents(Rml::Event& event);
	void SetTooltip(Rml::Event& event);
	void ClearTooltip();

	DataVar<Rml::String> m_Tooltip;
	MenuEventListenerObject m_DocumentListener;
	MenuEventListenerObject m_TooltipListener;
	Rml::Element* m_CurrentTooltipElement = nullptr;
};
