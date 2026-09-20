#pragma once

#include "menus/temp_new/NewBaseOptionsMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/Elements/ElementFormControlSelect.h>
#include "framework/MenuEventListenerObject.h"
#include "models/CvarModel.h"
#include "models/VideoModesModel.h"
#include "components/ModalComponent.h"
#include "components/TooltipComponent.h"

class NewAvOptionsMenu : public NewBaseOptionsMenu
{
public:
	NewAvOptionsMenu();

	void Update(float currentTime) override;

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	bool ShouldPop(const Rml::String& menuToSwapIn) override;

	void OnDocumentLoaded() override;
	void OnDocumentUnloaded() override;

private:
	struct PageModel
	{
		bool showModal = false;
		float modalExpiry = 0.0f;
		int modalTimeRemaining = 0;

		int currentWidth = 0;
		int currentHeight = 0;

		int newVideoModeIndex = -1;
		bool currentWindowed = false;
		bool newWindowed = false;
		bool needsApply = false;
		Rml::String menuTransparencyControl;
	};

	struct RevertInfo
	{
		bool wasWindowed = false;
		int width = 0;
		int height = 0;
	};

	void ProcessDocumentEvent(Rml::Event& event);
	void ProcessSliderEventForMenuTransparency(Rml::Event& event);
	void RefreshValuesFromCvars();
	void RefreshNeedsApply();

	void HandleApplyVideoMode(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandleApplyVideoMode();
	void HandleVideoModeConfirm(bool keepNewVideoMode);
	void HandleApplyVideoModeOnExit(bool applyVideoMode);
	void CreateRevertInfo();
	void ApplyRevertInfo();
	void ApplyVideoSettings(int vidMode, bool windowed);

	ModalComponent m_Modal;
	TooltipComponent m_TooltipComponent;
	VideoModesModel m_VideoModes;
	MenuEventListenerObject m_DocumentEventListener;
	MenuEventListenerObject m_SliderEventListener;
	PageModel m_PageModel;
	CvarModel m_CvarModel;
	Rml::ElementFormControlSelect* m_ResolutionDropdown = nullptr;
	CvarDataVar<bool>* m_DspOff = nullptr;
	CvarDataVar<bool>* m_Vsync = nullptr;
	CvarAccessorObj<bool> m_FullscreenCvar;
	CvarAccessorObj<int> m_VideoModeCvar;
	std::unique_ptr<RevertInfo> m_RevertInfo;
	bool m_PendingPop = false;
	Rml::String m_MenuToSwitchTo;
};
