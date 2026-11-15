#include "elements/StudioMDLSceneElement.h"
#include "EnginePublicAPI/entity_types.h"
#include "udll_int.h"

static constexpr const char* const TAG_MODEL = "model";
static constexpr const char* const TAG_CAMERA = "camera";
static constexpr const char* const ATTR_PATH = "path";
static constexpr const char* const ATTR_PITCH = "pitch";
static constexpr const char* const ATTR_YAW = "yaw";
static constexpr const char* const ATTR_ROLL = "roll";
static constexpr const char* const ATTR_X = "x";
static constexpr const char* const ATTR_Y = "y";
static constexpr const char* const ATTR_Z = "z";
static constexpr const char* const ATTR_DISTANCE = "distance";

StudioMDLSceneElement::StudioMDLSceneElement(const Rml::String& tag) :
	Rml::Element(tag)
{
	SetFov(40.0f);
}

void StudioMDLSceneElement::OnRender()
{
	Rml::Element::OnRender();
	DrawScene();
}

void StudioMDLSceneElement::OnResize()
{
	Rml::Element::OnResize();

	m_RefDef.viewport[0] = static_cast<int>(GetAbsoluteLeft());
	m_RefDef.viewport[1] = static_cast<int>(GetAbsoluteTop());

	const Rml::Vector2f size = GetBox().GetSize();

	m_RefDef.viewport[2] = static_cast<int>(size.x);
	m_RefDef.viewport[3] = static_cast<int>(size.y);

	CalcYFov();
}

void StudioMDLSceneElement::DrawScene()
{
	if ( m_SceneModels.empty() )
	{
		return;
	}

	gEngfuncs.pfnClearScene();
	gEngfuncs.pfnUpdateScene();

	for ( cl_entity_t& model : m_SceneModels )
	{
		gEngfuncs.CL_CreateVisibleEntity(ET_NORMAL, &model);
	}

	gEngfuncs.pfnRenderScene(&m_RefDef);
}

void StudioMDLSceneElement::DefaultInitialiseEntData(cl_entity_t& ent)
{
	memset(&ent, 0, sizeof(ent));

	ent.index = 0;
	ent.curstate.body = 0;
	ent.curstate.animtime = gpGlobals->time;  // start animation
	ent.curstate.sequence = 1;
	ent.curstate.scale = 1.0f;
	ent.curstate.frame = 0.0f;
	ent.curstate.framerate = 1.0f;
	ent.curstate.effects |= EF_FULLBRIGHT;
	ent.curstate.controller[0] = 127;
	ent.curstate.controller[1] = 127;
	ent.curstate.controller[2] = 127;
	ent.curstate.controller[3] = 127;
	ent.latched.prevcontroller[0] = 127;
	ent.latched.prevcontroller[1] = 127;
	ent.latched.prevcontroller[2] = 127;
	ent.latched.prevcontroller[3] = 127;
	ent.player = false;
}

void StudioMDLSceneElement::SetFov(float horizontalDegrees)
{
	if ( horizontalDegrees < 1.0f )
	{
		horizontalDegrees = 1.0f;
	}
	else if ( horizontalDegrees > 179.0f )
	{
		horizontalDegrees = 179.0f;
	}

	m_RefDef.fov_x = horizontalDegrees;
	CalcYFov();
}

void StudioMDLSceneElement::CalcYFov()
{
	if ( m_RefDef.viewport[2] < 1 || m_RefDef.viewport[3] < 1 || m_RefDef.fov_x <= 0.0f )
	{
		return;
	}

	float x = m_RefDef.viewport[2] / tanf(DEG2RADF(m_RefDef.fov_x) * 0.5f);
	float half_fov_y = atanf(m_RefDef.viewport[3] / x);
	m_RefDef.fov_y = RAD2DEGF(half_fov_y) * 2;
}
