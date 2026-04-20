#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"

class MainMenu : public MenuPage
{
public:
	static const char* const NAME;

	MainMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;
	bool ShouldPop(const Rml::String& menuToSwapIn) const override;

private:
	void HandleShowDeveloperConsole(Rml::DataModelHandle handle, Rml::Event& event, const Rml::VariantList& args);

	MenuFrameDataBinding m_MenuFrameDataBinding;
};
