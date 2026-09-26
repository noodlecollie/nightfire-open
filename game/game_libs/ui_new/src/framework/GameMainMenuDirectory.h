#pragma once

#include "framework/BaseMenuDirectory.h"

class GameMainMenuDirectory : public BaseMenuDirectory
{
public:
	GameMainMenuDirectory();

	const MenuDirectoryEntry* GetMainMenu() const override;
	const MenuDirectoryEntry* GetPauseMenu() const override;
	const MenuDirectoryEntry* GetServerConnectionMenu() const override;
	IServerConnectionHandler* GetServerConnectionHandler() const override;

protected:
	void PopulateInternal() override;
};
