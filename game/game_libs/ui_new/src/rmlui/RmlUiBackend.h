#pragma once

#include "rmlui/RenderInterfaceImpl.h"
#include "rmlui/SystemInterfaceImpl.h"
#include "rmlui/FileInterfaceImpl.h"
#include "rmlui/TextInputHandlerImpl.h"
#include "rmlui/EventListenerInstancerImpl.h"
#include "framework/MenuDirectory.h"
#include "framework/MenuStack.h"

namespace Rml
{
	class Context;
};  // namespace Rml

class RmlUiBackend
{
public:
	RmlUiBackend();
	~RmlUiBackend();

	void Initialise();
	bool VidInit(int width, int height);
	void ShutDown();
	bool IsInitialised() const;
	bool IsVisible() const;
	void ReceiveStartupComplete();
	void ReceiveShowMenu();
	void ReceiveHideMenu();
	void ReceiveMouseMove(int x, int y);
	void ReceiveMouseButton(int button, bool pressed);
	void ReceiveMouseWheel(bool down);
	void ReceiveKey(int key, bool pressed);
	void ReceiveChar(int character);

	void Update(float currentTime);
	void Render();

	Rml::Context* GetRmlContext() const;

	void RenderDebugTriangle();

private:
	struct MainMenuData;

	void ReleaseResources();
	void RegisterFonts();
	void RegisterCvars();

	SystemInterfaceImpl m_SystemInterface;
	RenderInterfaceImpl m_RenderInterface;
	FileInterfaceImpl m_FileInterface;
	TextInputHandlerImpl m_TextInputHandler;
	EventListenerInstancerImpl m_EventListenerInstancer;

	bool m_Initialised = false;
	Rml::Context* m_RmlContext = nullptr;
	unsigned char m_Modifiers = 0;

	MenuDirectory m_MenuDirectory;
	MenuStack m_MenuStack;
	bool m_Visible = false;

	struct cvar_s* m_cvarScrollSensitivity = nullptr;
};
