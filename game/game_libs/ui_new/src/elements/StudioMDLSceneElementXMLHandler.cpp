#include "elements/StudioMDLSceneElementXMLHandler.h"
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/XMLParser.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/Factory.h>
#include "elements/StudioMDLSceneElement.h"
#include "elements/StudioMDLSceneItemElement.h"
#include "UIDebug.h"

constexpr const char* const TAG_MODEL = "model";
constexpr const char* const TAG_CAMERA = "camera";
constexpr const char* const ATTR_PITCH = "pitch";
constexpr const char* const ATTR_YAW = "yaw";
constexpr const char* const ATTR_ROLL = "roll";
constexpr const char* const ATTR_X = "x";
constexpr const char* const ATTR_Y = "y";
constexpr const char* const ATTR_Z = "z";
constexpr const char* const ATTR_DISTANCE = "distance";
constexpr const char* const ATTR_PATH = "path";
constexpr const char* const ATTR_FOV = "fov";

template<typename T>
bool SetAttribute(Rml::ElementAttributes atts, const Rml::String& key, T& out, const T& defaultValue = T())
{
	const auto it = atts.find(key);

	if ( it == atts.end() )
	{
		return false;
	}

	out = it->second.Get<T>(defaultValue);
	return true;
}

Rml::Element* StudioMDLSceneElementXMLHandler::ElementStart(
	Rml::XMLParser* parser,
	const Rml::String& name,
	const Rml::XMLAttributes& attributes
)
{
	m_CurrentElementTag = name;
	const Rml::XMLParser::ParseFrame* frame = parser->GetParseFrame();

	if ( m_CurrentElementTag == StudioMDLSceneElement::TAG )
	{
		parser->PushHandler(StudioMDLSceneElement::TAG);

		Rml::ElementPtr element =
			Rml::Factory::InstanceElement(frame->element, m_CurrentElementTag, m_CurrentElementTag, attributes);

		m_CurrentElement = rmlui_dynamic_cast<StudioMDLSceneElement*>(element.get());

		ASSERT(m_CurrentElement);

		if ( !m_CurrentElement )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_ERROR,
				"StudioMDLSceneElementXMLHandler: Failed to create scene element"
			);
			return nullptr;
		}

		return frame->element->AppendChild(std::move(element));
	}
	else if ( m_CurrentElementTag == TAG_MODEL )
	{
		HandleAddModel(attributes);
		return nullptr;
	}
	else if ( m_CurrentElementTag == TAG_CAMERA )
	{
		HandleAddCamera(attributes);
		return nullptr;
	}
	else
	{
		// Anything not handled above should be parsed as a normal element.
		parser->PushDefaultHandler();

		Rml::ElementPtr element =
			Rml::Factory::InstanceElement(parser->GetParseFrame()->element, "*", name, attributes);

		return frame->element->AppendChild(std::move(element));
	}
}

bool StudioMDLSceneElementXMLHandler::ElementEnd(Rml::XMLParser* /* parser */, const Rml::String& name)
{
	if ( name == StudioMDLSceneElement::TAG )
	{
		ASSERT(m_CurrentElement);
		m_CurrentElement = nullptr;
	}

	return true;
}

bool StudioMDLSceneElementXMLHandler::ElementData(
	Rml::XMLParser* /* parser */,
	const Rml::String& /* data */,
	Rml::XMLDataType /* type */
)
{
	// This handler should not be used for any element which requires data parsing.
	return true;
}

void StudioMDLSceneElementXMLHandler::HandleAddModel(const Rml::XMLAttributes& attributes)
{
	ASSERT(m_CurrentElement);

	if ( !m_CurrentElement )
	{
		return;
	}

	StudioMDLSceneElement::SceneModel model {};

	SetAttribute(attributes, ATTR_X, model.position.x);
	SetAttribute(attributes, ATTR_Y, model.position.y);
	SetAttribute(attributes, ATTR_Z, model.position.z);
	SetAttribute(attributes, ATTR_PITCH, model.angles.x);
	SetAttribute(attributes, ATTR_YAW, model.angles.y);
	SetAttribute(attributes, ATTR_ROLL, model.angles.z);
	SetAttribute(attributes, ATTR_PATH, model.path);

	m_CurrentElement->AddRmlModel(std::move(model));
}

void StudioMDLSceneElementXMLHandler::HandleAddCamera(const Rml::XMLAttributes& attributes)
{
	ASSERT(m_CurrentElement);

	if ( !m_CurrentElement )
	{
		return;
	}

	StudioMDLSceneElement::SceneCamera camera {};

	SetAttribute(attributes, ATTR_X, camera.position.x);
	SetAttribute(attributes, ATTR_Y, camera.position.y);
	SetAttribute(attributes, ATTR_Z, camera.position.z);
	SetAttribute(attributes, ATTR_PITCH, camera.angles.x);
	SetAttribute(attributes, ATTR_YAW, camera.angles.y);
	SetAttribute(attributes, ATTR_ROLL, camera.angles.z);
	SetAttribute(attributes, ATTR_DISTANCE, camera.distance);
	SetAttribute(attributes, ATTR_FOV, camera.fov);

	m_CurrentElement->AddRmlCamera(std::move(camera));
}
