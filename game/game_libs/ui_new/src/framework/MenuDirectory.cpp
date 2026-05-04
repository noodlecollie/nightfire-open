#include "framework/MenuDirectory.h"
#include "framework/BaseMenu.h"
#include <RmlUi/Core/Context.h>
#include "UIDebug.h"

#include "menus/MainMenu.h"
#include "menus/MultiplayerMenu.h"
#include "menus/CreditsMenu.h"
#include "menus/ServerConnectionScreen.h"
#include "menus/options/KeysOptionsMenu.h"
#include "menus/options/MouseOptionsMenu.h"
#include "menus/options/AvOptionsMenu.h"
#include "menus/options/GameplayOptionsMenu.h"

void MenuDirectory::Populate()
{
	m_MenuMap.clear();

	AddToMap<MainMenu>();
	AddToMap<CreditsMenu>();
	AddToMap<ServerConnectionScreen>();
	AddToMap<KeysOptionsMenu>();
	AddToMap<MultiplayerMenu>();
	AddToMap<MouseOptionsMenu>();
	AddToMap<AvOptionsMenu>();
	AddToMap<GameplayOptionsMenu>();
}

void MenuDirectory::Clear()
{
	m_MenuMap.clear();
}

void MenuDirectory::LoadAllMenus(Rml::Context& context)
{
	// Do two passes, one for each operation, to make the logic clear.
	for ( MenuMap::iterator it = m_MenuMap.begin(); it != m_MenuMap.end(); ++it )
	{
		SetUpDataBindings(it->second, context);
	}

	for ( MenuMap::iterator it = m_MenuMap.begin(); it != m_MenuMap.end(); ++it )
	{
		LoadMenuRml(it->second, context);
	}
}

const MenuDirectoryEntry* MenuDirectory::GetMenuEntry(const Rml::String& name) const
{
	MenuMap::const_iterator it = m_MenuMap.find(name);
	return it != m_MenuMap.end() ? &it->second.menuEntry : nullptr;
}

void MenuDirectory::AddToMap(BaseMenu* newMenu)
{
	m_MenuMap.insert({Rml::String(newMenu->Name()), MapEntry {MenuDirectoryEntry(std::unique_ptr<BaseMenu>(newMenu))}});
}

void MenuDirectory::SetUpDataBindings(MapEntry& entry, Rml::Context& context)
{
	Rml::String dataModelName = entry.menuEntry.menuPtr->Name() + Rml::String("_model");

	Rml::DataModelConstructor constructor = context.CreateDataModel(dataModelName);
	bool success = false;

	if ( constructor )
	{
		if ( entry.menuEntry.menuPtr->SetUpDataModelBindings(constructor) )
		{
			success = true;
		}
		else
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_ERROR,
				"Failed to set up data bindings for menu %s",
				entry.menuEntry.menuPtr->Name()
			);
		}
	}
	else
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"Failed to construct data model \"%s\" for menu %s",
			dataModelName.c_str(),
			entry.menuEntry.menuPtr->Name()
		);
	}

	if ( !success )
	{
		context.RemoveDataModel(dataModelName);
	}
}

void MenuDirectory::LoadMenuRml(MapEntry& entry, Rml::Context& context)
{
	static const char* FALLBACK_RML =
		"<rml>\n"
		"<head>\n"
		"<title>Missing Menu!</title>\n"
		"<style>\n"
		"body { background-color: #FFFFFF; color: #000000; font-family: rmlui-debugger-font; "
		"width: 100%; height: 100%; }\n"
		"h1 { display: block; font-size: 50dp; }\n"
		"flex { display: flex; flex-direction: column; justify-content: center; height: 100%; }\n"
		"flex > h1 { display: block; margin-top: auto; margin-bottom: auto; text-align: center; }"
		"</style>\n"
		"</head>\n"
		"<body>\n"
		"<flex>\n"
		"<h1>Failed to load menu! :(</h1>"
		"</flex>\n"
		"</body>\n"
		"</rml>\n";

	entry.loadedDocument = false;
	entry.menuEntry.document = context.LoadDocument(entry.menuEntry.menuPtr->RmlFilePath());

	if ( entry.menuEntry.document )
	{
		entry.loadedDocument = true;
		entry.menuEntry.menuPtr->DocumentLoaded(entry.menuEntry.document);
		return;
	}

	Rml::Log::Message(
		Rml::Log::Type::LT_ERROR,
		"Failed to load %s for menu %s",
		entry.menuEntry.menuPtr->RmlFilePath(),
		entry.menuEntry.menuPtr->Name()
	);

	entry.menuEntry.document = context.LoadDocumentFromMemory(FALLBACK_RML);
	ASSERT(entry.menuEntry.document);
}

void MenuDirectory::UnloadAllDocuments()
{
	for ( MenuMap::iterator it = m_MenuMap.begin(); it != m_MenuMap.end(); ++it )
	{
		if ( it->second.loadedDocument )
		{
			it->second.menuEntry.menuPtr->DocumentUnloaded();
		}
	}
}
