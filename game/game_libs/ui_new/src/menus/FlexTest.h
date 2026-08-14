#pragma once

#include "framework/MenuPage.h"

class FlexTest : public MenuPage
{
public:
	FlexTest();

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleMouseEvents(Rml::Event& event);

	Rml::String m_Tooltip;
	Rml::DataModelHandle m_ModelHandle;
	MenuEventListenerObject m_TooltipListener;
};
