#pragma once

#include <RmlUi/Core/DataModelHandle.h>

namespace Rml
{
	class ElementDocument;
	class DataModelConstructor;
}  // namespace Rml

class BaseMenu;

class BaseMenuObserver
{
public:
	// This object must live at least as long as its parent menu.
	BaseMenuObserver(BaseMenu* parentMenu);
	virtual ~BaseMenuObserver() = default;

	BaseMenu* ParentMenu() const;

protected:
	friend class BaseMenu;

	virtual void DocumentLoaded(Rml::ElementDocument* document);
	virtual void DocumentUnloaded(Rml::ElementDocument* document);
	virtual bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor);

	Rml::DataModelHandle& ModelHandle(bool assertValid = true);
	bool IsVariableDirty(const Rml::String& variableName);
	void DirtyVariable(const Rml::String& variableName);

private:
	BaseMenu* m_ParentMenu = nullptr;
};
