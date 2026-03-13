#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "UIDebug.h"

static const Rml::String DEFAULT_FALLBACK_RML =
	"<rml>\n"
	"<head>\n"
	"<title>Missing Menu!</title>\n"
	"<style>\n"
	"body { background-color: #FFFFFF; color: #000000; font-family: rmlui-debugger-font; "
	"width: 100%; height: 100%; }\n"
	"h1 { display: block; font-size: 50dp; }\n"
	"flex { display: flex; flex-direction: column; justify-content: center; height: 100%; }\n"
	"flex > h1 { display: block; margin-top: auto; margin-bottom: auto; text-align: center; }"
	"</style>\n"
	"</head>\n"
	"<body>\n"
	"<flex>\n"
	"<h1>Failed to load menu! :(</h1>"
	"</flex>\n"
	"</body>\n"
	"</rml>\n";

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

const Rml::String& BaseMenu::FallbackRml() const
{
	return (!m_FallbackRml.empty()) ? m_FallbackRml : DEFAULT_FALLBACK_RML;
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
