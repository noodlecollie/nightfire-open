#include "menus/temp_new/NewMainMenuDirectory.h"

#include "menus/temp_new/StyleGuide.h"
#include "menus/temp_new/NewMainMenu.h"
#include "menus/temp_new/NewCreditsMenu.h"
#include "menus/temp_new/NewKeysOptionsMenu.h"
#include "menus/temp_new/NewAvOptionsMenu.h"
#include "menus/temp_new/NewGameplayOptionsMenu.h"
#include "menus/temp_new/NewMouseOptionsMenu.h"
#include "menus/temp_new/NewServerConnectionScreen.h"

NewMainMenuDirectory::NewMainMenuDirectory() :
	BaseMenuDirectory("resource/rml/temp_new")
{
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetMainMenu() const
{
	return GetMenuEntry(NewMainMenu::NAME);
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetPauseMenu() const
{
	// TODO
	return GetMenuEntry(NewMainMenu::NAME);
}

IServerConnectionMenu* NewMainMenuDirectory::GetServerConnectionHandler() const
{
	const MenuDirectoryEntry* entry = GetMenuEntry(NewServerConnectionScreen::NAME);
	ASSERT(entry);
	return entry ? entry->MenuDynamicCast<IServerConnectionMenu>() : nullptr;
}

void NewMainMenuDirectory::PopulateInternal()
{
	AddToMap<StyleGuide>();
	AddToMap<NewMainMenu>();
	AddToMap<NewCreditsMenu>();
	AddToMap<NewKeysOptionsMenu>();
	AddToMap<NewAvOptionsMenu>();
	AddToMap<NewGameplayOptionsMenu>();
	AddToMap<NewMouseOptionsMenu>();
	AddToMap<NewServerConnectionScreen>();
}
