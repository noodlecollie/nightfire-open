#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/KeyBindingModel.h"

class OptionsMenu : public MenuPage
{
public:
	OptionsMenu();

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	static constexpr const char* const TAB_GAMEPLAY = "gameplay";
	static constexpr const char* const TAB_KEYS = "keys";
	static constexpr const char* const TAB_MOUSE = "mouse";
	static constexpr const char* const TAB_AV = "av";
	static constexpr size_t INVALID_ROW = ~static_cast<size_t>(0);

	struct PageModel
	{
		Rml::String activeTab = TAB_GAMEPLAY;
	};

	void HandleRebindKeyEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments);
	void HandleRebindKeyEvent(const Rml::String& consoleCommand, int bindIndex);
	void ResetRebindingRow();

	MenuFrameDataBinding m_MenuFrameDataBinding;
	KeyBindingModel m_KeyBindings;
	PageModel m_PageModel;
	size_t m_RebindingRow = INVALID_ROW;
	Rml::DataModelHandle m_ModelHandle;
};
