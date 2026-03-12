#pragma once

#include <vector>

class MenuDirectoryEntry;

class MenuStack
{
public:
	void Push(MenuDirectoryEntry* menu);
	MenuDirectoryEntry* Pop();
	void Update(float currentTime);

	MenuDirectoryEntry* Top() const;
	bool IsEmpty() const;

private:
	using MenuVec = std::vector<MenuDirectoryEntry*>;

	MenuVec m_Stack;
};
