#include "menus/temp_new/NewMainMenuDirectory.h"

#include "menus/temp_new/StyleGuide.h"
#include "menus/temp_new/NewMainMenu.h"
#include "menus/temp_new/NewCreditsMenu.h"
#include "menus/temp_new/NewKeysOptionsMenu.h"
#include "menus/temp_new/NewAvOptionsMenu.h"
#include "menus/temp_new/NewGameplayOptionsMenu.h"
#include "menus/temp_new/NewMouseOptionsMenu.h"
#include "menus/temp_new/NewServerConnectionScreen.h"
#include "menus/temp_new/NewPauseMenu.h"

NewMainMenuDirectory::NewMainMenuDirectory() :
	BaseMenuDirectory("resource/rml/temp_new")
{
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetMainMenu() const
{
	ASSERT(m_MainMenuEntry);
	return m_MainMenuEntry;
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetPauseMenu() const
{
	ASSERT(m_PauseMenuEntry);
	return m_PauseMenuEntry;
}

const MenuDirectoryEntry* NewMainMenuDirectory::GetServerConnectionMenu() const
{
	ASSERT(m_ServerConnectionMenuEntry);
	return m_ServerConnectionMenuEntry;
}

IServerConnectionHandler* NewMainMenuDirectory::GetServerConnectionHandler() const
{
	ASSERT(m_ServerConnectionMenuEntry);
	return m_ServerConnectionMenuEntry ? m_ServerConnectionMenuEntry->MenuDynamicCast<IServerConnectionHandler>()
									   : nullptr;
}

void NewMainMenuDirectory::PopulateInternal()
{
	m_MainMenuEntry = AddToMapAndGetEntry<NewMainMenu>();
	m_PauseMenuEntry = AddToMapAndGetEntry<NewPauseMenu>();
	m_ServerConnectionMenuEntry = AddToMapAndGetEntry<NewServerConnectionScreen>();

	AddToMap<StyleGuide>();
	AddToMap<NewCreditsMenu>();
	AddToMap<NewKeysOptionsMenu>();
	AddToMap<NewAvOptionsMenu>();
	AddToMap<NewGameplayOptionsMenu>();
	AddToMap<NewMouseOptionsMenu>();
}
