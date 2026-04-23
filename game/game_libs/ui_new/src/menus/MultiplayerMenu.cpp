#include "menus/MultiplayerMenu.h"
#include "rmlui/RmlUiBackend.h"

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml"),
	m_MenuFrameDataBinding(this)
{
}

bool MultiplayerMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::OnSetUpDataModelBindings(constructor) && m_MenuFrameDataBinding.SetUpDataBindings(constructor) &&
		m_ServerModel.SetUpDataBindings(constructor);
}

void MultiplayerMenu::OnEndDocumentLoaded()
{
	RmlUiBackend::StaticInstance().SetDiscoveredServerCallback(
		[this](const netadr_t& address, Rml::String&& info)
		{
			AddServerToList(address, std::move(info));
		}
	);
}

void MultiplayerMenu::OnBeginDocumentUnloaded()
{
	RmlUiBackend::StaticInstance().ClearDiscoveredServerCallback();
}

void MultiplayerMenu::AddServerToList(const netadr_t& /* address */, Rml::String&& /* info */)
{
	// TODO
}
