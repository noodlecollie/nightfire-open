#pragma once

#include "framework/MenuPage.h"
#include "framework/MenuEventListenerObject.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "components/ModalComponent.h"

class PauseMenu : public MenuPage
{
public:
	static const char* const NAME;

	PauseMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	struct PageModel
	{
		bool showModal = false;
	};

	void ProcessHideEvents(Rml::Event& event);
	void HandleDisconnect(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandleQuitRequest(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandleQuitConfirm(Rml::Event&, size_t buttonIndex, const Rml::Variant&);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	PageModel m_PageModel;
	ModalComponent m_Modal;
	MenuEventListenerObject m_ShowHideEventListener;
	bool m_IsQuitting = false;
};
