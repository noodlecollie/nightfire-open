#pragma once

#include "framework/BaseMenu.h"
#include "listeners/EscapeMenuPopListener.h"

class ZooMenu : public BaseMenu
{
public:
	ZooMenu();

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	EscapeMenuPopListener m_EscapeListener;
};
