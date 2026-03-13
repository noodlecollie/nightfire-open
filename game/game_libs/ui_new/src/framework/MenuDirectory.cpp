#include "framework/MenuDirectory.h"
#include "framework/BaseMenu.h"
#include "menus/MainMenu.h"
#include <RmlUi/Core/Context.h>

void MenuDirectory::Populate()
{
	m_MenuMap.clear();

	AddToMap<MainMenu>();
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

const MenuDirectoryEntry* MenuDirectory::GetMenuEntry(const std::string& name) const
{
	MenuMap::const_iterator it = m_MenuMap.find(name);
	return it != m_MenuMap.end() ? &it->second : nullptr;
}

void MenuDirectory::AddToMap(BaseMenu* newMenu)
{
	m_MenuMap.insert({std::string(newMenu->Name()), MenuDirectoryEntry(std::unique_ptr<BaseMenu>(newMenu))});
}

void MenuDirectory::SetUpDataBindings(MenuDirectoryEntry& entry, Rml::Context& context)
{
	Rml::String dataModelName = entry.menuPtr->Name() + Rml::String("_model");

	Rml::DataModelConstructor constructor = context.CreateDataModel(dataModelName);
	bool success = false;

	if ( constructor )
	{
		if ( entry.menuPtr->SetUpDataBindings(constructor) )
		{
			success = true;
		}
		else
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_ERROR,
				"Failed to set up data bindings for menu %s",
				entry.menuPtr->Name()
			);
		}
	}
	else
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"Failed to construct data model \"%s\" for menu %s",
			dataModelName.c_str(),
			entry.menuPtr->Name()
		);
	}

	if ( !success )
	{
		context.RemoveDataModel(dataModelName);
	}
}

void MenuDirectory::LoadMenuRml(MenuDirectoryEntry& entry, Rml::Context& context)
{
	entry.document = context.LoadDocument(entry.menuPtr->RmlFilePath());

	if ( !entry.document )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"Failed to load %s for menu %s",
			entry.menuPtr->RmlFilePath(),
			entry.menuPtr->Name()
		);
	}
}
