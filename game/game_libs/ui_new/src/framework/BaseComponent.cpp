#include "framework/BaseComponent.h"
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include "CRTLib/crtlib.h"
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
	return m_ComponentElement;
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

	LoadParams();

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

void BaseComponent::AddParamSpec(Rml::String name, Rml::Variant defaultValue)
{
	// Should be called before the component is loaded.
	ASSERT(!m_ComponentElement);
	ASSERT(!name.empty());

	if ( !name.empty() )
	{
		m_ComponentParamSpec.insert({std::move(name), std::move(defaultValue)});
	}
}

bool BaseComponent::OnLoadFromDocument(Rml::ElementDocument*)
{
	return true;
}

void BaseComponent::OnUnload()
{
	m_ComponentParams.clear();
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

void BaseComponent::LoadParams()
{
	if ( !CheckLoaded("BaseComponent::LoadParams") )
	{
		return;
	}

	m_ComponentParams = m_ComponentParamSpec;

	for ( const auto& it : m_ComponentElement->GetAttributes() )
	{
		static constexpr size_t PREFIX_LENGTH = sizeof("param-") - 1;

		const Rml::String& key = it.first;

		if ( Q_strncmp(key.c_str(), "param-", PREFIX_LENGTH) != 0 )
		{
			continue;
		}

		const Rml::String paramName = key.substr(PREFIX_LENGTH);
		auto paramIt = m_ComponentParams.find(paramName);

		if ( paramIt == m_ComponentParams.end() )
		{
			continue;
		}

		paramIt->second = it.second;

		Rml::Log::Message(
			Rml::Log::Type::LT_DEBUG,
			"Component %s received param: %s=\"%s\"",
			m_ID.c_str(),
			paramIt->first.c_str(),
			paramIt->second.Get<Rml::String>().c_str()
		);
	}
}
