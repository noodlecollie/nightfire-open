#include "framework/MenuDirectory.h"
#include "framework/BaseMenu.h"
#include "menus/MainMenu.h"

void MenuDirectory::LoadAllMenus()
{
	AddToMap<MainMenu>();
}

BaseMenu* MenuDirectory::GetMenu(const std::string& name) const
{
	MenuMap::const_iterator it = m_MenuMap.find(name);
	return it != m_MenuMap.end() ? it->second.get() : nullptr;
}

void MenuDirectory::AddToMap(BaseMenu* newMenu)
{
	m_MenuMap.insert({std::string(newMenu->Name()), BaseMenuPtr(newMenu)});
}
