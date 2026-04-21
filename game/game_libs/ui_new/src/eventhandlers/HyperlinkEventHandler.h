#pragma once

#include "framework/DocumentObserver.h"
#include "framework/EventListenerObject.h"

class HyperlinkEventHandler : public DocumentObserver
{
public:
	explicit HyperlinkEventHandler(BaseMenu* parentMenu);

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	void HandleClickEvent(Rml::Event& event);

	EventListenerObject m_ClickListener;
};
