#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/BaseMenu.h"
#include "templatebindings/MenuFrameDataBinding.h"

class MainMenu : public BaseMenu
{
public:
	static const char* const NAME;

	MainMenu();

	void Update(float currentTime) override;

protected:
	bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
