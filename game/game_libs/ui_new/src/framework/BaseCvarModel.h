#pragma once

#include <memory>
#include <vector>
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/CvarDataVar.h"
#include "framework/DocumentObserver.h"
#include "framework/EventListenerObject.h"

class BaseCvarModel : public DocumentObserver
{
public:
	explicit BaseCvarModel(BaseMenu* parentMenu);

	template<typename T>
	void AddEntry(const char* name, const char* cvarName, T defaultValue = T())
	{
		ASSERTSZ(!m_ModelHandle, "Cannot add new entry once data has been bound");

		if ( m_ModelHandle )
		{
			return;
		}

		std::unique_ptr<BaseEntry> entry(new Entry<T>(name, cvarName, std::move(defaultValue)));
		m_Entries.push_back(std::move(entry));
	}

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor);

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	struct BaseEntry
	{
		virtual ~BaseEntry() = default;
		virtual const char* VariableName() const = 0;
		virtual bool Refresh() = 0;
		virtual void Get(Rml::Variant& outVal) const = 0;
		virtual void Set(const Rml::Variant& inVal) = 0;
	};

	template<typename T>
	struct Entry : public BaseEntry
	{
		CvarDataVar<T> var;

		Entry(const char* name, const char* cvarName, T defaultValue) :
			var(name, cvarName, std::move(defaultValue))
		{
		}

		bool Refresh() override
		{
			return var.Refresh();
		}

		const char* VariableName() const override
		{
			return var.Name();
		}

		void Get(Rml::Variant& outVal) const
		{
			outVal = Rml::Variant(var.CachedValue());
		}

		void Set(const Rml::Variant& inVal)
		{
			var.SetValue(inVal.Get<T>());
		}
	};

	void HandleShowDocument(Rml::Event& event);
	void RefreshAll();

	std::vector<std::unique_ptr<BaseEntry>> m_Entries;
	Rml::DataModelHandle m_ModelHandle;
	EventListenerObject m_EventListener;
};
