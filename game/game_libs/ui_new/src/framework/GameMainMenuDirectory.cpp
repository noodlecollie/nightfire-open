#include "framework/GameMainMenuDirectory.h"

#include "menus/MainMenu.h"
#include "menus/MultiplayerMenu.h"
#include "menus/PauseMenu.h"
#include "menus/CreditsMenu.h"
#include "menus/ServerConnectionScreen.h"
#include "menus/CreateMultiplayerGameMenu.h"
#include "menus/options/KeysOptionsMenu.h"
#include "menus/options/MouseOptionsMenu.h"
#include "menus/options/AvOptionsMenu.h"
#include "menus/options/GameplayOptionsMenu.h"

GameMainMenuDirectory::GameMainMenuDirectory() :
	BaseMenuDirectory("resource/rml")
{
}

const MenuDirectoryEntry* GameMainMenuDirectory::GetMainMenu() const
{
	return GetMenuEntry(MainMenu::NAME);
}

const MenuDirectoryEntry* GameMainMenuDirectory::GetPauseMenu() const
{
	return GetMenuEntry(PauseMenu::NAME);
}

const MenuDirectoryEntry* GameMainMenuDirectory::GetServerConnectionMenu() const
{
	return GetMenuEntry(ServerConnectionScreen::NAME);
}

IServerConnectionHandler* GameMainMenuDirectory::GetServerConnectionHandler() const
{
	const MenuDirectoryEntry* entry = GetMenuEntry(ServerConnectionScreen::NAME);
	ASSERT(entry);
	return entry ? entry->MenuDynamicCast<IServerConnectionHandler>() : nullptr;
}

void GameMainMenuDirectory::PopulateInternal()
{
	AddToMap<MainMenu>();
	AddToMap<CreditsMenu>();
	AddToMap<ServerConnectionScreen>();
	AddToMap<KeysOptionsMenu>();
	AddToMap<MultiplayerMenu>();
	AddToMap<MouseOptionsMenu>();
	AddToMap<AvOptionsMenu>();
	AddToMap<GameplayOptionsMenu>();
	AddToMap<PauseMenu>();
	AddToMap<CreateMultiplayerGameMenu>();
}
