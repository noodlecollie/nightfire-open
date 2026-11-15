#pragma once

#include <RmlUi/Core/Element.h>
#include "EnginePublicAPI/cl_entity.h"
#include "EnginePublicAPI/ref_params.h"

class StudioMDLSceneElement : public Rml::Element
{
public:
	static constexpr const char* const ELEMENT_NAME = "studiomdlscene";

	StudioMDLSceneElement(const Rml::String& tag);

protected:
	void OnRender() override;
	void OnResize() override;

private:
	void DrawScene();
	void DefaultInitialiseEntData(cl_entity_t& ent);
	void SetFov(float horizontalDegrees);
	void CalcYFov();

	ref_viewpass_t m_RefDef {};
	std::vector<cl_entity_t> m_SceneModels;
};
