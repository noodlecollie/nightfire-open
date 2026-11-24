#pragma once

#include "EnginePublicAPI/cl_entity.h"
#include "EnginePublicAPI/ref_params.h"
#include "framework/BaseComponent.h"
#include "framework/EventListenerObject.h"
#include "elements/StudioMDLSceneElementUtils.h"

class StudioMDLSceneElement;
class StudioMDLSceneItemElement;

class StudioMDLSceneComponent : public BaseComponent
{
public:
	explicit StudioMDLSceneComponent(BaseMenu* parentMenu, Rml::String id);

protected:
	bool ComponentLoadFromDocument(Rml::ElementDocument* document) override;
	void ComponentUnload() override;
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	struct Position
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	struct Angles
	{
		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;
	};

	struct SceneModel
	{
		std::string path;
		Position position {};
		Angles angles {};

		cl_entity_t entity = {};
		std::string cachedPath;

		SceneModel();
	};

	struct SceneCamera
	{
		Position position {};
		Angles angles {};
		float distance = 0.0f;
		float fov = 90.0f;
	};

	struct DataModel
	{
		std::vector<SceneModel> models;
		std::vector<SceneCamera> cameras;
		size_t activeCamera = 0;
		float orbitSensitivity = 1.0f;
		CameraOrbitMode orbitMode = CameraOrbitMode::None;
	};

	void HandleViewEvent(Rml::Event& event);
	void HandleOrbitMouseDrag(const Rml::Vector2i& delta);
	void CreateDataEntries();
	void Render();
	ref_viewpass_t CreateCameraParams(const Rml::Rectanglei& viewport, const SceneCamera& camera) const;
	void CalcYFov(ref_viewpass_t& def) const;
	SceneCamera* GetActiveCamera();

	StudioMDLSceneElement* m_Scene = nullptr;
	DataModel m_Model {};
	EventListenerObject m_ViewEventListener;
	Rml::Vector2i m_LastDragLoc;
};
