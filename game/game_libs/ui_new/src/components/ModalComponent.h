#pragma once

#include "framework/BaseComponent.h"

class ModalComponent : public BaseComponent
{
public:
	explicit ModalComponent(BaseMenu* parentMenu, Rml::String id);

protected:
	bool OnLoadFromDocument(Rml::ElementDocument* document) override;
	void OnUnload() override;

private:
	struct Elements
	{
		Rml::Element* modal = nullptr;
		Rml::Element* modalHeader = nullptr;
		Rml::Element* modalBody = nullptr;
		Rml::Element* modalFooter = nullptr;
	};

	Elements m_Elems {};
};
