#pragma once

#include "framework/MenuPage.h"
#include <RmlUi/Core/EventListener.h>
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/KeyBindingModel.h"
#include "components/ModalComponent.h"
#include "framework/EventListenerObject.h"

class OptionsMenu : public MenuPage
{
public:
	OptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	static constexpr const char* const TAB_GAMEPLAY = "gameplay";
	static constexpr const char* const TAB_KEYS = "keys";
	static constexpr const char* const TAB_MOUSE = "mouse";
	static constexpr const char* const TAB_AV = "av";
	static constexpr size_t INVALID_ROW = ~static_cast<size_t>(0);

	struct PageModel
	{
		Rml::String activeTab = TAB_GAMEPLAY;
		bool showModal = false;
	};

	void ProcessShowHideEvents(Rml::Event& event);
	void ProcessKeyEvents(Rml::Event& event);
	void HandleRebindKeyEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments);
	void HandleRebindKeyEvent(const Rml::String& consoleCommand, int bindIndex);
	void ResetRebindingRow();
	void ShowModal(bool show);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	KeyBindingModel m_KeyBindings;
	PageModel m_PageModel;
	size_t m_RebindingRow = INVALID_ROW;
	Rml::DataModelHandle m_ModelHandle;
	ModalComponent m_Modal;
	EventListenerObject m_ShowHideEventListener;
	EventListenerObject m_KeyEventListener;
};
