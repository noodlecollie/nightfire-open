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
	using MenuMap = std::unordered_map<std::string, MenuDirectoryEntry>;

	template<typename T>
	void AddToMap()
	{
		AddToMap(new T());
	}

	void AddToMap(BaseMenu* newMenu);
	void SetUpDataBindings(MenuDirectoryEntry& entry, Rml::Context& context);
	void LoadMenuRml(MenuDirectoryEntry& entry, Rml::Context& context);

	MenuMap m_MenuMap;
};
