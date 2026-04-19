#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include "framework/DataVar.h"
#include "framework/BaseTemplateBinding.h"
#include "framework/DocumentObserver.h"
#include "framework/EventListenerObject.h"

class MenuFrameDataBinding : public BaseTemplateBinding, public DocumentObserver
{
public:
	MenuFrameDataBinding(BaseMenu* parentMenu);

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	void HandleMouseEvents(Rml::Event& event);
	void SetTooltip(Rml::Event& event);
	void ClearTooltip();

	DataVar<Rml::String> m_Tooltip;
	EventListenerObject m_TooltipListener;
	Rml::DataModelHandle m_ModelHandle;
	Rml::Element* m_CurrentTooltipElement = nullptr;
};
