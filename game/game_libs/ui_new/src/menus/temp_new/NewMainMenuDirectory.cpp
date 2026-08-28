#include "menus/temp_new/NewMainMenuDirectory.h"

#include "menus/temp_new/StyleGuide.h"
#include "menus/temp_new/NewMainMenu.h"

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
	// TODO
	return nullptr;
}

void NewMainMenuDirectory::PopulateInternal()
{
	AddToMap<StyleGuide>();
	AddToMap<NewMainMenu>();
}
