#include "models/CvarModel.h"
#include <RmlUi/Core/ElementDocument.h>

CvarModel::CvarModel(BaseMenu* parentMenu) :
	DocumentObserver(parentMenu),
	m_EventListener(this, &CvarModel::HandleShowDocument)
{
}

bool CvarModel::SetChangeListener(const Rml::String& name, ChangeCallbackFunc cb)
{
	auto it = m_Entries.find(name);

	if ( it == m_Entries.end() )
	{
		return false;
	}

	it->second->changeCallback = std::move(cb);
	return true;
}

bool CvarModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	for ( const auto& it : m_Entries )
	{
		BaseEntry* entryPtr = it.second.get();

		const bool bindSuccess = constructor.BindFunc(
			it.second->VariableName(),
			[entryPtr](Rml::Variant& outVal)
			{
				entryPtr->Get(outVal);
			},
			[entryPtr](const Rml::Variant& inVal)
			{
				entryPtr->Set(inVal);
			}
		);

		if ( !bindSuccess )
		{
			return false;
		}
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

bool CvarModel::Refresh(const Rml::String& name)
{
	const auto it = m_Entries.find(name);

	if ( it == m_Entries.end() )
	{
		return false;
	}

	return Refresh(*(it->second));
}

void CvarModel::DocumentLoaded(Rml::ElementDocument* document)
{
	document->AddEventListener(Rml::EventId::Show, &m_EventListener);
}

void CvarModel::DocumentUnloaded(Rml::ElementDocument* document)
{
	document->RemoveEventListener(Rml::EventId::Show, &m_EventListener);
}

void CvarModel::HandleShowDocument(Rml::Event&)
{
	RefreshAll();
}

void CvarModel::RefreshAll()
{
	for ( const auto& it : m_Entries )
	{
		Refresh(*(it.second));
	}
}

bool CvarModel::Refresh(BaseEntry& entry)
{
	if ( !entry.Refresh() )
	{
		return false;
	}

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(entry.VariableName());
	}

	if ( entry.changeCallback )
	{
		Rml::Variant val;
		entry.Get(val);
		entry.changeCallback(val);
	}

	return true;
}
