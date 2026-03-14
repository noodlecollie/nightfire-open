#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "framework/BaseMenu.h"

namespace Rml
{
	class Context;
	class ElementDocument;
}  // namespace Rml

struct MenuDirectoryEntry
{
	std::unique_ptr<BaseMenu> menuPtr;
	Rml::ElementDocument* document = nullptr;

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

	const MenuDirectoryEntry* GetMenuEntry(const std::string& name) const;

private:
	struct MapEntry
	{
		MenuDirectoryEntry menuEntry;
		bool loadedDocument = false;
	};

	using MenuMap = std::unordered_map<std::string, MapEntry>;

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
