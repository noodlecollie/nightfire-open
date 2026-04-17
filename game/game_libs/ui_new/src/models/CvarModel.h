#pragma once

#include <memory>
#include <unordered_map>
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/CvarDataVar.h"
#include "framework/DocumentObserver.h"
#include "framework/EventListenerObject.h"

class CvarModel : public DocumentObserver
{
public:
	using ChangeCallbackFunc = std::function<void(const Rml::Variant& /*newVal*/)>;

	explicit CvarModel(BaseMenu* parentMenu);

	template<typename T>
	CvarDataVar<T>* AddEntry(Rml::String name, Rml::String cvarName, T defaultValue = T())
	{
		ASSERTSZ(!m_ModelHandle, "Cannot add new entry once data has been bound");

		if ( m_ModelHandle )
		{
			return nullptr;
		}

		Entry<T>* heapEntry = new Entry<T>(name, std::move(cvarName), std::move(defaultValue));
		m_Entries.insert({name, std::unique_ptr<BaseEntry>(heapEntry)});

		return &heapEntry->var;
	}

	bool SetChangeListener(const Rml::String& name, ChangeCallbackFunc cb);
	bool SetUpDataBindings(Rml::DataModelConstructor& constructor);
	bool Refresh(const Rml::String& name);

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	struct BaseEntry
	{
		ChangeCallbackFunc changeCallback;

		virtual ~BaseEntry() = default;
		virtual const Rml::String& VariableName() const = 0;
		virtual bool Refresh() = 0;
		virtual void Get(Rml::Variant& outVal) const = 0;
		virtual void Set(const Rml::Variant& inVal) = 0;
	};

	template<typename T>
	struct Entry : public BaseEntry
	{
		CvarDataVar<T> var;

		Entry(Rml::String name, Rml::String cvarName, T defaultValue) :
			var(std::move(name), std::move(cvarName), std::move(defaultValue))
		{
		}

		bool Refresh() override
		{
			return var.Refresh();
		}

		const Rml::String& VariableName() const override
		{
			return var.Name();
		}

		void Get(Rml::Variant& outVal) const
		{
			outVal = Rml::Variant(var.CachedValue());
		}

		void Set(const Rml::Variant& inVal)
		{
			if ( var.SetValue(inVal.Get<T>()) && changeCallback )
			{
				changeCallback(Rml::Variant(var.CachedValue()));
			}
		}
	};

	void HandleShowDocument(Rml::Event& event);
	void RefreshAll();
	bool Refresh(BaseEntry& entry);

	std::unordered_map<Rml::String, std::unique_ptr<BaseEntry>> m_Entries;
	Rml::DataModelHandle m_ModelHandle;
	EventListenerObject m_EventListener;
};
