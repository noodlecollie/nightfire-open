#pragma once

#include "framework/BaseMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/MenuEventListenerObject.h"

// A menu which assumes that the entire RML page has a data model,
// and which automatically implements pushMenu and popMenu.
class MenuPage : public BaseMenu
{
public:
	enum class ActionOnEscape
	{
		NONE,
		POP,
		SWITCH_FOCUS
	};

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	ActionOnEscape GetActionOnEscapeKey() const;
	void SetActionOnEscapeKey(ActionOnEscape action);

protected:
	MenuPage(const char* name, const char* rmlFilePath);

	void RequestPush(Rml::String menuToPush);
	void RequestPop(Rml::String menuToSwapIn = Rml::String());
	void RequestCutStack(size_t newSize, Rml::String menuToSwapIn = Rml::String());
	void RequestSwitchFocus(Rml::String target, Rml::String newMenu = Rml::String());

	virtual bool ShouldAllowMenuRequest(const Rml::String& targetToSwitchTo);

private:
	void ProcessKeyEvent(Rml::Event& event);
	void HandlePushMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandlePopMenu(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandleCutStack(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void HandleSwitchFocus(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);
	void ProcessActionOnEscape(const Rml::String& targetToSwitchTo);

	MenuEventListenerObject m_KeyEventListener;
	ActionOnEscape m_ActionOnEscape = ActionOnEscape::POP;
};
