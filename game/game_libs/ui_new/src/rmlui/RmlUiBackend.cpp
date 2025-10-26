#include "rmlui/RmlUiBackend.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

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

	Rml::Initialise();

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
