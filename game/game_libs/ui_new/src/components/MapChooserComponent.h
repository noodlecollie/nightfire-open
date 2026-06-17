#pragma once

#include "framework/BaseComponent.h"

class MapChooserComponent : public BaseComponent
{
public:
	explicit MapChooserComponent(BaseMenu* parentMenu, Rml::String id);

protected:
	bool ComponentLoadFromDocument(Rml::ElementDocument* document) override;
	void ComponentUnload() override;

private:
};
