#pragma once

#include "framework/BaseComponent.h"
#include "framework/EventListenerObject.h"
#include <functional>

class ModalComponent : public BaseComponent
{
public:
	using ButtonClickCallback = std::function<void(Rml::Event& /*event*/, size_t /*buttonIndex*/)>;

	explicit ModalComponent(BaseMenu* parentMenu, Rml::String id);

	void SetButtonClickCallback(ButtonClickCallback callback);

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
		Rml::ElementList buttons;
	};

	void LoadParams();
	void LoadButtons(const Rml::StringList& buttons);
	void HandleButtonEvent(Rml::Event& event);

	Elements m_Elems {};
	ButtonClickCallback m_ButtonClickCallback;
	EventListenerObject m_ButtonEventListener;
};
