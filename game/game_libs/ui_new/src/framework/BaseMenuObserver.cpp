#include "framework/BaseMenuObserver.h"
#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
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

bool BaseMenuObserver::IsModelLoaded() const
{
	return m_ParentMenu->IsModelLoaded();
}

Rml::DataModelHandle& BaseMenuObserver::ModelHandle(bool assertValid)
{
	return m_ParentMenu->ModelHandle(assertValid);
}

bool BaseMenuObserver::IsVariableDirty(const Rml::String& variableName)
{
	return m_ParentMenu->IsVariableDirty(variableName);
}

void BaseMenuObserver::DirtyVariable(const Rml::String& variableName)
{
	return m_ParentMenu->DirtyVariable(variableName);
}

void BaseMenuObserver::DirtyAllVariables()
{
	return m_ParentMenu->DirtyAllVariables();
}
