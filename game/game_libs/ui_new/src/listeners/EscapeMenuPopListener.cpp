#include "listeners/EscapeMenuPopListener.h"
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/ElementDocument.h>
#include "framework/BaseMenu.h"
#include "UIDebug.h"

EscapeMenuPopListener::EscapeMenuPopListener(BaseMenu* menu) :
	m_Menu(menu)
{
	ASSERT(m_Menu);
}

void EscapeMenuPopListener::Attach(Rml::ElementDocument* document)
{
	document->AddEventListener(Rml::EventId::Keydown, this);
}

void EscapeMenuPopListener::Detach(Rml::ElementDocument* document)
{
	document->RemoveEventListener(Rml::EventId::Keydown, this);
}

void EscapeMenuPopListener::ProcessEvent(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Keydown:
		{
			Rml::Input::KeyIdentifier keyId = (Rml::Input::KeyIdentifier)event.GetParameter<int>("key_identifier", 0);

			if ( keyId == Rml::Input::KI_ESCAPE )
			{
				m_Menu->SetCurrentRequest(MenuRequestType::PopMenu);
			}

			break;
		}

		default:
		{
			break;
		}
	}
}
