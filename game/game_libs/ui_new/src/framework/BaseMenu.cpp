#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "UIDebug.h"

BaseMenu::BaseMenu(const char* name, const char* rmlFilePath) :
	m_Name(name),
	m_RmlFilePath(rmlFilePath)
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

const MenuRequest* BaseMenu::CurrentRequest() const
{
	return m_Request.get();
}

void BaseMenu::ClearCurrentRequest()
{
	m_Request.reset();
}

bool BaseMenu::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	constructor.BindEventCallback("push_menu", &BaseMenu::HandlePushMenu, this);
	constructor.BindEventCallback("pop_menu", &BaseMenu::HandlePopMenu, this);

	return SetUpDataBindingsInternal(constructor);
}

bool BaseMenu::SetUpDataBindingsInternal(Rml::DataModelConstructor&)
{
	return true;
}

void BaseMenu::HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	m_Request.reset(new MenuRequest(MenuRequestType::PushMenu, Rml::VariantList(args)));
}

void BaseMenu::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	m_Request.reset(new MenuRequest(MenuRequestType::PopMenu));
}
