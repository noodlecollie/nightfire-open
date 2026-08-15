#pragma once

#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/DataVar.h"
#include "framework/MenuEventListenerObject.h"

class MenuFrameDataBinding : private BaseMenuObserver
{
public:
	MenuFrameDataBinding(BaseMenu* parentMenu);

	void SetTooltipInnerRml(const Rml::String& rml);

	Rml::String DefaultTooltipText() const;
	void SetDefaultTooltipText(Rml::String text);

	Rml::String Title() const;
	void SetTitle(Rml::String title);

protected:
	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleDocumentHide(Rml::Event& event);
	void HandleMouseEvents(Rml::Event& event);
	void SetTooltip(Rml::Event& event);
	void ResetTooltip();

	DataVar<Rml::String> m_Tooltip;
	DataVar<Rml::String> m_Title;

	MenuEventListenerObject m_DocumentListener;
	MenuEventListenerObject m_TooltipListener;
	Rml::Element* m_CurrentTooltipElement = nullptr;
	Rml::Element* m_TooltipDisplayElement = nullptr;
	Rml::String m_DefaultTooltipText;
};
