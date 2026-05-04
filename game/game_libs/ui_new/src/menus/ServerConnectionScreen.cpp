#include "menus/ServerConnectionScreen.h"
#include <RmlUi/Core/ElementDocument.h>

const char* const ServerConnectionScreen::NAME = "server_connection_screen";

ServerConnectionScreen::ServerConnectionScreen() :
	BaseMenu(NAME, "resource/rml/server_connection_screen.rml")
{
}

void ServerConnectionScreen::ReceiveConnectionProgress_Connect(const Rml::String& server)
{
	Rml::Element* list = Document()->GetElementById("debugging_list");

	if ( !list )
	{
		return;
	}

	Rml::String content;
	Rml::FormatString(content, "[Connect] Server: '%s'", Rml::StringUtilities::EncodeRml(server).c_str());

	Rml::ElementPtr item = Document()->CreateElement("li");
	item->SetInnerRML(content);
	list->AppendChild(std::move(item));
}
