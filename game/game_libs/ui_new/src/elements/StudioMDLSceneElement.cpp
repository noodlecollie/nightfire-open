#include "elements/StudioMDLSceneElement.h"
#include "EnginePublicAPI/com_model.h"
#include "EnginePublicAPI/entity_types.h"
#include "PlatformLib/String.h"
#include "elements/StudioMDLSceneComponentElement.h"
#include "udll_int.h"

template<typename T>
static T GetParameter(const Rml::Dictionary& atts, const Rml::String& name, const T& defaultValue)
{
	const auto it = atts.find(name);

	if ( it == atts.end() )
	{
		return defaultValue;
	}

	return it->second.Get<T>(defaultValue);
}

template<typename T>
static T& CreateSceneItem(std::vector<T>& vec, Rml::Element* element)
{
	T item {};
	item.element = reinterpret_cast<uintptr_t>(element);
	vec.push_back(std::move(item));
	return vec.back();
}

template<typename T>
static T* GetSceneElement(std::vector<T>& vec, Rml::Element* element)
{
	uintptr_t ptr = reinterpret_cast<uintptr_t>(element);

	const auto it = std::find_if(
		vec.begin(),
		vec.end(),
		[ptr](const T& item)
		{
			return item.element == ptr;
		}
	);

	return it != vec.end() ? &(*it) : nullptr;
}

template<typename T>
static bool RemoveSceneItem(std::vector<T>& vec, Rml::Element* element)
{
	uintptr_t ptr = reinterpret_cast<uintptr_t>(element);

	const auto it = std::find_if(
		vec.begin(),
		vec.end(),
		[ptr](const T& item)
		{
			return item.element == ptr;
		}
	);

	if ( it != vec.end() )
	{
		vec.erase(it);
		return true;
	}

	return false;
}

StudioMDLSceneElement::SceneModel::SceneModel()
{
	memset(&entity, 0, sizeof(entity));

	entity.index = 0;
	entity.curstate.body = 0;
	entity.curstate.animtime = gpGlobals->time;  // start animation
	entity.curstate.sequence = 1;
	entity.curstate.scale = 1.0f;
	entity.curstate.frame = 0.0f;
	entity.curstate.framerate = 1.0f;
	entity.curstate.effects |= EF_FULLBRIGHT;
	entity.curstate.controller[0] = 127;
	entity.curstate.controller[1] = 127;
	entity.curstate.controller[2] = 127;
	entity.curstate.controller[3] = 127;
	entity.latched.prevcontroller[0] = 127;
	entity.latched.prevcontroller[1] = 127;
	entity.latched.prevcontroller[2] = 127;
	entity.latched.prevcontroller[3] = 127;
	entity.player = false;
}

StudioMDLSceneElement::StudioMDLSceneElement(const Rml::String& tag) :
	Rml::Element(tag),
	m_ChildEventListener(this, &StudioMDLSceneElement::HandleChildEvent)
{
}

void StudioMDLSceneElement::OnRender()
{
	Rml::Element::OnRender();
	DrawScene();
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
	SceneModel& model = CreateSceneItem(m_SceneModels, child);
	HandleSceneModelTransformChanged(child->GetAttributes(), model);
	HandleSceneModelPathChanged(child->GetAttributes(), model);

	child->AddEventListener(StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED, &m_ChildEventListener);
	child->AddEventListener(StudioMDLSceneComponentElement::EVENT_PATH_CHANGED, &m_ChildEventListener);
}

void StudioMDLSceneElement::HandleRemoveModel(StudioMDLSceneComponentElement* child)
{
	child->RemoveEventListener(StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED, &m_ChildEventListener);

	child->RemoveEventListener(StudioMDLSceneComponentElement::EVENT_PATH_CHANGED, &m_ChildEventListener);

	RemoveSceneItem(m_SceneModels, child);
}

void StudioMDLSceneElement::HandleAddCamera(StudioMDLSceneComponentElement* child)
{
	SceneCamera& camera = CreateSceneItem(m_SceneCameras, child);
	HandleSceneCameraTransformChanged(child->GetAttributes(), camera);

	child->AddEventListener(StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED, &m_ChildEventListener);
}

void StudioMDLSceneElement::HandleRemoveCamera(StudioMDLSceneComponentElement* child)
{
	child->RemoveEventListener(StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED, &m_ChildEventListener);

	RemoveSceneItem(m_SceneCameras, child);
}

void StudioMDLSceneElement::HandleChildEvent(Rml::Event& event)
{
	StudioMDLSceneComponentElement* element =
		rmlui_dynamic_cast<StudioMDLSceneComponentElement*>(event.GetTargetElement());

	if ( !element )
	{
		ASSERT(false);
		return;
	}

	if ( event.GetType() == StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED )
	{
		if ( element->GetType() == StudioMDLSceneComponentElement::Type::Model )
		{
			SceneModel* model = GetSceneElement(m_SceneModels, element);
			ASSERT(model);

			if ( model )
			{
				HandleSceneModelTransformChanged(event.GetParameters(), *model);
			}
		}
		else if ( element->GetType() == StudioMDLSceneComponentElement::Type::Camera )
		{
			SceneCamera* camera = GetSceneElement(m_SceneCameras, element);
			ASSERT(camera);

			if ( camera )
			{
				HandleSceneCameraTransformChanged(event.GetParameters(), *camera);
			}
		}
	}
	else if ( event.GetType() == StudioMDLSceneComponentElement::EVENT_PATH_CHANGED )
	{
		SceneModel* model = GetSceneElement(m_SceneModels, element);
		ASSERT(model);

		if ( model )
		{
			HandleSceneModelPathChanged(event.GetParameters(), *model);
		}
	}
}

void StudioMDLSceneElement::HandleSceneModelTransformChanged(const Rml::Dictionary& params, SceneModel& model)
{
	model.entity.origin[0] = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_X, 0.0f);
	model.entity.origin[1] = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_Y, 0.0f);
	model.entity.origin[2] = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_Z, 0.0f);

	model.entity.angles[0] = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_PITCH, 0.0f);
	model.entity.angles[1] = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_YAW, 0.0f);
	model.entity.angles[2] = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_ROLL, 0.0f);
}

void StudioMDLSceneElement::HandleSceneModelPathChanged(const Rml::Dictionary& params, SceneModel& model)
{
	Rml::String modelPath = GetParameter<Rml::String>(params, StudioMDLSceneComponentElement::ATTR_PATH, Rml::String());

	if ( modelPath == model.modelPath )
	{
		return;
	}

	model.modelPath = modelPath;
	model.modelPathDirty = true;

	Rml::Log::Message(
		Rml::Log::Type::LT_DEBUG,
		"StudioMDLSceneElement: Model path updated to %s",
		model.modelPath.c_str()
	);
}

void StudioMDLSceneElement::HandleSceneCameraTransformChanged(const Rml::Dictionary& params, SceneCamera& camera)
{
	camera.orientation.x = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_PITCH, 0.0f);
	camera.orientation.y = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_YAW, 0.0f);
	camera.orientation.z = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_ROLL, 0.0f);
	camera.distance = GetParameter<float>(params, StudioMDLSceneComponentElement::ATTR_DISTANCE, 0.0f);
}

ref_viewpass_t StudioMDLSceneElement::GetCameraParams(const SceneCamera& camera)
{
	ref_viewpass_t params {};

	params.viewport[0] = static_cast<int>(GetAbsoluteLeft());
	params.viewport[1] = static_cast<int>(GetAbsoluteTop());

	const Rml::Vector2f size = GetBox().GetSize();

	params.viewport[2] = static_cast<int>(size.x);
	params.viewport[3] = static_cast<int>(size.y);

	params.fov_x = camera.horizontalFOV;
	params.viewangles[0] = camera.orientation.x;
	params.viewangles[1] = camera.orientation.y;
	params.viewangles[2] = camera.orientation.z;

	CalcYFov(params);

	return params;
}

void StudioMDLSceneElement::DrawScene()
{
	if ( m_SceneModels.empty() || m_SceneCameras.empty() )
	{
		return;
	}

	// For now, just use the first camera.
	ref_viewpass_t camera = GetCameraParams(m_SceneCameras.front());

	gEngfuncs.pfnClearScene();
	gEngfuncs.pfnUpdateScene();

	for ( SceneModel& model : m_SceneModels )
	{
		gEngfuncs.CL_CreateVisibleEntity(ET_NORMAL, &model.entity);

		if ( model.modelPathDirty )
		{
			gEngfuncs.pfnSetModel(&model.entity, model.modelPath.c_str());
			model.modelPathDirty = false;
		}
	}

	gEngfuncs.pfnRenderScene(&camera);
}

void StudioMDLSceneElement::CalcYFov(ref_viewpass_t& def)
{
	if ( def.viewport[2] < 1 || def.viewport[3] < 1 || def.fov_x <= 0.0f )
	{
		return;
	}

	float x = def.viewport[2] / tanf(DEG2RADF(def.fov_x) * 0.5f);
	float half_fov_y = atanf(def.viewport[3] / x);
	def.fov_y = RAD2DEGF(half_fov_y) * 2;
}
