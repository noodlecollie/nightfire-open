#include "components/MapChooserComponent.h"

MapChooserComponent::MapChooserComponent(BaseMenu* parentMenu, Rml::String id) :
	BaseComponent(parentMenu, id)
{
}

bool MapChooserComponent::ComponentLoadFromDocument(Rml::ElementDocument*)
{
	return true;
}

void MapChooserComponent::ComponentUnload()
{
}
