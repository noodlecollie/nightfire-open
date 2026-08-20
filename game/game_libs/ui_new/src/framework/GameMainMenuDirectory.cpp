#include "framework/GameMainMenuDirectory.h"

#include "menus/MainMenu.h"
#include "menus/MultiplayerMenu.h"
#include "menus/PauseMenu.h"
#include "menus/CreditsMenu.h"
#include "menus/ServerConnectionScreen.h"
#include "menus/CreateMultiplayerGameMenu.h"
#include "menus/StyleGuide.h"
#include "menus/options/KeysOptionsMenu.h"
#include "menus/options/MouseOptionsMenu.h"
#include "menus/options/AvOptionsMenu.h"
#include "menus/options/GameplayOptionsMenu.h"

GameMainMenuDirectory::GameMainMenuDirectory() :
	BaseMenuDirectory("resource/rml")
{
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
	AddToMap<StyleGuide>();
}
