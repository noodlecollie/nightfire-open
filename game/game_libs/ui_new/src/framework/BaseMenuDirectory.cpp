#include "framework/BaseMenuDirectory.h"
#include "framework/BaseMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>
#include "UIDebug.h"

BaseMenuDirectory::BaseMenuDirectory(Rml::String rootDirectory) :
	m_RootDirectory(std::move(rootDirectory))
{
}

void BaseMenuDirectory::Populate()
{
	ASSERT(!m_Context);

	// We cannot already be set up.
	if ( m_Context )
	{
		return;
	}

	m_MenuMap.clear();

	PopulateInternal();
}

void BaseMenuDirectory::AcquireContext(Rml::Context* context)
{
	ReleaseContext();

	m_Context = context;

	if ( !m_Context )
	{
		ASSERT(false);
		return;
	}

	for ( MenuMap::iterator it = m_MenuMap.begin(); it != m_MenuMap.end(); ++it )
	{
		SetUpDataBindings(it->second);
	}

	for ( MenuMap::iterator it = m_MenuMap.begin(); it != m_MenuMap.end(); ++it )
	{
		LoadMenuRml(it->second);
	}
}

void BaseMenuDirectory::ReleaseContext()
{
	if ( !m_Context )
	{
		return;
	}

	UnloadAllMenus();
	m_Context = nullptr;
}

const MenuDirectoryEntry* BaseMenuDirectory::GetMenuEntry(const Rml::String& name) const
{
	MenuMap::const_iterator it = m_MenuMap.find(name);
	return it != m_MenuMap.end() ? &it->second.menuEntry : nullptr;
}

void BaseMenuDirectory::ReloadMenu(const Rml::String& name, bool reloadModel)
{
	ASSERT(m_Context);

	if ( !m_Context )
	{
		return;
	}

	MenuMap::iterator it = m_MenuMap.find(name);

	if ( it == m_MenuMap.end() )
	{
		return;
	}

	MapEntry& entry = it->second;
	const bool wasVisible = entry.menuEntry.document && entry.menuEntry.document->IsVisible();

	UnloadMenu(entry, reloadModel);

	if ( reloadModel )
	{
		SetUpDataBindings(entry);
	}

	LoadMenuRml(entry);

	if ( wasVisible && entry.menuEntry.document )
	{
		entry.menuEntry.document->Show();
	}
}

void BaseMenuDirectory::AddToMap(BaseMenu* newMenu)
{
	m_MenuMap.insert({Rml::String(newMenu->Name()), MapEntry {MenuDirectoryEntry(std::unique_ptr<BaseMenu>(newMenu))}});
}

void BaseMenuDirectory::SetUpDataBindings(MapEntry& entry)
{
	ASSERT(m_Context);

	Rml::DataModelConstructor constructor = m_Context->CreateDataModel(entry.menuEntry.dataModelName);
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
			entry.menuEntry.dataModelName.c_str(),
			entry.menuEntry.menuPtr->Name()
		);
	}

	if ( !success )
	{
		m_Context->RemoveDataModel(entry.menuEntry.dataModelName);
	}
}

void BaseMenuDirectory::LoadMenuRml(MapEntry& entry)
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

	ASSERT(m_Context);

	entry.loadedDocument = false;

	Rml::String fullPath =
		Rml::CreateString("%s/%s", m_RootDirectory.c_str(), entry.menuEntry.menuPtr->RmlFileRelPath());

	entry.menuEntry.document = m_Context->LoadDocument(fullPath);

	if ( entry.menuEntry.document )
	{
		entry.loadedDocument = true;
		entry.menuEntry.menuPtr->DocumentLoaded(entry.menuEntry.document);
		return;
	}

	Rml::Log::Message(
		Rml::Log::Type::LT_ERROR,
		"Failed to load %s for menu %s",
		fullPath.c_str(),
		entry.menuEntry.menuPtr->Name()
	);

	entry.menuEntry.document = m_Context->LoadDocumentFromMemory(FALLBACK_RML);
	ASSERT(entry.menuEntry.document);
}

void BaseMenuDirectory::UnloadMenu(MapEntry& entry, bool unloadModel)
{
	ASSERT(m_Context);

	if ( entry.loadedDocument )
	{
		ASSERT(entry.menuEntry.document);

		entry.menuEntry.menuPtr->DocumentUnloaded();
		m_Context->UnloadDocument(entry.menuEntry.document);

		entry.menuEntry.document = nullptr;
		entry.loadedDocument = false;
	}

	if ( unloadModel )
	{
		m_Context->RemoveDataModel(entry.menuEntry.dataModelName);
	}
}

void BaseMenuDirectory::UnloadAllMenus()
{
	ASSERT(m_Context);

	for ( MenuMap::iterator it = m_MenuMap.begin(); it != m_MenuMap.end(); ++it )
	{
		UnloadMenu(it->second, true);
	}
}
