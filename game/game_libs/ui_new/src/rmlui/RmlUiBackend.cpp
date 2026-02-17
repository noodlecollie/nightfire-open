#include "rmlui/RmlUiBackend.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "udll_int.h"

RmlUiBackend::RmlUiBackend() :
	m_SystemInterface(this),
	m_RenderInterface(this)
{
}

RmlUiBackend::~RmlUiBackend()
{
	ShutDown();
}

bool RmlUiBackend::Initialise(int width, int height)
{
	if ( m_Initialised )
	{
		return true;
	}

	Rml::SetSystemInterface(&m_SystemInterface);
	Rml::SetRenderInterface(&m_RenderInterface);
	Rml::SetFileInterface(&m_FileInterface);

	Rml::Initialise();
	RegisterFonts();

	m_RmlContext = Rml::CreateContext("main", Rml::Vector2i(width, height));

	if ( !m_RmlContext )
	{
		Rml::Shutdown();
		return false;
	}

	m_RenderInterface.SetViewport(width, height);
	Rml::Debugger::Initialise(m_RmlContext);

	m_Initialised = true;
	return true;
}

void RmlUiBackend::ShutDown()
{
	if ( !m_Initialised )
	{
		return;
	}

	Rml::Shutdown();
	m_RmlContext = nullptr;
	m_Initialised = false;
}

bool RmlUiBackend::IsInitialised() const
{
	return m_Initialised;
}

Rml::Context* RmlUiBackend::GetRmlContext() const
{
	return m_RmlContext;
}

void RmlUiBackend::Update()
{
	if ( !m_Initialised )
	{
		return;
	}

	m_RmlContext->Update();
}

void RmlUiBackend::Render()
{
	if ( !m_Initialised )
	{
		return;
	}

	m_RenderInterface.BeginFrame();
	m_RmlContext->Render();
	m_RenderInterface.EndFrame();
}

void RmlUiBackend::RenderDebugTriangle()
{
	if ( !m_Initialised )
	{
		return;
	}

	m_RenderInterface.RenderDebugTriangle();
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
