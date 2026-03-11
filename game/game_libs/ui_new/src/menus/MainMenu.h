#pragma once

#include "framework/BaseMenu.h"

class MainMenu : public BaseMenu
{
public:
	static const char* const NAME;

	MainMenu();
	void Update(float currentTime) override;
};
