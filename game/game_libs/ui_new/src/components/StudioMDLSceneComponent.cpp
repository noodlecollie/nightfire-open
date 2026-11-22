#include "components/StudioMDLSceneComponent.h"
#include "framework/ElementFinder.h"
#include "elements/StudioMDLSceneElement.h"
#include "elements/StudioMDLSceneItemElement.h"

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

StudioMDLSceneComponent::StudioMDLSceneComponent(BaseMenu* parentMenu, Rml::String id) :
	BaseComponent(parentMenu, std::move(id))
{
}

bool StudioMDLSceneComponent::ComponentLoadFromDocument(Rml::ElementDocument*)
{
	ElementFinder finder;

	finder.Add(ComponentElementPtrPtr(), "studiomdlscene", &m_Elems.scene);
	finder.AddMulti(&m_Elems.scene, "model", &m_Elems.models);
	finder.AddMulti(&m_Elems.scene, "camera", &m_Elems.cameras);

	if ( !finder.FindAll() )
	{
		return false;
	}

	// TODO: Attach to events for adding/removing other scene items?
	// Do we want to support this?

	CreateDataEntries();

	return true;
}

void StudioMDLSceneComponent::ComponentUnload()
{
	m_Elems = {};
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

	Rml::Log::Message(
		Rml::Log::Type::LT_DEBUG,
		"StudioMDLSceneComponent: Creating data entries for %zu models and %zu cameras",
		m_Elems.models.size(),
		m_Elems.cameras.size()
	);

	m_Model.models.resize(m_Elems.models.size());
	m_Model.cameras.resize(m_Elems.cameras.size());

	DirtyVariable(NAME_MODELS);
	DirtyVariable(NAME_CAMERAS);
}
