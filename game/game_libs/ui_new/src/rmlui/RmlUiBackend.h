#pragma once

#include "rmlui/RenderInterfaceImpl.h"
#include "rmlui/SystemInterfaceImpl.h"
#include "rmlui/FileInterfaceImpl.h"
#include "rmlui/TextInputHandlerImpl.h"

namespace Rml
{
	class Context;
};  // namespace Rml

class RmlUiBackend
{
public:
	RmlUiBackend();
	~RmlUiBackend();

	bool Initialise(int width, int height);
	void ShutDown();
	bool IsInitialised() const;
	void ReceiveMouseMove(int x, int y);
	void ReceiveMouseButton(int button, bool pressed);
	void ReceiveMouseWheel(bool down);
	void ReceiveKey(int key, bool pressed);
	void ReceiveChar(int character);

	void Update();
	void Render();

	Rml::Context* GetRmlContext() const;

	void RenderDebugTriangle();

private:
	void RegisterFonts();

	SystemInterfaceImpl m_SystemInterface;
	RenderInterfaceImpl m_RenderInterface;
	FileInterfaceImpl m_FileInterface;
	TextInputHandlerImpl m_TextInputHandler;

	bool m_Initialised = false;
	Rml::Context* m_RmlContext = nullptr;
	unsigned char m_Modifiers = 0;
};
