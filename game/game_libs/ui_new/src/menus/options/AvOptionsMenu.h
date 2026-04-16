#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/EventListenerObject.h"
#include "models/CvarModel.h"
#include "models/VideoModesModel.h"

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
		bool windowed = false;
	};

	void ProcessShowHideEvents(Rml::Event& event);
	void RefreshValuesFromCvars();

	VideoModesModel m_VideoModes;
	EventListenerObject m_ShowHideEventListener;
	PageModel m_PageModel;
	CvarModel m_CvarModel;
	Rml::DataModelHandle m_ModelHandle;
	CvarDataVar<bool>* m_DspOff = nullptr;
};
