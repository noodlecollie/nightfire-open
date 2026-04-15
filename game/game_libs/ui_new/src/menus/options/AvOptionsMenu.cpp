#include "menus/options/AvOptionsMenu.h"
#include <RmlUi/Core/ElementDocument.h>

AvOptionsMenu::AvOptionsMenu() :
	BaseOptionsMenu("av_options_menu", "resource/rml/av_options_menu.rml"),
	m_ShowHideEventListener(this, &AvOptionsMenu::ProcessShowHideEvents)
{
}

bool AvOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !BaseOptionsMenu::OnSetUpDataModelBindings(constructor) || !m_VideoModes.SetUpDataBindings(constructor) )
	{
		return false;
	}

	return true;
}

void AvOptionsMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
}

void AvOptionsMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void AvOptionsMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			m_VideoModes.Populate();
			break;
		}

		default:
		{
			break;
		}
	}
}
