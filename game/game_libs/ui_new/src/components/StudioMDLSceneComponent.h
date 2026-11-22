#pragma once

#include "framework/BaseComponent.h"

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
	struct Elements
	{
		StudioMDLSceneElement* scene = nullptr;
		std::vector<StudioMDLSceneItemElement*> models;
		std::vector<StudioMDLSceneItemElement*> cameras;
	};

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
	};

	void CreateDataEntries();

	Elements m_Elems {};
	DataModel m_Model {};
};
