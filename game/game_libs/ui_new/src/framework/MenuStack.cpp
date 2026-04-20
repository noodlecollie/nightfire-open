#include "framework/MenuStack.h"
#include "framework/MenuDirectory.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Log.h>
#include <algorithm>
#include "UIDebug.h"

MenuStack::MenuStack(MenuDirectory* directory) :
	m_Directory(directory)
{
	ASSERT(directory);
}

bool MenuStack::Push(const MenuDirectoryEntry* menu)
{
	if ( !menu || Top() == menu )
	{
		return false;
	}

	SetTopDocumentVisible(false);
	m_Stack.push_back(menu);
	SetTopDocumentVisible(true);

	return true;
}

const MenuDirectoryEntry* MenuStack::Pop()
{
	if ( m_Stack.empty() )
	{
		ASSERT(false);
		return nullptr;
	}

	SetTopDocumentVisible(false);

	const MenuDirectoryEntry* menu = m_Stack.back();
	m_Stack.pop_back();

	SetTopDocumentVisible(true);

	return menu;
}

void MenuStack::Update(float currentTime)
{
	// Update all menus from the front to the back (ie. bottom to top)
	// of the stack. This means that the most recently updated menu
	// will be the one on the top.
	for ( const MenuDirectoryEntry* entry : m_Stack )
	{
		entry->menuPtr->Update(currentTime);
	}
}

void MenuStack::HandleRequests()
{
	const MenuDirectoryEntry* top = Top();

	if ( !top )
	{
		return;
	}

	const MenuRequest* request = top->menuPtr->CurrentRequest();

	if ( request )
	{
		HandleTopMenuRequest(*request);
		top->menuPtr->ClearCurrentRequest();
	}
}

const MenuDirectoryEntry* MenuStack::Top() const
{
	return (!m_Stack.empty()) ? m_Stack.back() : nullptr;
}

bool MenuStack::IsEmpty() const
{
	return m_Stack.empty();
}

size_t MenuStack::Size() const
{
	return m_Stack.size();
}

void MenuStack::SetTopDocumentVisible(bool visible)
{
	if ( m_Stack.empty() )
	{
		return;
	}

	const MenuDirectoryEntry* entry = m_Stack.back();
	Rml::ElementDocument* document = entry->document;

	if ( !document )
	{
		return;
	}

	if ( visible )
	{
		document->Show();
	}
	else
	{
		// If there are any pending requests, cancel them.
		entry->menuPtr->ClearCurrentRequest();

		document->Hide();
	}
}

void MenuStack::HandleTopMenuRequest(const MenuRequest& request)
{
	switch ( request.requestType )
	{
		case MenuRequestType::PushMenu:
		{
			Rml::String menuName;

			if ( !request.args.empty() )
			{
				request.args[0].GetInto(menuName);
			}

			HandlePushMenuRequest(menuName);
			break;
		}

		case MenuRequestType::PopMenu:
		{
			Rml::String menuName;

			if ( !request.args.empty() )
			{
				request.args[0].GetInto(menuName);
			}

			HandlePopMenuRequest(menuName);
			break;
		}

		default:
		{
			ASSERT(false);
			break;
		}
	}
}

void MenuStack::HandlePushMenuRequest(const Rml::String& name)
{
	if ( name.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Ignoring menu push request which specified no menu name");
		return;
	}

	const MenuDirectoryEntry* entry = m_Directory->GetMenuEntry(name);

	if ( !entry )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Ignoring menu push request for non-existent menu \"%s\"",
			name.c_str()
		);

		return;
	}

	Push(entry);
}

void MenuStack::HandlePopMenuRequest(const Rml::String& name)
{
	if ( name.empty() )
	{
		Pop();
		return;
	}

	const MenuDirectoryEntry* entry = m_Directory->GetMenuEntry(name);

	if ( !entry )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Ignoring menu pop requesting to swap in non-existent menu \"%s\"",
			name.c_str()
		);

		return;
	}

	Pop();
	Push(entry);
}
