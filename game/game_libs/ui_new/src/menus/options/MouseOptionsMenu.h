#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "models/CvarModel.h"
#include "framework/CvarDataVar.h"

class MouseOptionsMenu : public BaseOptionsMenu
{
public:
	MouseOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	CvarModel m_CvarModel;
	CvarDataVar<float>* m_MousePitch = nullptr;
	Rml::DataModelHandle m_ModelHandle;
};
