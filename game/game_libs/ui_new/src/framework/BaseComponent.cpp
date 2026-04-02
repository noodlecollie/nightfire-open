#include "framework/BaseComponent.h"
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include "framework/BaseMenu.h"
#include "UIDebug.h"

BaseComponent::BaseComponent(BaseMenu* parentMenu, Rml::String id) :
	m_ParentMenu(parentMenu),
	m_ID(std::move(id))
{
	ASSERT(m_ParentMenu);

	ASSERTSZ(!m_ID.empty(), "Component was constructed with an empty ID");

	if ( m_ParentMenu )
	{
		m_ParentMenu->RegisterComponent(this);
	}
}

bool BaseComponent::Loaded() const
{
	return m_ComponentElement || m_StowedComponentElement.get();
}

void BaseComponent::LoadFromDocument(Rml::ElementDocument* document)
{
	if ( m_ComponentElement || m_ID.empty() )
	{
		return;
	}

	m_ComponentElement = document->GetElementById(m_ID);

	if ( !m_ComponentElement )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"BaseComponent::DocumentLoaded: Could not find component element with ID \"%s\"",
			m_ID.c_str()
		);

		return;
	}

	if ( !OnLoadFromDocument(document) )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"BaseComponent::DocumentLoaded: Component \"%s\" failed to load",
			m_ID.c_str()
		);

		OnUnload();
	}
}

void BaseComponent::Unload()
{
	if ( m_ComponentElement )
	{
		OnUnload();
		m_ComponentElement = nullptr;
	}
}

Rml::Element* BaseComponent::ComponentElement() const
{
	return m_ComponentElement;
}

Rml::Element* const* BaseComponent::ComponentElementPtrPtr() const
{
	return &m_ComponentElement;
}

bool BaseComponent::OnLoadFromDocument(Rml::ElementDocument*)
{
	return true;
}

void BaseComponent::OnUnload()
{
}

bool BaseComponent::CheckLoaded(const char* operation)
{
	if ( !Loaded() )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"%s: Called when component \"%s\" was not loaded",
			operation,
			m_ID.c_str()
		);

		return false;
	}

	return true;
}
