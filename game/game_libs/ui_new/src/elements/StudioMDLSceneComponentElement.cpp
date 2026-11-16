#include "elements/StudioMDLSceneComponentElement.h"

const char StudioMDLSceneComponentElement::TAG_MODEL[] = "model";
const char StudioMDLSceneComponentElement::TAG_CAMERA[] = "camera";
const char StudioMDLSceneComponentElement::EVENT_TRANSFORM_CHANGED[] = "transformChanged";
const char StudioMDLSceneComponentElement::EVENT_PATH_CHANGED[] = "pathChanged";
const char StudioMDLSceneComponentElement::ATTR_PITCH[] = "pitch";
const char StudioMDLSceneComponentElement::ATTR_YAW[] = "yaw";
const char StudioMDLSceneComponentElement::ATTR_ROLL[] = "roll";
const char StudioMDLSceneComponentElement::ATTR_X[] = "x";
const char StudioMDLSceneComponentElement::ATTR_Y[] = "y";
const char StudioMDLSceneComponentElement::ATTR_Z[] = "z";
const char StudioMDLSceneComponentElement::ATTR_DISTANCE[] = "distance";
const char StudioMDLSceneComponentElement::ATTR_PATH[] = "path";

StudioMDLSceneComponentElement::StudioMDLSceneComponentElement(const Rml::String& tag) :
	Rml::Element(tag)
{
	if ( tag == TAG_MODEL )
	{
		m_Type = Type::Model;
	}
	else if ( tag == TAG_CAMERA )
	{
		m_Type = Type::Camera;
	}

	SetDefaultAttributes();
}

StudioMDLSceneComponentElement::Type StudioMDLSceneComponentElement::GetType() const
{
	return m_Type;
}

void StudioMDLSceneComponentElement::OnAttributeChange(const Rml::ElementAttributes& changedAttributes)
{
	Rml::Element::OnAttributeChange(changedAttributes);

	CheckTransformAttributesChanged(changedAttributes);
	CheckPathAttributesChanged(changedAttributes);
}

void StudioMDLSceneComponentElement::CheckTransformAttributesChanged(const Rml::ElementAttributes& changedAttributes)
{
	Rml::Dictionary atts;
	bool applied = false;

	AddToDictionaryIfChanged(atts, changedAttributes, ATTR_PITCH, applied);
	AddToDictionaryIfChanged(atts, changedAttributes, ATTR_YAW, applied);
	AddToDictionaryIfChanged(atts, changedAttributes, ATTR_ROLL, applied);

	switch ( m_Type )
	{
		case Type::Model:
		{
			AddToDictionaryIfChanged(atts, changedAttributes, ATTR_X, applied);
			AddToDictionaryIfChanged(atts, changedAttributes, ATTR_Y, applied);
			AddToDictionaryIfChanged(atts, changedAttributes, ATTR_Z, applied);
			break;
		}

		case Type::Camera:
		{
			AddToDictionaryIfChanged(atts, changedAttributes, ATTR_DISTANCE, applied);
			break;
		}

		default:
		{
			break;
		}
	}

	if ( !applied )
	{
		// Nothing we care about changed.
		return;
	}

	CopyAttributeIfDoesNotExist(atts, ATTR_PITCH);
	CopyAttributeIfDoesNotExist(atts, ATTR_YAW);
	CopyAttributeIfDoesNotExist(atts, ATTR_ROLL);

	switch ( m_Type )
	{
		case Type::Model:
		{
			CopyAttributeIfDoesNotExist(atts, ATTR_X);
			CopyAttributeIfDoesNotExist(atts, ATTR_Y);
			CopyAttributeIfDoesNotExist(atts, ATTR_Z);
			break;
		}

		case Type::Camera:
		{
			CopyAttributeIfDoesNotExist(atts, ATTR_DISTANCE);
			break;
		}

		default:
		{
			break;
		}
	}

	DispatchEvent(EVENT_TRANSFORM_CHANGED, atts);
}

void StudioMDLSceneComponentElement::CheckPathAttributesChanged(const Rml::ElementAttributes& changedAttributes)
{
	if ( m_Type != Type::Model )
	{
		return;
	}

	Rml::Dictionary atts;
	bool applied = false;

	AddToDictionaryIfChanged(atts, changedAttributes, ATTR_PATH, applied);

	if ( !applied )
	{
		// Nothing we care about changed.
		return;
	}

	CopyAttributeIfDoesNotExist(atts, ATTR_PATH);
	DispatchEvent(EVENT_PATH_CHANGED, atts);
}

void StudioMDLSceneComponentElement::AddToDictionaryIfChanged(
	Rml::Dictionary& dict,
	const Rml::ElementAttributes& changedAttributes,
	const Rml::String& key,
	bool& applied
) const
{
	const auto it = changedAttributes.find(key);

	if ( it != changedAttributes.end() )
	{
		dict[key] = it->second;
		applied = true;
	}
}

void StudioMDLSceneComponentElement::CopyAttributeIfDoesNotExist(Rml::Dictionary& dict, const Rml::String& name) const
{
	if ( dict.find(name) == dict.end() )
	{
		const Rml::Variant* var = GetAttribute(name);
		dict[name] = var ? (*var) : Rml::Variant();
	}
}

void StudioMDLSceneComponentElement::SetDefaultAttributes()
{
	Rml::ElementAttributes atts;

	atts[ATTR_X] = Rml::Variant(0.0f);
	atts[ATTR_Y] = Rml::Variant(0.0f);
	atts[ATTR_Z] = Rml::Variant(0.0f);
	atts[ATTR_PITCH] = Rml::Variant(0.0f);
	atts[ATTR_YAW] = Rml::Variant(0.0f);
	atts[ATTR_ROLL] = Rml::Variant(0.0f);

	switch ( m_Type )
	{
		case Type::Model:
		{
			atts[ATTR_PATH] = Rml::Variant("");
			break;
		}

		case Type::Camera:
		{
			atts[ATTR_DISTANCE] = Rml::Variant(0.0f);
			break;
		}

		default:
		{
			break;
		}
	}

	SetAttributes(atts);
}
