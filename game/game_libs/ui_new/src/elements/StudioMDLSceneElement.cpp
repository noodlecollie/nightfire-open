#include "elements/StudioMDLSceneElement.h"
#include <RmlUi/Core/Log.h>

static constexpr const char* const ATTR_ORBIT_MODE = "orbit";
static constexpr const char* const ATTR_ORBIT_SENSITIVITY = "orbit-sensitivity";

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

void StudioMDLSceneElement::OnAttributeChange(const Rml::ElementAttributes& changedAttributes)
{
	Rml::Element::OnAttributeChange(changedAttributes);

	{
		const auto it = changedAttributes.find(ATTR_ORBIT_MODE);

		if ( it != changedAttributes.end() )
		{
			Rml::String str = it->second.Get<Rml::String>();

			if ( !str.empty() )
			{
				CameraOrbitMode mode = CameraOrbitMode::None;

				if ( ParseCameraOrbitModeString(str, mode) )
				{
					m_OrbitMode = mode;

					SetProperty(
						Rml::PropertyId::Drag,
						Rml::Property(
							m_OrbitMode != CameraOrbitMode::None ? Rml::Style::Drag::Drag : Rml::Style::Drag::None
						)
					);
				}
				else
				{
					Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Unrecognised orbit-mode value \"%s\"", str.c_str());
				}
			}
		}
	}

	{
		const auto it = changedAttributes.find(ATTR_ORBIT_SENSITIVITY);

		if ( it != changedAttributes.end() )
		{
			m_OrbitSensitivity = it->second.Get<float>(1.0f);
		}
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

CameraOrbitMode StudioMDLSceneElement::GetCameraOrbitMode() const
{
	return m_OrbitMode;
}

float StudioMDLSceneElement::GetOrbitSensitivity() const
{
	return m_OrbitSensitivity;
}
