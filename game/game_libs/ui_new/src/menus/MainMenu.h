#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>
#include "framework/BaseMenu.h"
#include "framework/DataBinding.h"

class MainMenu : public BaseMenu
{
public:
	static const char* const NAME;

	MainMenu();
	void Update(float currentTime) override;
	bool SetUpDataBindings(Rml::Context* context) override;

private:
	void SetTooltip(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList&);
	void ClearTooltip(Rml::DataModelHandle handle, Rml::Event&, const Rml::VariantList&);

	DataBinding<Rml::String> m_Tooltip;
};
