#include "framework/BaseCvarModel.h"
#include <RmlUi/Core/ElementDocument.h>

BaseCvarModel::BaseCvarModel(BaseMenu* parentMenu) :
	DocumentObserver(parentMenu),
	m_EventListener(this, &BaseCvarModel::HandleShowDocument)
{
}

bool BaseCvarModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	for ( const std::unique_ptr<BaseEntry>& entry : m_Entries )
	{
		BaseEntry* entryPtr = entry.get();

		const bool bindSuccess = constructor.BindFunc(
			entry->VariableName(),
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
	for ( const std::unique_ptr<BaseEntry>& entry : m_Entries )
	{
		if ( entry->Refresh() && m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(entry->VariableName());
		}
	}
}
