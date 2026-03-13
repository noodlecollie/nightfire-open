#include "framework/MenuStack.h"
#include "framework/MenuDirectory.h"
#include <algorithm>

bool MenuStack::Push(MenuDirectoryEntry* menu)
{
	MenuVec::iterator it = std::find(m_Stack.begin(), m_Stack.end(), menu);

	if ( it != m_Stack.end() )
	{
		return false;
	}

	m_Stack.push_back(menu);
	return true;
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
