#include "framework/MenuStack.h"
#include "framework/MenuDirectory.h"
#include <algorithm>

void MenuStack::Push(MenuDirectoryEntry* menu)
{
	MenuVec::iterator it = std::find(m_Stack.begin(), m_Stack.end(), menu);

	// Bit of a cheat, but move the menu to the top
	// if it already exists in the stack.
	if ( it != m_Stack.end() )
	{
		m_Stack.erase(it);
	}

	m_Stack.push_back(menu);
}

MenuDirectoryEntry* MenuStack::Pop()
{
	if ( m_Stack.empty() )
	{
		return nullptr;
	}

	MenuDirectoryEntry* menu = m_Stack.back();
	m_Stack.pop_back();
	return menu;
}

void MenuStack::Update(float currentTime)
{
	MenuDirectoryEntry* topMenu = Top();

	if ( topMenu )
	{
		topMenu->menuPtr->Update(currentTime);
	}
}

MenuDirectoryEntry* MenuStack::Top() const
{
	return (!m_Stack.empty()) ? m_Stack.back() : nullptr;
}

bool MenuStack::IsEmpty() const
{
	return m_Stack.empty();
}
