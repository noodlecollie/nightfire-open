#include "menus/options/GameplayOptionsMenu.h"
#include <RmlUi/Core/StringUtilities.h>
#include "udll_int.h"

static constexpr const char* const NAME_CROSSHAIR_ENABLED = "crosshairEnabled";
static constexpr const char* const NAME_AUTOAIM_ENABLED = "autoaimEnabled";
static constexpr const char* const NAME_VIBRATION_ENABLED = "vibrationEnabled";
static constexpr const char* const NAME_VIBRATION_INTENSITY = "vibrationIntensity";

GameplayOptionsMenu::GameplayOptionsMenu() :
	BaseOptionsMenu("gameplay_options_menu", "resource/rml/gameplay_options_menu.rml"),
	m_CvarModel(this)
{
	m_CvarModel.AddEntry<bool>(NAME_CROSSHAIR_ENABLED, "crosshair");
	m_CvarModel.AddEntry<bool>(NAME_AUTOAIM_ENABLED, "sv_aim");
	m_CvarModel.AddEntry<bool>(NAME_VIBRATION_ENABLED, "vibration_enable");
	m_CvarModel.AddEntry<float>(NAME_VIBRATION_INTENSITY, "vibration_length");

	m_CvarModel.SetChangeListener(
		NAME_VIBRATION_INTENSITY,
		[this](const Rml::Variant& newValue)
		{
			Rml::String cmd;
			Rml::FormatString(cmd, "vibrate %f", newValue.Get<float>());
			gEngfuncs.pfnClientCmd(0, cmd.c_str());
		}
	);
}

bool GameplayOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseOptionsMenu::OnSetUpDataModelBindings(constructor) && m_CvarModel.SetUpDataBindings(constructor);
}
