#include "components/StudioMDLSceneComponent.h"
#include "EnginePublicAPI/entity_types.h"
#include "MathLib/utils.h"
#include "framework/ElementFinder.h"
#include "elements/StudioMDLSceneElement.h"
#include "elements/StudioMDLSceneItemElement.h"
#include "udll_int.h"
#include "UIDebug.h"

static constexpr const char* const NAME_MODELS = "models";
static constexpr const char* const NAME_CAMERAS = "cameras";
static constexpr const char* const SUBNAME_X = "x";
static constexpr const char* const SUBNAME_Y = "y";
static constexpr const char* const SUBNAME_Z = "z";
static constexpr const char* const SUBNAME_PITCH = "pitch";
static constexpr const char* const SUBNAME_YAW = "yaw";
static constexpr const char* const SUBNAME_ROLL = "roll";
static constexpr const char* const SUBNAME_POSITION = "position";
static constexpr const char* const SUBNAME_ANGLES = "angles";
static constexpr const char* const SUBNAME_PATH = "path";
static constexpr const char* const SUBNAME_DISTANCE = "distance";
static constexpr const char* const SUBNAME_FOV = "fov";

StudioMDLSceneComponent::SceneModel::SceneModel()
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

StudioMDLSceneComponent::StudioMDLSceneComponent(BaseMenu* parentMenu, Rml::String id) :
	BaseComponent(parentMenu, std::move(id))
{
}

bool StudioMDLSceneComponent::ComponentLoadFromDocument(Rml::ElementDocument*)
{
	ElementFinder finder;

	finder.Add(ComponentElementPtrPtr(), "studiomdlscene", &m_Scene);

	if ( !finder.FindAll() )
	{
		return false;
	}

	m_Scene->SetRenderCallback(
		[this]()
		{
			Render();
		}
	);

	CreateDataEntries();

	return true;
}

void StudioMDLSceneComponent::ComponentUnload()
{
	m_Scene = nullptr;
	m_Model = {};
}

bool StudioMDLSceneComponent::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<Position> positionDef = constructor.RegisterStruct<Position>();
	Rml::StructHandle<Angles> anglesDef = constructor.RegisterStruct<Angles>();
	Rml::StructHandle<SceneModel> modelDef = constructor.RegisterStruct<SceneModel>();
	Rml::StructHandle<SceneCamera> cameraDef = constructor.RegisterStruct<SceneCamera>();

	if ( !positionDef || !anglesDef || !modelDef || !cameraDef )
	{
		return false;
	}

	if ( !positionDef.RegisterMember(SUBNAME_X, &Position::x) || !positionDef.RegisterMember(SUBNAME_Y, &Position::y) ||
		 !positionDef.RegisterMember(SUBNAME_Z, &Position::z) )
	{
		return false;
	}

	if ( !anglesDef.RegisterMember(SUBNAME_PITCH, &Angles::pitch) ||
		 !anglesDef.RegisterMember(SUBNAME_YAW, &Angles::yaw) ||
		 !anglesDef.RegisterMember(SUBNAME_ROLL, &Angles::roll) )
	{
		return false;
	}

	if ( !modelDef.RegisterMember(SUBNAME_POSITION, &SceneModel::position) ||
		 !modelDef.RegisterMember(SUBNAME_ANGLES, &SceneModel::angles) ||
		 !modelDef.RegisterMember(SUBNAME_PATH, &SceneModel::path) )
	{
		return false;
	}

	if ( !cameraDef.RegisterMember(SUBNAME_POSITION, &SceneCamera::position) ||
		 !cameraDef.RegisterMember(SUBNAME_ANGLES, &SceneCamera::angles) ||
		 !cameraDef.RegisterMember(SUBNAME_DISTANCE, &SceneCamera::distance) ||
		 !cameraDef.RegisterMember(SUBNAME_FOV, &SceneCamera::fov) )
	{
		return false;
	}

	if ( !constructor.RegisterArray<std::vector<SceneModel>>() ||
		 !constructor.RegisterArray<std::vector<SceneCamera>>() )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_MODELS, &m_Model.models) || !constructor.Bind(NAME_CAMERAS, &m_Model.cameras) )
	{
		return false;
	}

	return true;
}

void StudioMDLSceneComponent::CreateDataEntries()
{
	m_Model = {};

	ASSERT(m_Scene);
	std::vector<StudioMDLSceneElement::SceneModel> inModels = m_Scene->TakeRmlModels();
	std::vector<StudioMDLSceneElement::SceneCamera> inCameras = m_Scene->TakeRmlCameras();

	Rml::Log::Message(
		Rml::Log::Type::LT_DEBUG,
		"StudioMDLSceneComponent: Creating data entries for %zu models and %zu cameras",
		inModels.size(),
		inCameras.size()
	);

	m_Model.models.reserve(inModels.size());
	m_Model.cameras.reserve(inCameras.size());

	for ( StudioMDLSceneElement::SceneModel& inModel : inModels )
	{
		SceneModel model {};

		model.position.x = inModel.position.x;
		model.position.y = inModel.position.y;
		model.position.z = inModel.position.z;
		model.angles.pitch = inModel.angles.x;
		model.angles.yaw = inModel.angles.y;
		model.angles.roll = inModel.angles.z;
		model.path = inModel.path;

		m_Model.models.push_back(std::move(model));
	}

	for ( StudioMDLSceneElement::SceneCamera& inCamera : inCameras )
	{
		SceneCamera camera {};

		camera.position.x = inCamera.position.x;
		camera.position.y = inCamera.position.y;
		camera.position.z = inCamera.position.z;
		camera.angles.pitch = inCamera.angles.x;
		camera.angles.yaw = inCamera.angles.y;
		camera.angles.roll = inCamera.angles.z;
		camera.distance = inCamera.distance;
		camera.fov = inCamera.fov;

		m_Model.cameras.push_back(std::move(camera));
	}

	DirtyVariable(NAME_MODELS);
	DirtyVariable(NAME_CAMERAS);
}
void StudioMDLSceneComponent::Render()
{
	if ( !m_Scene || m_Model.models.empty() || m_Model.cameras.empty() )
	{
		return;
	}

	const Rml::Vector2i topLeft(
		static_cast<int>(m_Scene->GetAbsoluteLeft()),
		static_cast<int>(m_Scene->GetAbsoluteTop())
	);

	const Rml::Vector2f size = m_Scene->GetBox().GetSize();

	const Rml::Rectanglei viewport =
		Rml::Rectanglei::FromPositionSize(topLeft, Rml::Vector2i(static_cast<int>(size.x), static_cast<int>(size.y)));

	// TODO: Swap out array of cameras for just one
	ref_viewpass_t camera = CreateCameraParams(viewport, m_Model.cameras.front());

	gUiGlFuncs.renderer.setRendererMode(UI_GL_RENDERER_MODE_GAME);
	gEngfuncs.pfnClearScene();
	gEngfuncs.pfnUpdateScene();

	for ( SceneModel& model : m_Model.models )
	{
		gEngfuncs.CL_CreateVisibleEntity(ET_NORMAL, &model.entity);

		if ( model.cachedPath != model.path )
		{
			model.cachedPath = model.path;
			gEngfuncs.pfnSetModel(&model.entity, model.cachedPath.c_str());
		}
	}

	gEngfuncs.pfnRenderScene(&camera);
	gUiGlFuncs.renderer.setRendererMode(UI_GL_RENDERER_MODE_UI);
}

ref_viewpass_t
StudioMDLSceneComponent::CreateCameraParams(const Rml::Rectanglei& viewport, const SceneCamera& camera) const
{
	ref_viewpass_t params {};

	params.viewport[0] = viewport.Left();
	params.viewport[1] = viewport.Top();
	params.viewport[2] = viewport.Width();
	params.viewport[3] = viewport.Height();

	params.fov_x = camera.fov;
	params.viewangles[0] = camera.angles.pitch;
	params.viewangles[1] = camera.angles.yaw;
	params.viewangles[2] = camera.angles.roll;
	params.vieworigin[0] = camera.position.x;
	params.vieworigin[1] = camera.position.y;
	params.vieworigin[2] = camera.position.z;

	CalcYFov(params);

	vec3_t fwd;
	AngleVectors(params.viewangles, fwd, nullptr, nullptr);
	VectorScale(fwd, -1.0f * camera.distance, fwd);
	VectorAdd(params.vieworigin, fwd, params.vieworigin);

	return params;
}

void StudioMDLSceneComponent::CalcYFov(ref_viewpass_t& def) const
{
	if ( def.viewport[2] < 1 || def.viewport[3] < 1 || def.fov_x <= 0.0f )
	{
		return;
	}

	float x = def.viewport[2] / tanf(DEG2RADF(def.fov_x) * 0.5f);
	float half_fov_y = atanf(def.viewport[3] / x);
	def.fov_y = RAD2DEGF(half_fov_y) * 2;
}
