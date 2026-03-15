#include "framework/BaseMenu.h"
#include "UIDebug.h"

BaseMenu::BaseMenu(const char* name, const char* rmlFilePath) :
	m_Name(name),
	m_RmlFilePath(rmlFilePath)
{
	ASSERT(m_Name);
	ASSERT(m_RmlFilePath);
}

BaseMenu::~BaseMenu()
{
}

const char* BaseMenu::Name() const
{
	return m_Name;
}

const char* BaseMenu::RmlFilePath() const
{
	return m_RmlFilePath;
}

const MenuRequest* BaseMenu::CurrentRequest() const
{
	return m_Request.get();
}

void BaseMenu::SetCurrentRequest(MenuRequestType requestType, const Rml::VariantList& args)
{
	m_Request.reset(new MenuRequest(requestType, args));
}

void BaseMenu::ClearCurrentRequest()
{
	m_Request.reset();
}

bool BaseMenu::SetUpDefaultDataModelBindings(Rml::DataModelConstructor&)
{
	return true;
}

bool BaseMenu::SetUpCustomDataModelBindings(Rml::Context&)
{
	return false;
}

void BaseMenu::DocumentLoaded(Rml::ElementDocument*)
{
}

void BaseMenu::DocumentUnloaded(Rml::ElementDocument*)
{
}

void BaseMenu::Update(float)
{
}
