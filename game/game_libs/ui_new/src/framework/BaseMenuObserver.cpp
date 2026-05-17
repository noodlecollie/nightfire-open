#include "framework/BaseMenuObserver.h"
#include "framework/BaseMenu.h"
#include "UIDebug.h"

BaseMenuObserver::BaseMenuObserver(BaseMenu* parentMenu)
{
	ASSERT(parentMenu);
	parentMenu->RegisterObserver(this);
}

void BaseMenuObserver::DocumentLoaded(Rml::ElementDocument*)
{
}

void BaseMenuObserver::DocumentUnloaded(Rml::ElementDocument*)
{
}
