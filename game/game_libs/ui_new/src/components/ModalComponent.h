#pragma once

#include "framework/BaseComponent.h"
#include "framework/EventListenerObject.h"
#include <functional>

class ModalComponent : public BaseComponent
{
public:
	using ButtonClickCallback =
		std::function<void(Rml::Event& /*event*/, size_t /*buttonIndex*/, const Rml::Variant& /*userData*/)>;

	explicit ModalComponent(BaseMenu* parentMenu, Rml::String id);

	void SetTitle(const Rml::String& title);
	void SetContentsRml(const Rml::String& rml);
	void SetButtons(const Rml::StringList& buttons);
	void SetButtonClickCallback(ButtonClickCallback callback);

	const Rml::Variant& UserData() const;
	void SetUserData(Rml::Variant data);

protected:
	bool OnLoadFromDocument(Rml::ElementDocument* document) override;
	void OnUnload() override;

private:
	struct Elements
	{
		Rml::Element* shade = nullptr;
		Rml::Element* modal = nullptr;
		Rml::Element* modalHeader = nullptr;
		Rml::Element* modalBody = nullptr;
		Rml::Element* modalFooter = nullptr;
		Rml::ElementList buttons;
	};

	void LoadParams();
	void HandleButtonEvent(Rml::Event& event);

	Elements m_Elems {};
	ButtonClickCallback m_ButtonClickCallback;
	EventListenerObject m_ButtonEventListener;
	Rml::Variant m_UserData;
};
