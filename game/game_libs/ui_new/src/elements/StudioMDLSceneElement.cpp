#include "elements/StudioMDLSceneElement.h"
#include "EnginePublicAPI/entity_types.h"
#include "elements/StudioMDLSceneComponentElement.h"
#include "udll_int.h"

StudioMDLSceneElement::StudioMDLSceneElement(const Rml::String& tag) :
	Rml::Element(tag),
	m_ChildAttributesChangedListener(this, &StudioMDLSceneElement::HandleChildAttributesChanged)
{
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

void StudioMDLSceneElement::OnChildAdd(Rml::Element* child)
{
	if ( !child || child == this || child->GetParentNode() != this )
	{
		Rml::Element::OnChildAdd(child);
		return;
	}

	StudioMDLSceneComponentElement* component = rmlui_dynamic_cast<StudioMDLSceneComponentElement*>(child);

	if ( !component )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"StudioMDLSceneElement: Encountered unexpected child element \"%s\"",
			child->GetTagName().c_str()
		);

		Rml::Element::OnChildAdd(child);
		return;
	}

	const Rml::String tag = child->GetTagName();

	if ( tag == StudioMDLSceneComponentElement::TAG_MODEL )
	{
		HandleAddModel(component);
	}
	else if ( tag == StudioMDLSceneComponentElement::TAG_CAMERA )
	{
		HandleAddCamera(component);
	}
	else
	{
		Rml::Element::OnChildAdd(child);
	}
}

void StudioMDLSceneElement::OnChildRemove(Rml::Element* child)
{
	if ( !child )
	{
		return;
	}

	StudioMDLSceneComponentElement* component = rmlui_dynamic_cast<StudioMDLSceneComponentElement*>(child);

	if ( !component )
	{
		Rml::Element::OnChildAdd(child);
		return;
	}

	const Rml::String tag = child->GetTagName();

	if ( tag == StudioMDLSceneComponentElement::TAG_MODEL )
	{
		HandleRemoveModel(component);
	}
	else if ( tag == StudioMDLSceneComponentElement::TAG_CAMERA )
	{
		HandleRemoveCamera(component);
	}
	else
	{
		Rml::Element::OnChildRemove(child);
	}
}

void StudioMDLSceneElement::HandleAddModel(StudioMDLSceneComponentElement* child)
{
	child->AddEventListener(StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED, &m_ChildAttributesChangedListener);
	child->AddEventListener(StudioMDLSceneComponentElement::EVENT_PATH_CHANGED, &m_ChildAttributesChangedListener);
}

void StudioMDLSceneElement::HandleRemoveModel(StudioMDLSceneComponentElement* child)
{
	child->RemoveEventListener(
		StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED,
		&m_ChildAttributesChangedListener
	);

	child->RemoveEventListener(StudioMDLSceneComponentElement::EVENT_PATH_CHANGED, &m_ChildAttributesChangedListener);
}

void StudioMDLSceneElement::HandleAddCamera(StudioMDLSceneComponentElement* child)
{
	child->AddEventListener(StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED, &m_ChildAttributesChangedListener);
}

void StudioMDLSceneElement::HandleRemoveCamera(StudioMDLSceneComponentElement* child)
{
	child->RemoveEventListener(
		StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED,
		&m_ChildAttributesChangedListener
	);
}

void StudioMDLSceneElement::HandleChildAttributesChanged(Rml::Event& event)
{
	// TODO
	Rml::Log::Message(
		Rml::Log::Type::LT_WARNING,
		"HandleChildAttributesChanged: event %s for element %s",
		event.GetType().c_str(),
		event.GetTargetElement()->GetTagName().c_str()
	);
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
