#pragma once

#include <RmlUi/Core/Element.h>
#include "framework/EventListenerObject.h"
#include "EnginePublicAPI/cl_entity.h"
#include "EnginePublicAPI/ref_params.h"

class StudioMDLSceneComponentElement;

class StudioMDLSceneElement : public Rml::Element
{
public:
	static constexpr const char* const ELEMENT_NAME = "studiomdlscene";

	explicit StudioMDLSceneElement(const Rml::String& tag);

protected:
	void OnRender() override;
	void OnChildAdd(Rml::Element* child) override;
	void OnChildRemove(Rml::Element* child) override;

private:
	struct SceneModel
	{
		uintptr_t element = 0;
		cl_entity_t entity = {};
		Rml::String modelPath;
		bool modelPathDirty = true;

		SceneModel();
	};

	struct SceneCamera
	{
		uintptr_t element = 0;
		Rml::Vector3f position;
		Rml::Vector3f orientation;
		float distance = 0.0f;
		float horizontalFOV = 40.0f;
	};

	static void CalcYFov(ref_viewpass_t& def);

	void HandleAddModel(StudioMDLSceneComponentElement* child);
	void HandleRemoveModel(StudioMDLSceneComponentElement* child);
	void HandleAddCamera(StudioMDLSceneComponentElement* child);
	void HandleRemoveCamera(StudioMDLSceneComponentElement* child);
	void HandleChildEvent(Rml::Event& event);
	void HandleSceneModelTransformChanged(const Rml::Dictionary& params, SceneModel& model);
	void HandleSceneModelPathChanged(const Rml::Dictionary& params, SceneModel& model);
	void HandleSceneCameraTransformChanged(const Rml::Dictionary& params, SceneCamera& camera);
	ref_viewpass_t GetCameraParams(const SceneCamera& camera);
	void DrawScene();

	std::vector<SceneModel> m_SceneModels;
	std::vector<SceneCamera> m_SceneCameras;
	EventListenerObject m_ChildEventListener;
};
