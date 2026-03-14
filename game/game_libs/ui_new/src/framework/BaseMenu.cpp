#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/ElementDocument.h>
#include "UIDebug.h"

BaseMenu::BaseMenu(const char* name, const char* rmlFilePath, size_t flags) :
	m_Name(name),
	m_RmlFilePath(rmlFilePath),
	m_AttrFlags(flags)
{
	ASSERT(m_Name);
	ASSERT(m_RmlFilePath);
}

BaseMenu::~BaseMenu()
{
}

const char* BaseMenu::Name() const
{
	return m_Name;
}

const char* BaseMenu::RmlFilePath() const
{
	return m_RmlFilePath;
}

const MenuRequest* BaseMenu::CurrentRequest() const
{
	return m_Request.get();
}

void BaseMenu::SetCurrentRequest(MenuRequestType requestType, const Rml::VariantList& args)
{
	m_Request.reset(new MenuRequest(requestType, args));
}

void BaseMenu::ClearCurrentRequest()
{
	m_Request.reset();
}

bool BaseMenu::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	if ( m_AttrFlags & MenuAttrRegisterPushPop )
	{
		constructor.BindEventCallback("push_menu", &BaseMenu::HandlePushMenu, this);
		constructor.BindEventCallback("pop_menu", &BaseMenu::HandlePopMenu, this);
	}

	return SetUpDataBindingsInternal(constructor);
}

void BaseMenu::DocumentLoaded(Rml::ElementDocument* document)
{
	document->AddEventListener(Rml::EventId::Keydown, this);
	document->AddEventListener(Rml::EventId::Keyup, this);

	DocumentLoadedInternal(document);
}

void BaseMenu::DocumentUnloaded(Rml::ElementDocument* document)
{
	DocumentUnloadedInternal(document);

	document->RemoveEventListener(Rml::EventId::Keydown, this);
	document->RemoveEventListener(Rml::EventId::Keyup, this);
}

void BaseMenu::Update(float)
{
}

void BaseMenu::ProcessEvent(Rml::Event& event)
{
	if ( m_AttrFlags & MenuAttrPopOnEscape )
	{
		switch ( event.GetId() )
		{
			case Rml::EventId::Keydown:
			{
				const int keyId = event.GetParameter<int>("key_identifier", 0);

				if ( keyId == Rml::Input::KI_ESCAPE )
				{
					event.StopPropagation();
					SetCurrentRequest(MenuRequestType::PopMenu);
				}

				break;
			}

			default:
			{
				break;
			}
		}
	}
}

bool BaseMenu::SetUpDataBindingsInternal(Rml::DataModelConstructor&)
{
	return true;
}

void BaseMenu::DocumentLoadedInternal(Rml::ElementDocument*)
{
}

void BaseMenu::DocumentUnloadedInternal(Rml::ElementDocument*)
{
}

void BaseMenu::HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PushMenu, args);
}

void BaseMenu::HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	SetCurrentRequest(MenuRequestType::PopMenu, args);
}
