#pragma once

#include "framework/BaseComponent.h"
#include "framework/DataVar.h"
#include "framework/MenuEventListenerObject.h"

class TooltipComponent : public BaseComponent
{
public:
	TooltipComponent(
		BaseMenu* parentMenu,
		Rml::String componentID,
		Rml::String displayElementID,
		Rml::String dataVarName
	);

	void SetHintInnerRml(const Rml::String& rml);

	Rml::String DefaultTooltipText() const;
	void SetDefaultTooltipText(Rml::String text);

	bool Enabled();
	void SetEnabled(bool enabled);

protected:
	bool ComponentLoadFromDocument(Rml::ElementDocument* document) override;
	void ComponentUnload() override;
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	void Update(float currentTime) override;

private:
	enum class TooltipType
	{
		STATIC,
		FOLLOW_CURSOR
	};

	void HandleDocumentEvents(Rml::Event& event);
	void HandleTooltipTriggerEvents(Rml::Event& event);
	void SetType(TooltipType tooltipType);
	void SetTooltip(Rml::Event& event);
	void ResetTooltip();
	void UpdateTooltipPosition(const Rml::Event& event);
	void UpdateTooltipPosition(const Rml::Vector2f& mousePos);
	bool IsShowingDisplayElement() const;

	DataVar<Rml::String> m_TooltipText;
	MenuEventListenerObject m_DocumentListener;
	MenuEventListenerObject m_TooltipListener;
	Rml::String m_DisplayElementID;
	Rml::Element* m_CurrentTooltipSourceElement = nullptr;
	Rml::Element* m_TooltipDisplayElement = nullptr;
	Rml::String m_DefaultTooltipText;
	TooltipType m_Type = TooltipType::STATIC;
	bool m_Enabled = true;
};
