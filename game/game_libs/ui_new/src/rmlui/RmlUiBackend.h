#pragma once

#include "rmlui/RenderInterfaceImpl.h"
#include "rmlui/SystemInterfaceImpl.h"

namespace Rml
{
	class Context;
};

class RmlUiBackend
{
public:
	RmlUiBackend();
	~RmlUiBackend();

	bool Initialise(int width, int height);
	void ShutDown();
	bool IsInitialised() const;

	void Update();
	void Render();

	Rml::Context* GetRmlContext() const;

	void RenderDebugTriangle();

private:
	SystemInterfaceImpl m_SystemInterface;
	RenderInterfaceImpl m_RenderInterface;

	bool m_Initialised = false;
	Rml::Context* m_RmlContext = nullptr;
};
