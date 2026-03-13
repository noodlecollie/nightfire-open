#pragma once

#include <vector>

class MenuDirectoryEntry;

class MenuStack
{
public:
	bool Push(MenuDirectoryEntry* menu);
	MenuDirectoryEntry* Pop();
	void Update(float currentTime);

	MenuDirectoryEntry* Top() const;
	bool IsEmpty() const;

private:
	using MenuVec = std::vector<MenuDirectoryEntry*>;

	MenuVec m_Stack;
};
