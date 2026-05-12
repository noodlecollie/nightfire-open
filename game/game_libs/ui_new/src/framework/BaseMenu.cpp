#include "framework/BaseMenu.h"
#include "framework/DocumentObserver.h"
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
	return OnSetUpDataModelBindings(constructor);
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
	OnBeginDocumentLoaded();

	for ( DocumentObserver* observer : m_DocObservers )
	{
		observer->DocumentLoaded(document);
	}

	OnEndDocumentLoaded();
}

void BaseMenu::DocumentUnloaded()
{
	ASSERT(m_Document);

	if ( !m_Document )
	{
		return;
	}

	OnBeginDocumentUnloaded();

	for ( DocumentObserver* observer : m_DocObservers )
	{
		observer->DocumentUnloaded(m_Document);
	}

	OnEndDocumentUnloaded();
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

void BaseMenu::OnBeginDocumentLoaded()
{
}

void BaseMenu::OnEndDocumentLoaded()
{
}

void BaseMenu::OnBeginDocumentUnloaded()
{
}

void BaseMenu::OnEndDocumentUnloaded()
{
}

bool BaseMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor&)
{
	return true;
}

void BaseMenu::RegisterDocumentObserver(DocumentObserver* observer)
{
	ASSERT(observer);

	if ( observer )
	{
		m_DocObservers.push_back(observer);
	}
}
