#pragma once

#include <unordered_map>
#include <memory>
#include <RmlUi/Core/Types.h>
#include "framework/BaseMenu.h"
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

	template<typename T>
	T* MenuDynamicCast(bool assertSuccessInDebug = true) const
	{
		T* newPtr = dynamic_cast<T*>(menuPtr.get());

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
	friend class MenuDirectory;

	explicit MenuDirectoryEntry(std::unique_ptr<BaseMenu>&& ptr) :
		menuPtr(std::move(ptr))
	{
	}
};

class MenuDirectory
{
public:
	void Populate();
	void Clear();

	void LoadAllMenus(Rml::Context& context);

	const MenuDirectoryEntry* GetMenuEntry(const Rml::String& name) const;

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

private:
	struct MapEntry
	{
		MenuDirectoryEntry menuEntry;
		bool loadedDocument = false;
	};

	using MenuMap = std::unordered_map<Rml::String, MapEntry>;

	template<typename T>
	void AddToMap()
	{
		AddToMap(new T());
	}

	void AddToMap(BaseMenu* newMenu);
	void SetUpDataBindings(MapEntry& entry, Rml::Context& context);
	void LoadMenuRml(MapEntry& entry, Rml::Context& context);
	void UnloadAllDocuments();

	MenuMap m_MenuMap;
};
