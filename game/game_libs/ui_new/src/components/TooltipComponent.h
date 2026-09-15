#pragma once

#include "framework/BaseComponent.h"
#include "framework/DataVar.h"
#include "framework/MenuEventListenerObject.h"

class TooltipComponent : public BaseComponent
{
public:
	TooltipComponent(BaseMenu* parentMenu, Rml::String displayElementID, Rml::String dataVarName);

	void SetHintInnerRml(const Rml::String& rml);

	Rml::String DefaultTooltipText() const;
	void SetDefaultTooltipText(Rml::String text);

protected:
	bool ComponentLoadFromDocument(Rml::ElementDocument* document) override;
	void ComponentUnload() override;
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	void HandleDocumentHide(Rml::Event& event);
	void HandleMouseEvents(Rml::Event& event);
	void SetTooltip(Rml::Event& event);
	void ResetTooltip();

private:
	DataVar<Rml::String> m_TooltipText;
	MenuEventListenerObject m_DocumentListener;
	MenuEventListenerObject m_TooltipListener;
	Rml::String m_DisplayElementID;
	Rml::Element* m_CurrentTooltipSourceElement = nullptr;
	Rml::Element* m_TooltipDisplayElement = nullptr;
	Rml::String m_DefaultTooltipText;
};
