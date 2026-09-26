#pragma once

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <RmlUi/Core/Types.h>
#include "framework/BaseMenu.h"
#include "framework/IServerConnectionHandler.h"
#include "UIDebug.h"

namespace Rml
{
	class Context;
	class ElementDocument;
}  // namespace Rml

struct MenuDirectoryEntry
{
	std::unique_ptr<BaseMenu> menuPtr;
	Rml::ElementDocument* document = nullptr;
	Rml::String dataModelName;

	template<typename T>
	T* MenuDynamicCast(bool assertSuccessInDebug = true) const
	{
		T* newPtr = rmlui_dynamic_cast<T*>(menuPtr.get());

#ifdef _DEBUG
		if ( assertSuccessInDebug )
		{
			ASSERT(newPtr);
		}
#else
		(void)assertSuccessInDebug;
#endif

		return newPtr;
	}

private:
	friend class BaseMenuDirectory;

	explicit MenuDirectoryEntry(std::unique_ptr<BaseMenu>&& ptr) :
		menuPtr(std::move(ptr)),
		dataModelName(menuPtr->Name() + Rml::String("_model"))
	{
	}
};

class BaseMenuDirectory
{
public:
	void Populate();

	void AcquireContext(Rml::Context* context);
	void ReleaseContext();

	const MenuDirectoryEntry* GetMenuEntry(const Rml::String& name) const;
	void ReloadMenu(const Rml::String& name, bool reloadModel = false);

	template<typename T>
	T* GetMenu(const Rml::String& name, bool assertSuccessInDebug = true) const
	{
		const MenuDirectoryEntry* entry = GetMenuEntry(name);

#ifdef _DEBUG
		if ( assertSuccessInDebug )
		{
			ASSERT(entry);
		}
#endif

		if ( !entry )
		{
			return nullptr;
		}

		return entry->MenuDynamicCast<T>(assertSuccessInDebug);
	}

	virtual const MenuDirectoryEntry* GetMainMenu() const = 0;
	virtual const MenuDirectoryEntry* GetPauseMenu() const = 0;
	virtual const MenuDirectoryEntry* GetServerConnectionMenu() const = 0;
	virtual IServerConnectionHandler* GetServerConnectionHandler() const = 0;

protected:
	explicit BaseMenuDirectory(Rml::String rootDirectory);

	virtual void PopulateInternal() = 0;

	template<typename T>
	typename std::enable_if<std::is_base_of<BaseMenu, T>::value, void>::type AddToMap()
	{
		AddToMap(new T());
	}

	template<typename T>
	typename std::enable_if<std::is_base_of<BaseMenu, T>::value, const MenuDirectoryEntry*>::type AddToMapAndGetEntry()
	{
		T* menu = new T();
		AddToMap(menu);

		const MenuDirectoryEntry* entry = GetMenuEntry(static_cast<BaseMenu*>(menu)->Name());
		ASSERT(entry);
		return entry;
	}

	void AddToMap(BaseMenu* newMenu);

private:
	struct MapEntry
	{
		MenuDirectoryEntry menuEntry;
		bool loadedDocument = false;
	};

	using MenuMap = std::unordered_map<Rml::String, MapEntry>;

	void SetUpDataBindings(MapEntry& entry);
	void LoadMenuRml(MapEntry& entry);
	void UnloadMenu(MapEntry& entry, bool unloadModel);
	void UnloadAllMenus();

	Rml::String m_RootDirectory;
	MenuMap m_MenuMap;
	Rml::Context* m_Context = nullptr;
};
