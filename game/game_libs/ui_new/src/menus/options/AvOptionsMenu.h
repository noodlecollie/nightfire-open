#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include "framework/EventListenerObject.h"
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
	void ProcessShowHideEvents(Rml::Event& event);

	VideoModesModel m_VideoModes;
	EventListenerObject m_ShowHideEventListener;
};
