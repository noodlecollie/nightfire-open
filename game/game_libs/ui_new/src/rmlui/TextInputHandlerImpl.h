#pragma once

// It'd be really nice if the text input handler header
// actually included what it uses...
#include <RmlUi/Core/Header.h>
#include <RmlUi/Core/Traits.h>
#include <RmlUi/Core/TextInputHandler.h>

class TextInputHandlerImpl : public Rml::TextInputHandler
{
public:
	void OnActivate(Rml::TextInputContext* context) override;
	void OnDeactivate(Rml::TextInputContext* context) override;

	bool IsActive() const;

private:
	bool m_IsActive = false;
};
