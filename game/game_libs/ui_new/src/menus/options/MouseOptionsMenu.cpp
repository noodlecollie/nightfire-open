#include "menus/options/MouseOptionsMenu.h"

static constexpr const char* const PROP_MOUSE_SENSITIVITY = "mouseSensitivity";
static constexpr const char* const PROP_RAW_MOUSE_INPUT = "rawMouseInput";
static constexpr const char* const PROP_MOUSE_FILTER = "mouseFilter";
static constexpr const char* const PROP_MOUSE_PITCH = "mousePitch";
static constexpr const char* const PROP_INVERT_MOUSE = "invertMouse";

MouseOptionsMenu::MouseOptionsMenu() :
	BaseOptionsMenu("mouse_options_menu", "resource/rml/mouse_options_menu.rml"),
	m_CvarModel(this)
{
	m_CvarModel.AddEntry<float>(PROP_MOUSE_SENSITIVITY, "sensitivity");
	m_CvarModel.AddEntry<bool>(PROP_RAW_MOUSE_INPUT, "m_rawinput");
	m_CvarModel.AddEntry<bool>(PROP_MOUSE_FILTER, "look_filter");
	m_MousePitch = m_CvarModel.AddEntry<float>(PROP_MOUSE_PITCH, "m_Pitch");
}

bool MouseOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !BaseOptionsMenu::OnSetUpDataModelBindings(constructor) || !m_CvarModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	const bool invertMouseBound = constructor.BindFunc(
		PROP_INVERT_MOUSE,
		[this](Rml::Variant& outVar)
		{
			outVar = Rml::Variant(m_MousePitch->CachedValue() < 0.0f);
		},
		[this](const Rml::Variant& inVar)
		{
			const bool pitchIsNegative = m_MousePitch->CachedValue() < 0.0f;
			const bool pitchShouldBeNegative = inVar.Get<bool>();

			if ( pitchIsNegative != pitchShouldBeNegative )
			{
				m_MousePitch->SetValue(-m_MousePitch->CachedValue());
			}
		}
	);

	if ( !invertMouseBound )
	{
		return false;
	}

	const bool changeListenerSet = m_CvarModel.SetChangeListener(
		PROP_MOUSE_PITCH,
		[this](const Rml::Variant&)
		{
			if ( m_ModelHandle )
			{
				m_ModelHandle.DirtyVariable(PROP_INVERT_MOUSE);
			}
		}
	);

	if ( !changeListenerSet )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}
