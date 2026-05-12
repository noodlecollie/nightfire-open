#include "menus/ServerConnectionScreen.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Elements/ElementProgress.h>
#include "menus/PauseMenu.h"
#include "UIDebug.h"

static constexpr const char* const NAME_LOADING_PROGRESS = "loadingProgress";
static constexpr const char* const NAME_DOWNLOAD_PROGRESS = "downloadProgress";

const char* const ServerConnectionScreen::NAME = "server_connection_screen";

ServerConnectionScreen::ServerConnectionScreen() :
	BaseMenu(NAME, "resource/rml/server_connection_screen.rml"),
	m_CvarModel(this)
{
	m_CvarLoading = m_CvarModel.AddEntry<float>(NAME_LOADING_PROGRESS, "scr_loading");
	m_CvarDownload = m_CvarModel.AddEntry<float>(NAME_DOWNLOAD_PROGRESS, "scr_download");
}

void ServerConnectionScreen::Update(float)
{
	if ( !IsDocumentVisible() || !m_ModelHandle )
	{
		return;
	}

	m_CvarModel.RefreshAll();
}

void ServerConnectionScreen::ReceiveConnectionProgress_Connect(const Rml::String& server)
{
	ClearContentArea();

	Rml::String text;
	Rml::FormatString(text, "Connecting to server: %s", server.c_str());

	AppendContentLine(text);
}

void ServerConnectionScreen::ReceiveConnectionProgress_ParseServerInfo(const Rml::String& /* server */)
{
	AppendContentLine("Parsing server info");
}

void ServerConnectionScreen::ReceiveConnectionProgress_Precache()
{
	AppendContentLine("Precaching resources");
}

void ServerConnectionScreen::ReceiveConnectionProgress_Download(
	const Rml::String& fileName,
	const Rml::String& /* serverName */,
	int current,
	int total,
	const Rml::String& comment
)
{
	Rml::String text;
	Rml::FormatString(text, "[%d/%d] Downloaded %s", current, total, fileName.c_str());

	if ( !comment.empty() )
	{
		Rml::String extra;
		Rml::FormatString(extra, " (%s)", comment.c_str());
		text += extra;
	}

	AppendContentLine(text);
}

void ServerConnectionScreen::ReceiveConnectionProgress_DownloadEnd()
{
	AppendContentLine("Resource download finished");
}

void ServerConnectionScreen::ReceiveConnectionProgress_Connected()
{
	AppendContentLine("Joined server");

	Rml::Dictionary options;
	options.insert({SwitchFocusRequest::OPTION_TARGET, Rml::Variant(Rml::String("game"))});
	options.insert({SwitchFocusRequest::OPTION_NEW_MENU, Rml::Variant(Rml::String(PauseMenu::NAME))});

	SetCurrentRequest(MenuRequestType::SwitchFocus, std::move(options));
}

void ServerConnectionScreen::ReceiveConnectionProgress_Disconnect()
{
	AppendContentLine("Disconnected from server");
}

void ServerConnectionScreen::ReceiveConnectionProgress_ChangeLevel()
{
	// This seems to just be fired when we change level in single player,
	// so probably nothing to do other than clearing the content.
	ClearContentArea();
}

bool ServerConnectionScreen::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !m_CvarModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void ServerConnectionScreen::OnEndDocumentLoaded()
{
	BaseMenu::OnEndDocumentLoaded();

	m_ContentElement = Document()->GetElementById("content");
	m_ProgressElement = dynamic_cast<Rml::ElementProgress*>(Document()->GetElementById("progress_bar"));

	ASSERT(m_ContentElement);
	ASSERT(m_ProgressElement);
}

void ServerConnectionScreen::OnBeginDocumentUnloaded()
{
	m_ContentElement = nullptr;
	m_ProgressElement = nullptr;

	BaseMenu::OnBeginDocumentUnloaded();
}

void ServerConnectionScreen::ClearContentArea()
{
	if ( !m_ContentElement )
	{
		ASSERT(false);
		return;
	}

	for ( Rml::Element* child = m_ContentElement->GetLastChild(); child; child = m_ContentElement->GetLastChild() )
	{
		m_ContentElement->RemoveChild(child);
	}
}

void ServerConnectionScreen::AppendContentLine(const Rml::String& text)
{
	static constexpr int MAX_TEXT_LINES = 30;

	if ( !m_ContentElement )
	{
		ASSERT(false);
		return;
	}

	Rml::Log::Message(Rml::Log::Type::LT_DEBUG, "ServerConnectionScreen: %s", text.c_str());

	Rml::ElementPtr elem = Document()->CreateElement("p");
	elem->AppendChild(Document()->CreateTextNode(text));
	m_ContentElement->AppendChild(std::move(elem));

	while ( m_ContentElement->GetNumChildren() > MAX_TEXT_LINES )
	{
		m_ContentElement->RemoveChild(m_ContentElement->GetFirstChild());
	}
}
