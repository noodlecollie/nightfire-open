#pragma once

#include <RmlUi/Core/Element.h>

class StudioMDLSceneComponentElement : public Rml::Element
{
public:
	static const char TAG_MODEL[];
	static const char TAG_CAMERA[];
	static const char EVENT_TRANSFORM_CHANGED[];
	static const char EVENT_PATH_CHANGED[];
	static const char ATTR_PITCH[];
	static const char ATTR_YAW[];
	static const char ATTR_ROLL[];
	static const char ATTR_X[];
	static const char ATTR_Y[];
	static const char ATTR_Z[];
	static const char ATTR_DISTANCE[];
	static const char ATTR_PATH[];
	static const char ATTR_FOV[];

	enum class Type
	{
		Unknown,
		Model,
		Camera
	};

	explicit StudioMDLSceneComponentElement(const Rml::String& tag);

	Type GetType() const;

protected:
	void OnAttributeChange(const Rml::ElementAttributes& changedAttributes) override;

private:
	void CheckTransformAttributesChanged(const Rml::ElementAttributes& changedAttributes);
	void CheckPathAttributesChanged(const Rml::ElementAttributes& changedAttributes);

	void AddToDictionaryIfChanged(
		Rml::Dictionary& dict,
		const Rml::ElementAttributes& changedAttributes,
		const Rml::String& key,
		bool& applied
	) const;

	void CopyAttributeIfDoesNotExist(Rml::Dictionary& dict, const Rml::String& name) const;

	void SetDefaultAttributes();

	Type m_Type = Type::Unknown;
};
