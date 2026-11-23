#include "elements/StudioMDLSceneElement.h"

StudioMDLSceneElement::StudioMDLSceneElement(const Rml::String& tag) :
	Rml::Element(tag)
{
}

void StudioMDLSceneElement::SetRenderCallback(std::function<void(void)> callback)
{
	m_RenderCallback = callback;
}

void StudioMDLSceneElement::OnRender()
{
	Rml::Element::OnRender();

	if ( m_RenderCallback )
	{
		m_RenderCallback();
	}
}

void StudioMDLSceneElement::AddRmlModel(SceneModel&& model)
{
	m_Models.push_back(std::move(model));
}

std::vector<StudioMDLSceneElement::SceneModel> StudioMDLSceneElement::TakeRmlModels()
{
	return std::move(m_Models);
}

void StudioMDLSceneElement::AddRmlCamera(SceneCamera&& camera)
{
	m_Cameras.push_back(std::move(camera));
}

std::vector<StudioMDLSceneElement::SceneCamera> StudioMDLSceneElement::TakeRmlCameras()
{
	return std::move(m_Cameras);
}
