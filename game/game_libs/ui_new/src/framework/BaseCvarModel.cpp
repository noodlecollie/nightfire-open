#include "framework/BaseCvarModel.h"
#include <RmlUi/Core/ElementDocument.h>

BaseCvarModel::BaseCvarModel(BaseMenu* parentMenu) :
	DocumentObserver(parentMenu),
	m_EventListener(this, &BaseCvarModel::HandleShowDocument)
{
}

bool BaseCvarModel::SetChangeListener(const Rml::String& name, ChangeCallbackFunc cb)
{
	auto it = m_Entries.find(name);

	if ( it == m_Entries.end() )
	{
		return false;
	}

	it->second->changeCallback = std::move(cb);
	return true;
}

bool BaseCvarModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
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

void BaseCvarModel::DocumentLoaded(Rml::ElementDocument* document)
{
	document->AddEventListener(Rml::EventId::Show, &m_EventListener);
}

void BaseCvarModel::DocumentUnloaded(Rml::ElementDocument* document)
{
	document->RemoveEventListener(Rml::EventId::Show, &m_EventListener);
}

void BaseCvarModel::HandleShowDocument(Rml::Event&)
{
	RefreshAll();
}

void BaseCvarModel::RefreshAll()
{
	for ( const auto& it : m_Entries )
	{
		if ( it.second->Refresh() && m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(it.second->VariableName());

			if ( it.second->changeCallback )
			{
				Rml::Variant val;
				it.second->Get(val);
				it.second->changeCallback(val);
			}
		}
	}
}
