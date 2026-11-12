#pragma once

#include <RmlUi/Core/Element.h>
#include "EnginePublicAPI/ref_params.h"
#include "models/StudioMDLSceneModel.h"

class StudioMDLSceneElement : public Rml::Element
{
public:
	static constexpr const char* const ELEMENT_NAME = "studiomdlscene";

	StudioMDLSceneElement(const Rml::String& tag);

protected:
	void OnUpdate() override;
	void OnRender() override;
	void OnResize() override;

private:
	void CalcFov();

	ref_viewpass_t m_RefDef {};
	float m_DistFromOrigin = 0.0f;
	Rml::Vector3f m_CameraCentre;
	StudioMDLSceneModel m_Model;
};
