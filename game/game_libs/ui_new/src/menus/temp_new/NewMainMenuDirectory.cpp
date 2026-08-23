#include "menus/temp_new/NewMainMenuDirectory.h"

#include "menus/temp_new/StyleGuide.h"

NewMainMenuDirectory::NewMainMenuDirectory() :
	BaseMenuDirectory("resource/rml/temp_new")
{
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetMainMenu() const
{
	// TODO
	return GetMenuEntry("style_guide");
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetPauseMenu() const
{
	// TODO
	return GetMenuEntry("style_guide");
}

IServerConnectionMenu* NewMainMenuDirectory::GetServerConnectionHandler() const
{
	// TODO
	return nullptr;
}

void NewMainMenuDirectory::PopulateInternal()
{
	AddToMap<StyleGuide>();
}
