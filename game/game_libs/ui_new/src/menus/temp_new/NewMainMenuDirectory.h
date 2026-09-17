#pragma once

#include "framework/BaseMenuDirectory.h"

class NewMainMenuDirectory : public BaseMenuDirectory
{
public:
	NewMainMenuDirectory();

	const MenuDirectoryEntry* GetMainMenu() const override;
	const MenuDirectoryEntry* GetPauseMenu() const override;
	IServerConnectionMenu* GetServerConnectionHandler() const override;

protected:
	void PopulateInternal() override;
};
