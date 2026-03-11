#include "framework/MenuStack.h"
#include "framework/BaseMenu.h"

void MenuStack::Update(float currentTime)
{
	BaseMenu* topMenu = Top();

	if ( topMenu )
	{
		topMenu->Update(currentTime);
	}
}

BaseMenu* MenuStack::Top() const
{
	return (!m_Stack.empty()) ? m_Stack.back().get() : nullptr;
}

bool MenuStack::IsEmpty() const
{
	return m_Stack.empty();
}
