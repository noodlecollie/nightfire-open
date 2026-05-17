#pragma once

#include "framework/BaseMenuObserver.h"
#include "framework/EventListenerObject.h"

class HyperlinkEventHandler : public BaseMenuObserver
{
public:
	explicit HyperlinkEventHandler(BaseMenu* parentMenu);

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	void HandleClickEvent(Rml::Event& event);

	EventListenerObject m_ClickListener;
};
