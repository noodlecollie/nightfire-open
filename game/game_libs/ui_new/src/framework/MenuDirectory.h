#pragma once

#include <unordered_map>
#include <string>
#include <memory>

class BaseMenu;

class MenuDirectory
{
public:
	void LoadAllMenus();
	BaseMenu* GetMenu(const std::string& name) const;

private:
	using BaseMenuPtr = std::unique_ptr<BaseMenu>;
	using MenuMap = std::unordered_map<std::string, BaseMenuPtr>;

	template<typename T>
	void AddToMap()
	{
		AddToMap(new T());
	}

	void AddToMap(BaseMenu* newMenu);

	MenuMap m_MenuMap;
};
