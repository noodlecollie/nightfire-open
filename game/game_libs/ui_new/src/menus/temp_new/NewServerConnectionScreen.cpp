#include "menus/temp_new/NewServerConnectionScreen.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Elements/ElementProgress.h>
#include "menus/temp_new/NewPauseMenu.h"
#include "UIDebug.h"

static constexpr const char* const NAME_LOADING_PROGRESS = "loadingProgress";
static constexpr const char* const NAME_DOWNLOAD_PROGRESS = "downloadProgress";

const char* const NewServerConnectionScreen::NAME = "new_server_connection_screen";

NewServerConnectionScreen::NewServerConnectionScreen() :
	BaseMenu(NAME, "server_connection_screen.rml"),
	m_CvarModel(this)
{
	m_CvarLoading = m_CvarModel.AddEntry<float>(NAME_LOADING_PROGRESS, "scr_loading");
	m_CvarDownload = m_CvarModel.AddEntry<float>(NAME_DOWNLOAD_PROGRESS, "scr_download");
}

void NewServerConnectionScreen::Update(float currentTime)
{
	BaseMenu::Update(currentTime);

	if ( !IsDocumentVisible() || !IsModelLoaded() )
	{
		return;
	}

	if ( m_Connecting )
	{
		m_CvarModel.RefreshAll();
	}
}

void NewServerConnectionScreen::Connect(const char* server, bool isBackground)
{
	m_Connecting = true;
	ClearContentArea();

	if ( server )
	{
		AppendContentLine(
			Rml::CreateString(
				"Connecting to %sserver: %s",
				isBackground ? "background " : "",
				(*server) ? server : "<unknown>"
			)
		);
	}
	else
	{
		AppendContentLine(Rml::CreateString("Connecting to local %sserver", isBackground ? "background " : ""));
	}
}

void NewServerConnectionScreen::ParseServerInfo(const char* /* server */)
{
	AppendContentLine("Parsing server info");
}

void NewServerConnectionScreen::Precache(const char* mapFileName)
{
	if ( mapFileName && mapFileName[0] )
	{
		AppendContentLine(Rml::CreateString("Loading map: %s", mapFileName));
	}

	AppendContentLine("Precaching resources");
}

void NewServerConnectionScreen::Download(
	const char* fileName,
	const char* /* serverName */,
	int current,
	int total,
	const char* comment
)
{
	Rml::String text;
	Rml::FormatString(text, "[%d/%d] Downloaded %s", current, total, fileName);

	if ( !comment || !comment[0] )
	{
		text += Rml::CreateString(" (%s)", comment);
	}

	AppendContentLine(text);
}

void NewServerConnectionScreen::DownloadEnd()
{
	AppendContentLine("Resource download finished");
}

void NewServerConnectionScreen::Connected()
{
	AppendContentLine("Joined server");
	m_Connecting = false;
}

void NewServerConnectionScreen::Disconnect()
{
	m_Connecting = false;
	AppendContentLine("Disconnected from server");
}

void NewServerConnectionScreen::ChangeLevel(bool /* isBackground */)
{
	m_Connecting = true;

	// This seems to just be fired when we change level in single player,
	// so probably nothing to do other than clearing the content just in case.
	ClearContentArea();
}

void NewServerConnectionScreen::OnDocumentLoaded()
{
	BaseMenu::OnDocumentLoaded();

	m_ContentElement = Document()->GetElementById("content");
	m_ProgressElement = rmlui_dynamic_cast<Rml::ElementProgress*>(Document()->GetElementById("progress_bar"));

	ASSERT(m_ContentElement);
	ASSERT(m_ProgressElement);
}

void NewServerConnectionScreen::OnDocumentUnloaded()
{
	m_ContentElement = nullptr;
	m_ProgressElement = nullptr;
	m_Connecting = false;

	BaseMenu::OnDocumentUnloaded();
}

void NewServerConnectionScreen::ClearContentArea()
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

void NewServerConnectionScreen::AppendContentLine(const Rml::String& text)
{
	static constexpr int MAX_TEXT_LINES = 30;

	if ( !m_ContentElement )
	{
		ASSERT(false);
		return;
	}

	Rml::Log::Message(Rml::Log::Type::LT_DEBUG, "NewServerConnectionScreen: %s", text.c_str());

	Rml::ElementPtr elem = Document()->CreateElement("p");
	elem->AppendChild(Document()->CreateTextNode(text));
	m_ContentElement->AppendChild(std::move(elem));

	while ( m_ContentElement->GetNumChildren() > MAX_TEXT_LINES )
	{
		m_ContentElement->RemoveChild(m_ContentElement->GetFirstChild());
	}
}
