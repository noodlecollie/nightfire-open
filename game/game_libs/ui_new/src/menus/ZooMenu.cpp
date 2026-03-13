#include "menus/ZooMenu.h"

ZooMenu::ZooMenu() :
	BaseMenu("zoo_menu", "resource/rml/zoo.rml"),
	m_EscapeListener(this)
{
}

void ZooMenu::DocumentLoaded(Rml::ElementDocument* document)
{
	m_EscapeListener.Attach(document);
}

void ZooMenu::DocumentUnloaded(Rml::ElementDocument* document)
{
	m_EscapeListener.Detach(document);
}
