#pragma once

#include <vector>
#include <memory>
#include <type_traits>

class BaseMenu;

class MenuStack
{
public:
	void Push(BaseMenu* menu);
	BaseMenu* Pop();
	void Update(float currentTime);

	BaseMenu* Top() const;
	bool IsEmpty() const;

private:
	using MenuVec = std::vector<BaseMenu*>;

	MenuVec m_Stack;
};
