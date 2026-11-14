#include "elements/StudioMDLSceneElement.h"
#include "EnginePublicAPI/entity_types.h"
#include "MathLib/utils.h"
#include "udll_int.h"

StudioMDLSceneElement::StudioMDLSceneElement(const Rml::String& tag) :
	Rml::Element(tag)
{
	memset(&m_RefDef, 0, sizeof(m_RefDef));
	m_RefDef.fov_x = 40.0f;
}

void StudioMDLSceneElement::OnUpdate()
{
	Rml::Element::OnUpdate();
}

void StudioMDLSceneElement::OnResize()
{
	const Rml::Vector2f offset = GetAbsoluteOffset();
	m_RefDef.viewport[0] = static_cast<int>(offset.x);
	m_RefDef.viewport[1] = static_cast<int>(offset.y);

	const Rml::Vector2f area = GetBox().GetSize(Rml::BoxArea::Content);
	m_RefDef.viewport[2] = static_cast<int>(area.x);
	m_RefDef.viewport[3] = static_cast<int>(area.y);

	CalcFov();
}

void StudioMDLSceneElement::OnRender()
{
	Rml::Element::OnRender();

	gEngfuncs.pfnClearScene();
	gEngfuncs.pfnUpdateScene();

	vec3_t camCentre = {m_CameraCentre.x, m_CameraCentre.y, m_CameraCentre.z};

	vec3_t fwd;
	AngleVectors(m_RefDef.viewangles, fwd, nullptr, nullptr);
	VectorScale(fwd, -1.0f * m_DistFromOrigin, fwd);
	VectorAdd(camCentre, fwd, m_RefDef.vieworigin);

	size_t numEnts = m_Model.GetNumEnts();

	for ( size_t index = 0; index < numEnts; ++index )
	{
		gEngfuncs.CL_CreateVisibleEntity(ET_NORMAL, m_Model.GetEntData(index));
	}

	gEngfuncs.pfnRenderScene(&m_RefDef);
}

void StudioMDLSceneElement::CalcFov()
{
	float x = m_RefDef.viewport[2] / tanf(DEG2RADF(m_RefDef.fov_x) * 0.5f);
	float half_fov_y = atanf(m_RefDef.viewport[3] / x);
	m_RefDef.fov_y = RAD2DEGF(half_fov_y) * 2;
}
