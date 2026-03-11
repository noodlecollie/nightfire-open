#include "menus/MainMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	BaseMenu(NAME, "resource/rml/main_menu.rml"),
	m_Tooltip("footer_tooltip", "")
{
}

void MainMenu::Update(float)
{
}

bool MainMenu::SetUpDataBindings(Rml::Context* context)
{
	Rml::DataModelConstructor constructor = context->CreateDataModel("mainmenumodel");

	if ( !constructor )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Failed to construct main menu data model");
		return false;
	}

	constructor.Bind(m_Tooltip.Name(), &m_Tooltip.Value());
	constructor.BindEventCallback("set_tooltip", &MainMenu::SetTooltip, this);
	constructor.BindEventCallback("clear_tooltip", &MainMenu::ClearTooltip, this);

	return true;
}

void MainMenu::SetTooltip(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList&)
{
	Rml::Element* element = event.GetTargetElement();

	if ( !element )
	{
		return;
	}

	Rml::Variant* tooltipAttr = element->GetAttribute("tooltip");

	if ( !tooltipAttr )
	{
		return;
	}

	if ( tooltipAttr->GetInto(m_Tooltip.Value()) )
	{
		handle.DirtyVariable(m_Tooltip.Name());
	}
}

void MainMenu::ClearTooltip(Rml::DataModelHandle handle, Rml::Event&, const Rml::VariantList&)
{
	if ( !m_Tooltip.Value().empty() )
	{
		m_Tooltip.Value().clear();
		handle.DirtyVariable(m_Tooltip.Name());
	}
}
