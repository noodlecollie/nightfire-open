#include "framework/BaseMenuObserver.h"
#include "framework/BaseMenu.h"
#include "UIDebug.h"

BaseMenuObserver::BaseMenuObserver(BaseMenu* parentMenu) :
	m_ParentMenu(parentMenu)
{
	ASSERT(m_ParentMenu);
	m_ParentMenu->RegisterObserver(this);
}

BaseMenu* BaseMenuObserver::ParentMenu() const
{
	return m_ParentMenu;
}

void BaseMenuObserver::DocumentLoaded(Rml::ElementDocument*)
{
}

void BaseMenuObserver::DocumentUnloaded(Rml::ElementDocument*)
{
}

bool BaseMenuObserver::SetUpDataModelBindings(Rml::DataModelConstructor&)
{
	return true;
}

Rml::DataModelHandle& BaseMenuObserver::ModelHandle(bool assertValid)
{
#ifndef _DEBUG
	(void)assertValid;
#endif

	if ( assertValid )
	{
		ASSERT(m_ParentMenu->m_ModelHandle.operator bool());
	}

	return m_ParentMenu->m_ModelHandle;
}
