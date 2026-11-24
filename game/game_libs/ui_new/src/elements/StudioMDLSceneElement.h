#pragma once

#include <RmlUi/Core/Element.h>
#include "elements/StudioMDLSceneElementUtils.h"

class StudioMDLSceneItemElement;

class StudioMDLSceneElement : public Rml::Element
{
public:
	static constexpr const char* const TAG = "studiomdlscene";

	struct SceneModel
	{
		std::string path;
		Rml::Vector3f position {};
		Rml::Vector3f angles {};
	};

	struct SceneCamera
	{
		Rml::Vector3f position {};
		Rml::Vector3f angles {};
		float distance = 0.0f;
		float fov = 90.0f;
	};

	explicit StudioMDLSceneElement(const Rml::String& tag);

	void SetRenderCallback(std::function<void(void)> callback);

	void AddRmlModel(SceneModel&& model);
	std::vector<SceneModel> TakeRmlModels();

	void AddRmlCamera(SceneCamera&& camera);
	std::vector<SceneCamera> TakeRmlCameras();

	CameraOrbitMode GetCameraOrbitMode() const;
	float GetOrbitSensitivity() const;

protected:
	void OnRender() override;
	void OnAttributeChange(const Rml::ElementAttributes& changedAttributes) override;

private:
	std::function<void(void)> m_RenderCallback;
	std::vector<SceneModel> m_Models;
	std::vector<SceneCamera> m_Cameras;
	CameraOrbitMode m_OrbitMode = CameraOrbitMode::None;
	float m_OrbitSensitivity = 1.0f;
};
