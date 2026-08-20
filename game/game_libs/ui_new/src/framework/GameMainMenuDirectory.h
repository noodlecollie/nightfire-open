#pragma once

#include "framework/BaseMenuDirectory.h"

class GameMainMenuDirectory : public BaseMenuDirectory
{
public:
	GameMainMenuDirectory();

protected:
	void PopulateInternal() override;
};
