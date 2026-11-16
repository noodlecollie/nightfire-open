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
	void OnResize() override;
	void OnChildAdd(Rml::Element* child) override;
	void OnChildRemove(Rml::Element* child) override;

private:
	void HandleAddModel(StudioMDLSceneComponentElement* child);
	void HandleRemoveModel(StudioMDLSceneComponentElement* child);
	void HandleAddCamera(StudioMDLSceneComponentElement* child);
	void HandleRemoveCamera(StudioMDLSceneComponentElement* child);
	void HandleChildAttributesChanged(Rml::Event& event);
	void DrawScene();
	void DefaultInitialiseEntData(cl_entity_t& ent);
	void SetFov(float horizontalDegrees);
	void CalcYFov();

	ref_viewpass_t m_RefDef {};
	std::vector<cl_entity_t> m_SceneModels;
	EventListenerObject m_ChildAttributesChangedListener;
};
