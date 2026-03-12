#include "framework/BaseMenu.h"
#include "UIDebug.h"

BaseMenu::BaseMenu(const char* name, const char* rmlFilePath, const char* dataModelName) :
	m_Name(name),
	m_RmlFilePath(rmlFilePath),
	m_DataModelName(dataModelName)
{
	ASSERT(m_Name);
	ASSERT(m_RmlFilePath);
}

const char* BaseMenu::Name() const
{
	return m_Name;
}

const char* BaseMenu::RmlFilePath() const
{
	return m_RmlFilePath;
}

const char* BaseMenu::DataModelName() const
{
	return m_DataModelName;
}

bool BaseMenu::SetUpDataBindings(Rml::DataModelConstructor&)
{
	return true;
}
