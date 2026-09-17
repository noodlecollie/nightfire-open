#pragma once

#include "framework/MenuPage.h"
#include "eventhandlers/HyperlinkEventHandler.h"

class NewCreditsMenu : public MenuPage
{
public:
	NewCreditsMenu();

private:
	HyperlinkEventHandler m_HyperlinkEventHandler;
};
