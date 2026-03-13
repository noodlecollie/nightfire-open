#include "framework/MenuDirectory.h"
#include "framework/BaseMenu.h"
#include "menus/MainMenu.h"
#include <RmlUi/Core/Context.h>
#include "UIDebug.h"

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

// TODO: Load fallback RML
void MenuDirectory::LoadMenuRml(MenuDirectoryEntry& entry, Rml::Context& context)
{
	static const char* FINAL_FALLBACK_RML =
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

	entry.document = context.LoadDocument(entry.menuPtr->RmlFilePath());

	if ( entry.document )
	{
		return;
	}

	Rml::Log::Message(
		Rml::Log::Type::LT_ERROR,
		"Failed to load %s for menu %s",
		entry.menuPtr->RmlFilePath(),
		entry.menuPtr->Name()
	);

	const Rml::String& fallback = entry.menuPtr->FallbackRml();

	if ( !fallback.empty() )
	{
		entry.document = context.LoadDocumentFromMemory(entry.menuPtr->FallbackRml());

		if ( entry.document )
		{
			return;
		}

		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Failed to load fallback RML for menu %s", entry.menuPtr->Name());
	}

	entry.document = context.LoadDocumentFromMemory(FINAL_FALLBACK_RML);
	ASSERT(entry.document);
}
