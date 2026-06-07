#pragma once

#include "framework/MenuEventListenerObject.h"

class HyperlinkEventHandler
{
public:
	explicit HyperlinkEventHandler(BaseMenu* parentMenu);

private:
	void HandleClickEvent(Rml::Event& event);

	MenuEventListenerObject m_ClickListener;
};
