#pragma once

#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/DataVar.h"
#include "framework/MenuEventListenerObject.h"

class HintDisplay : private BaseMenuObserver
{
public:
	HintDisplay(BaseMenu* parentMenu, Rml::String displayElementID, Rml::String dataVarName);

	void SetHintInnerRml(const Rml::String& rml);

	Rml::String DefaultHintText() const;
	void SetDefaultHintText(Rml::String text);

protected:
	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleDocumentHide(Rml::Event& event);
	void HandleMouseEvents(Rml::Event& event);
	void SetHint(Rml::Event& event);
	void ResetHint();

	DataVar<Rml::String> m_HintText;

	MenuEventListenerObject m_DocumentListener;
	MenuEventListenerObject m_HintListener;
	Rml::String m_DisplayElementID;
	Rml::Element* m_CurrentHintElement = nullptr;
	Rml::Element* m_HintDisplayElement = nullptr;
	Rml::String m_DefaultHintText;
};
