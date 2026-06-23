#pragma once

#include "rmlui/RenderInterfaceImpl.h"
#include "rmlui/SystemInterfaceImpl.h"
#include "rmlui/FileInterfaceImpl.h"
#include "rmlui/TextInputHandlerImpl.h"
#include "rmlui/EventListenerInstancerImpl.h"
#include "framework/MenuDirectory.h"
#include "framework/MenuStack.h"
#include "EnginePublicAPI/netadr.h"

namespace Rml
{
	class Context;
};  // namespace Rml

class RmlUiBackend
{
public:
	using DiscoveredServerCallback = std::function<void(const netadr_t&, Rml::String&&)>;

	struct StoredKey
	{
		int key = -1;
		bool pressed = false;
	};

	static RmlUiBackend& StaticInstance();

	RmlUiBackend();
	~RmlUiBackend();

	void Initialise();
	bool VidInit(int width, int height);
	void ShutDown();
	bool IsInitialised() const;
	bool IsVisible() const;
	bool HasMenuInStack() const;
	void ReceiveShowMenu();
	void ReceiveHideMenu();
	void ReceiveMouseMove(int x, int y);
	void ReceiveMouseButton(int button, bool pressed);
	void ReceiveMouseWheel(bool down);
	void ReceiveKey(int key, bool pressed);
	void ReceiveChar(int character);
	void ReceiveDiscoveredServer(netadr_t address, const char* info);

	void ReceiveConnectionProgress_Connect(const char* server);
	void ReceiveConnectionProgress_ParseServerInfo(const char* server);
	void ReceiveConnectionProgress_Precache();
	void ReceiveConnectionProgress_Download(
		const char* pszFileName,
		const char* pszServerName,
		int iCurrent,
		int iTotal,
		const char* comment
	);
	void ReceiveConnectionProgress_DownloadEnd();
	void ReceiveConnectionProgress_Connected();
	void ReceiveConnectionProgress_Disconnect();
	void ReceiveConnectionProgress_ChangeLevel();

	void Update(float currentTime);
	void Render();

	Rml::Context* GetRmlContext() const;
	MenuStack::FocusChangeResult GetFocusChange();

	void SetStoreNextKey(bool onPressed);
	void ClearStoreNextKey();
	bool IsStoringNextKey() const;
	bool HasStoredKey() const;
	StoredKey TakeStoredKey();

	void SetDiscoveredServerCallback(DiscoveredServerCallback callback);
	void ClearDiscoveredServerCallback();

private:
	struct MainMenuData;

	void ReleaseResources();
	void RegisterFonts();
	void RegisterCvars();
	void RegisterCommands();

	void HandleMenuPushCommand();
	void HandleMenuPopCommand();
	void ReloadCurrentMenu();

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
	MenuStack::FocusChangeResult m_FocusChange = MenuStack::FocusChangeResult::None;

	bool m_StoreNextKey = false;
	StoredKey m_StoredKey {};
	DiscoveredServerCallback m_DiscoveredServerCallback;

	struct cvar_s* m_cvarScrollSensitivity = nullptr;
};
