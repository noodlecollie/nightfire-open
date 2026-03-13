#pragma once

#include <RmlUi/Core/EventListener.h>

namespace Rml
{
	class ElementDocument;
}

class BaseMenu;

class EscapeMenuPopListener : public Rml::EventListener
{
public:
	explicit EscapeMenuPopListener(BaseMenu* menu);

	void Attach(Rml::ElementDocument* document);
	void Detach(Rml::ElementDocument* document);
	void ProcessEvent(Rml::Event& event) override;

private:
	BaseMenu* m_Menu;
};
