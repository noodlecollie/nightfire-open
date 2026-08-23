#pragma once

#include "framework/MenuPage.h"

class StyleGuide : public MenuPage
{
public:
	StyleGuide();

protected:
	void OnDocumentLoaded() override;
};
