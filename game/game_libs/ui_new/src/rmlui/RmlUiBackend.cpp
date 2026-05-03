#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "EnginePublicAPI/keydefs.h"
#include "EnginePublicAPI/cvardef.h"
#include "rmlui/RmlUiBackend.h"
#include "rmlui/Utils.h"
#include "framework/BaseMenu.h"
#include "menus/MainMenu.h"
#include "udll_int.h"
#include "UIDebug.h"

static constexpr const char* const CONTEXT_NAME = "main";

RmlUiBackend& RmlUiBackend::StaticInstance()
{
	static RmlUiBackend instance;
	return instance;
}

RmlUiBackend::RmlUiBackend() :
	m_SystemInterface(this),
	m_RenderInterface(this),
	m_MenuDirectory(),
	m_MenuStack(&m_MenuDirectory)
{
}

RmlUiBackend::~RmlUiBackend()
{
	ShutDown();
}

void RmlUiBackend::Initialise()
{
	if ( m_Initialised )
	{
		return;
	}

	Rml::SetSystemInterface(&m_SystemInterface);
	Rml::SetRenderInterface(&m_RenderInterface);
	Rml::SetFileInterface(&m_FileInterface);
	Rml::SetTextInputHandler(&m_TextInputHandler);
	Rml::Factory::RegisterEventListenerInstancer(&m_EventListenerInstancer);

	Rml::Initialise();
	RegisterFonts();
	RegisterCvars();
	RegisterCommands();
	m_MenuDirectory.Populate();

	m_Modifiers = 0;

	m_RmlContext = Rml::CreateContext(CONTEXT_NAME, Rml::Vector2i(16, 16));
	ASSERT(m_RmlContext);

	if ( !m_RmlContext )
	{
		ReleaseResources();
		Rml::Shutdown();
		return;
	}

	Rml::Debugger::Initialise(m_RmlContext);
	m_MenuDirectory.LoadAllMenus(*m_RmlContext);

	m_Initialised = true;
}

bool RmlUiBackend::VidInit(int width, int height)
{
	if ( !m_Initialised )
	{
		return false;
	}

	m_RenderInterface.SetViewport(width, height);
	m_RmlContext->SetDimensions(Rml::Vector2i(width, height));

	float dpiScale = 1.0f;

	if ( height >= 2160 )
	{
		dpiScale = 2.0f;
	}
	else if ( height >= 1080 )
	{
		dpiScale = 1.5f;
	}

	m_RmlContext->SetDensityIndependentPixelRatio(dpiScale);

	return true;
}

void RmlUiBackend::ShutDown()
{
	if ( !m_Initialised )
	{
		return;
	}

	ClearDiscoveredServerCallback();
	ReleaseResources();
	Rml::Shutdown();

	m_RmlContext = nullptr;
	m_Initialised = false;
	m_Modifiers = 0;
	m_StoreNextKey = false;
	m_StoredKey = StoredKey {};
}

bool RmlUiBackend::IsInitialised() const
{
	return m_Initialised && m_RmlContext != nullptr;
}

bool RmlUiBackend::IsVisible() const
{
	if ( !IsInitialised() )
	{
		return false;
	}

	return m_Visible;
}

void RmlUiBackend::ReceiveShowMenu()
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_Visible = true;

	if ( m_MenuStack.IsEmpty() )
	{
		const MenuDirectoryEntry* menu = m_MenuDirectory.GetMenuEntry(MainMenu::NAME);
		ASSERT(menu);
		m_MenuStack.Push(menu);
	}
}

void RmlUiBackend::ReceiveHideMenu()
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_Visible = false;
}

void RmlUiBackend::ReceiveMouseMove(int x, int y)
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_RmlContext->ProcessMouseMove(x, y, m_Modifiers);
}

void RmlUiBackend::ReceiveMouseButton(int button, bool pressed)
{
	if ( !IsInitialised() )
	{
		return;
	}

	if ( m_StoreNextKey && m_StoredKey.pressed == pressed )
	{
		m_StoredKey.key = button;
		m_StoreNextKey = false;
	}

	switch ( button )
	{
		case K_MOUSE1:
		case K_MOUSE2:
		case K_MOUSE3:
		{
			if ( pressed )
			{
				m_RmlContext->ProcessMouseButtonDown(button - K_MOUSE1, m_Modifiers);
			}
			else
			{
				m_RmlContext->ProcessMouseButtonUp(button - K_MOUSE1, m_Modifiers);
			}
		}
	}
}

void RmlUiBackend::ReceiveMouseWheel(bool down)
{
	if ( !IsInitialised() )
	{
		return;
	}

	if ( m_StoreNextKey && m_StoredKey.pressed )
	{
		m_StoredKey.key = down ? K_MWHEELDOWN : K_MWHEELUP;
		m_StoreNextKey = false;
	}

	float scrollDelta = std::max<float>(m_cvarScrollSensitivity->value, 0.1f);
	m_RmlContext->ProcessMouseWheel(Rml::Vector2f(0.0f, scrollDelta * (down ? 1.0f : -1.0f)), m_Modifiers);
}

void RmlUiBackend::ReceiveKey(int key, bool pressed)
{
	if ( !IsInitialised() )
	{
		return;
	}

	if ( m_StoreNextKey && m_StoredKey.pressed == pressed )
	{
		m_StoredKey.key = key;
		m_StoreNextKey = false;
	}

	Rml::Input::KeyIdentifier rmlKey = EngineKeyToRmlKey(key);

	// TODO: A better solution for this?
#ifdef _DEBUG
	if ( rmlKey == Rml::Input::KeyIdentifier::KI_F1 && pressed &&
		 (m_Modifiers & (Rml::Input::KeyModifier::KM_CTRL | Rml::Input::KeyModifier::KM_SHIFT)) )
	{
		Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
	}
#endif

	if ( rmlKey == Rml::Input::KeyIdentifier::KI_UNKNOWN )
	{
		// Not handled as a normal key, so set modifiers.
		// If not a recognised modifier, flags will remain unchanged.
		if ( pressed )
		{
			m_Modifiers |= EngineKeyToRmlKeyModifier(key);
		}
		else
		{
			m_Modifiers &= ~EngineKeyToRmlKeyModifier(key);
		}

		return;
	}

	// In developer mode, open the console on Ctrl + Escape
	if ( gpGlobals->developer != 0 && (m_Modifiers == Rml::Input::KeyModifier::KM_CTRL) &&
		 rmlKey == Rml::Input::KI_ESCAPE && pressed )
	{
		m_ShouldPopToConsole = true;
		return;
	}

	if ( pressed )
	{
		m_RmlContext->ProcessKeyDown(rmlKey, m_Modifiers);
	}
	else
	{
		m_RmlContext->ProcessKeyUp(rmlKey, m_Modifiers);
	}
}

void RmlUiBackend::ReceiveChar(int character)
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_RmlContext->ProcessTextInput(static_cast<char>(character));
}

void RmlUiBackend::ReceiveDiscoveredServer(netadr_t address, const char* info)
{
	if ( !IsInitialised() )
	{
		return;
	}

	if ( m_DiscoveredServerCallback )
	{
		m_DiscoveredServerCallback(address, Rml::String(info));
	}
}

Rml::Context* RmlUiBackend::GetRmlContext() const
{
	return m_RmlContext;
}

bool RmlUiBackend::ShouldPopToConsole()
{
	bool out = m_ShouldPopToConsole;
	m_ShouldPopToConsole = false;
	return out;
}

void RmlUiBackend::SetStoreNextKey(bool onPressed)
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_StoreNextKey = true;

	// Store whether we want to track the press or release.
	m_StoredKey = StoredKey {-1, onPressed};
}

void RmlUiBackend::ClearStoreNextKey()
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_StoreNextKey = false;
	m_StoredKey = StoredKey {};
}

bool RmlUiBackend::IsStoringNextKey() const
{
	return IsInitialised() && m_StoreNextKey;
}

bool RmlUiBackend::HasStoredKey() const
{
	return IsInitialised() && m_StoredKey.key != -1;
}

RmlUiBackend::StoredKey RmlUiBackend::TakeStoredKey()
{
	if ( !IsInitialised() )
	{
		return StoredKey {};
	}

	StoredKey storedKey = m_StoredKey;
	m_StoredKey = StoredKey {};
	return storedKey;
}

void RmlUiBackend::SetDiscoveredServerCallback(DiscoveredServerCallback callback)
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_DiscoveredServerCallback = std::move(callback);
}

void RmlUiBackend::ClearDiscoveredServerCallback()
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_DiscoveredServerCallback = {};
}

void RmlUiBackend::Update(float currentTime)
{
	if ( !IsInitialised() )
	{
		return;
	}

	bool hadMenusInStack = !m_MenuStack.IsEmpty();

	m_MenuStack.Update(currentTime);
	m_RmlContext->Update();
	m_MenuStack.HandleRequests();

	if ( hadMenusInStack && m_MenuStack.IsEmpty() )
	{
		m_ShouldPopToConsole = true;
	}
}

void RmlUiBackend::Render()
{
	if ( !IsInitialised() || !m_Visible )
	{
		return;
	}

	m_RenderInterface.BeginFrame();
	m_RmlContext->Render();
	m_RenderInterface.EndFrame();
}

void RmlUiBackend::ReleaseResources()
{
	Rml::Debugger::Shutdown();

	if ( m_RmlContext )
	{
		m_RmlContext->UnloadAllDocuments();
		Rml::RemoveContext(CONTEXT_NAME);
		m_RmlContext = nullptr;
	}

	m_MenuDirectory.Clear();
	Rml::ReleaseFontResources();
}

void RmlUiBackend::RegisterFonts()
{
	ui_gl_filesystem_listing* listing = gUiGlFuncs.filesystem.findFiles("resource/fonts/*.ttf");

	for ( const char* path = gUiGlFuncs.filesystem.listingGetCurrentItem(listing); path;
		  gUiGlFuncs.filesystem.listingNextItem(listing), path = gUiGlFuncs.filesystem.listingGetCurrentItem(listing) )
	{
		Rml::LoadFontFace(path);
	}

	gUiGlFuncs.filesystem.freeListing(listing);
}

void RmlUiBackend::RegisterCvars()
{
	m_cvarScrollSensitivity = gEngfuncs.pfnRegisterVariable("ui_scroll_sensitivity", "1", FCVAR_ARCHIVE);

	m_SystemInterface.RegisterCvars();
}

void RmlUiBackend::RegisterCommands()
{
	gEngfuncs.pfnAddCommand(
		"menu_push",
		[]()
		{
			StaticInstance().HandleMenuPushCommand();
		}
	);

	gEngfuncs.pfnAddCommand(
		"menu_pop",
		[]()
		{
			StaticInstance().HandleMenuPopCommand();
		}
	);
}

void RmlUiBackend::HandleMenuPushCommand()
{
	int argc = gEngfuncs.pfnCmdArgc();

	if ( argc < 2 )
	{
		gEngfuncs.Con_Printf("Usage: menu_push <menu> [menu] ...\n");
		return;
	}

	for ( int index = 1; index < argc; ++index )
	{
		const char* menuName = gEngfuncs.pfnCmdArgv(index);

		// Should never happen:
		if ( !menuName || !(*menuName) )
		{
			ASSERT(false);
			continue;
		}

		const MenuDirectoryEntry* menu = m_MenuDirectory.GetMenuEntry(menuName);

		if ( !menu )
		{
			gEngfuncs.Con_Printf("Could not find menu with name \"%s\"\n", menuName);
			return;
		}

		m_MenuStack.Push(menu);
	}
}

void RmlUiBackend::HandleMenuPopCommand()
{
	int argc = gEngfuncs.pfnCmdArgc();

	if ( argc > 2 )
	{
		gEngfuncs.Con_Printf("Usage: menu_pop [new_menu] ...\n");
		gEngfuncs.Con_Printf("If a new menu is provided, it will replace the current one.\n");
		return;
	}

	m_MenuStack.Pop();

	if ( argc == 2 )
	{
		const char* menuName = gEngfuncs.pfnCmdArgv(1);

		// Should never happen:
		if ( !menuName || !(*menuName) )
		{
			ASSERT(false);
			return;
		}

		const MenuDirectoryEntry* menu = m_MenuDirectory.GetMenuEntry(menuName);

		if ( !menu )
		{
			gEngfuncs.Con_Printf("Could not find menu with name \"%s\"\n", menuName);
			return;
		}

		m_MenuStack.Push(menu);
	}
}
