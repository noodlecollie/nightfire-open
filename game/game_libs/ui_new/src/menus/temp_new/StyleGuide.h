#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/HintDisplay.h"

class StyleGuide : public MenuPage
{
public:
	StyleGuide();

protected:
	void OnDocumentLoaded() override;

private:
	HintDisplay m_HintDisplay;
};
