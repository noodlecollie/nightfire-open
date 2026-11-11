#include "templatebindings/OptionsTabBarDataBinding.h"
#include <RmlUi/Core/ElementDocument.h>
#include "framework/BaseMenu.h"
#include "UIDebug.h"

static constexpr const char* const EVENT_GO_TO_TAB = "goToTab";

OptionsTabBarDataBinding::OptionsTabBarDataBinding(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_ShowListener(parentMenu, this, &OptionsTabBarDataBinding::HandleDocumentShow, {Rml::EventId::Hide})
{
}

void OptionsTabBarDataBinding::DocumentLoaded(Rml::ElementDocument* document)
{
	m_TabSetElement = rmlui_dynamic_cast<Rml::ElementTabSet*>(document->GetElementById("options_tab_set"));
	ASSERT(m_TabSetElement);

	if ( !m_TabSetElement )
	{
		return;
	}

	SetActiveTab();
}

void OptionsTabBarDataBinding::DocumentUnloaded(Rml::ElementDocument*)
{
	m_TabSetElement = nullptr;
}

bool OptionsTabBarDataBinding::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !BaseMenuObserver::SetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.BindEventCallback(EVENT_GO_TO_TAB, &OptionsTabBarDataBinding::HandleGoToTab, this) )
	{
		return false;
	}

	return true;
}

void OptionsTabBarDataBinding::SetGoToNewTabCallback(GoToNewTabFunc cb)
{
	m_GoToNewTabCallback = std::move(cb);
}

void OptionsTabBarDataBinding::HandleDocumentShow(Rml::Event&)
{
	SetActiveTab();
}

void OptionsTabBarDataBinding::HandleGoToTab(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.empty() )
	{
		return;
	}

	RequestChangeMenu(args[0].Get<Rml::String>());
}

void OptionsTabBarDataBinding::RequestChangeMenu(const Rml::String& newMenu)
{
	// We don't actually change the active tab,
	// because each menu has its own tab bar and the active
	// tab must always be the one relevant to that menu.
	if ( !newMenu.empty() && newMenu != ParentMenu()->Name() && m_GoToNewTabCallback )
	{
		m_GoToNewTabCallback(newMenu);
	}
}

void OptionsTabBarDataBinding::SetActiveTab()
{
	// Work out which tab we should set as active, based on the name of this menu.
	const Rml::String menuName = ParentMenu()->Name();
	Rml::ElementList tabs;
	m_TabSetElement->QuerySelectorAll(tabs, "tabs>tab");

	for ( size_t index = 0; index < tabs.size(); ++index )
	{
		Rml::Element* tab = tabs[index];
		Rml::Variant* value = tab->GetAttribute("value");

		if ( value && value->Get<Rml::String>() == menuName )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_DEBUG,
				"OptionsTabBarDataBinding: Setting active tab index %zu for menu %s",
				index,
				menuName.c_str()
			);

			m_TabSetElement->SetActiveTab(static_cast<int>(index));
		}
	}
}
