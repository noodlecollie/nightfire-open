#pragma once

#include <RmlUi/Core/SystemInterface.h>

class RmlUiBackend;

class SystemInterfaceImpl : public Rml::SystemInterface
{
public:
	SystemInterfaceImpl(RmlUiBackend* backend);

	double GetElapsedTime() override;
	void SetMouseCursor(const Rml::String& cursor_name) override;
	void SetClipboardText(const Rml::String& text) override;
	void GetClipboardText(Rml::String& text) override;
	bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

private:
	RmlUiBackend* m_Backend;
};
