#include "framework/DocumentObserver.h"
#include "framework/BaseMenu.h"
#include "UIDebug.h"

DocumentObserver::DocumentObserver(BaseMenu* parentMenu)
{
	ASSERT(parentMenu);
	parentMenu->RegisterDocumentObserver(this);
}
