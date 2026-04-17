#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/Elements/ElementFormControlSelect.h>
#include "framework/EventListenerObject.h"
#include "models/CvarModel.h"
#include "models/VideoModesModel.h"
#include "components/ModalComponent.h"

class AvOptionsMenu : public BaseOptionsMenu
{
public:
	AvOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	struct PageModel
	{
		bool showModal = false;
		int currentWidth = 0;
		int currentHeight = 0;

		int newVideoModeIndex = -1;
		bool currentWindowed = false;
		bool newWindowed = false;
		bool needsApply = false;
	};

	void ProcessDocumentEvent(Rml::Event& event);
	void RefreshValuesFromCvars();
	void RefreshNeedsApply();

	void HandleApplyVideoMode(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandleApplyVideoMode();
	void HandleModalButton(bool keepNewVideoMode);

	ModalComponent m_Modal;
	VideoModesModel m_VideoModes;
	EventListenerObject m_DocumentEventListener;
	PageModel m_PageModel;
	CvarModel m_CvarModel;
	Rml::DataModelHandle m_ModelHandle;
	Rml::ElementFormControlSelect* m_ResolutionDropdown = nullptr;
	CvarDataVar<bool>* m_DspOff = nullptr;
	CvarDataVar<bool>* m_Vsync = nullptr;
	CvarAccessorObj<bool> m_FullscreenCvar;
	CvarAccessorObj<int> m_VideoModeCvar;
};
