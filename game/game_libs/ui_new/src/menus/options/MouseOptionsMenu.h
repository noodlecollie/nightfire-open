#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/BaseCvarModel.h"
#include "framework/CvarDataVar.h"

class MouseOptionsMenu : public BaseOptionsMenu
{
public:
	MouseOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	BaseCvarModel m_CvarModel;
	CvarDataVar<float>* m_MousePitch = nullptr;
	Rml::DataModelHandle m_ModelHandle;
};
