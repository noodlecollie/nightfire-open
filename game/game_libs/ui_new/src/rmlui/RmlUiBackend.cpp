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

	ReleaseResources();
	Rml::Shutdown();

	m_RmlContext = nullptr;
	m_Initialised = false;
	m_Modifiers = 0;
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

void RmlUiBackend::ReceiveStartupComplete()
{
	ASSERT(IsInitialised());

	if ( !IsInitialised() )
	{
		return;
	}

	const MenuDirectoryEntry* menu = m_MenuDirectory.GetMenuEntry(MainMenu::NAME);
	ASSERT(menu);

	m_MenuStack.Push(menu);
	ReceiveShowMenu();
}

void RmlUiBackend::ReceiveShowMenu()
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_Visible = true;
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

	float scrollDelta = std::max<float>(m_cvarScrollSensitivity->value, 0.1f);
	m_RmlContext->ProcessMouseWheel(Rml::Vector2f(0.0f, scrollDelta * (down ? 1.0f : -1.0f)), m_Modifiers);
}

void RmlUiBackend::ReceiveKey(int key, bool pressed)
{
	if ( !IsInitialised() )
	{
		return;
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

Rml::Context* RmlUiBackend::GetRmlContext() const
{
	return m_RmlContext;
}

void RmlUiBackend::Update(float currentTime)
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_MenuStack.Update(currentTime);
	m_RmlContext->Update();
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

	// Now that rendering is done, modify the menu stack if needed.
	m_MenuStack.HandleRequests();
}

void RmlUiBackend::RenderDebugTriangle()
{
	if ( !IsInitialised() )
	{
		return;
	}

	m_RenderInterface.RenderDebugTriangle();
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
