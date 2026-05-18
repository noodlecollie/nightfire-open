#include "framework/BaseMenu.h"
#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/ElementDocument.h>
#include "UIDebug.h"

BaseMenu::BaseMenu(const char* name, const char* rmlFilePath) :
	m_Name(name),
	m_RmlFilePath(rmlFilePath)
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

Rml::ElementDocument* BaseMenu::Document() const
{
	return m_Document;
}

const MenuRequest* BaseMenu::CurrentRequest() const
{
	return m_Request.get();
}

void BaseMenu::SetCurrentRequest(MenuRequestType requestType, Rml::Dictionary options)
{
	m_Request.reset(new MenuRequest(requestType, std::move(options)));
}

void BaseMenu::ClearCurrentRequest()
{
	m_Request.reset();
}

bool BaseMenu::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	for ( BaseMenuObserver* observer : m_MenuObservers )
	{
		if ( !observer->SetUpDataModelBindings(constructor) )
		{
			return false;
		}
	}

	if ( !OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void BaseMenu::DocumentLoaded(Rml::ElementDocument* document)
{
	ASSERT(document);
	ASSERT(!m_Document);

	if ( !document || m_Document )
	{
		return;
	}

	m_Document = document;

	for ( BaseMenuObserver* observer : m_MenuObservers )
	{
		observer->DocumentLoaded(document);
	}

	OnDocumentLoaded();
}

void BaseMenu::DocumentUnloaded()
{
	ASSERT(m_Document);

	if ( !m_Document )
	{
		return;
	}

	OnDocumentUnloaded();

	for ( BaseMenuObserver* observer : m_MenuObservers )
	{
		observer->DocumentUnloaded(m_Document);
	}

	m_Document = nullptr;
}

void BaseMenu::Update(float)
{
}

bool BaseMenu::IsDocumentVisible() const
{
	Rml::ElementDocument* document = Document();
	return document && document->IsVisible();
}

void BaseMenu::OnDocumentLoaded()
{
}

void BaseMenu::OnDocumentUnloaded()
{
}

bool BaseMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor&)
{
	return true;
}

void BaseMenu::RegisterObserver(BaseMenuObserver* observer)
{
	ASSERT(observer);

	if ( observer )
	{
		m_MenuObservers.push_back(observer);
	}
}
