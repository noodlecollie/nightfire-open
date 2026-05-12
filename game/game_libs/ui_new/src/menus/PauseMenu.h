#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "components/ModalComponent.h"

class PauseMenu : public MenuPage
{
public:
	static const char* const NAME;

	PauseMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	struct PageModel
	{
		bool showModal = false;
	};

	void ProcessShowHideEvents(Rml::Event& event);
	void HandleDisconnect(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandleQuitRequest(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandleQuitConfirm(Rml::Event&, size_t buttonIndex, const Rml::Variant&);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	PageModel m_PageModel;
	Rml::DataModelHandle m_ModelHandle;
	ModalComponent m_Modal;
	EventListenerObject m_ShowHideEventListener;
	bool m_IsQuitting = false;
};
