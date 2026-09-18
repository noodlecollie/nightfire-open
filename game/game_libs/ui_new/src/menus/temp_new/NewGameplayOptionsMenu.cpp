#include "menus/temp_new/NewGameplayOptionsMenu.h"
#include <RmlUi/Core/StringUtilities.h>
#include "udll_int.h"

static constexpr const char* const NAME_CROSSHAIR_ENABLED = "crosshairEnabled";
static constexpr const char* const NAME_AUTOAIM_ENABLED = "autoaimEnabled";
static constexpr const char* const NAME_VIBRATION_ENABLED = "vibrationEnabled";
static constexpr const char* const NAME_VIBRATION_INTENSITY = "vibrationIntensity";

NewGameplayOptionsMenu::NewGameplayOptionsMenu() :
	NewBaseOptionsMenu("new_gameplay_options_menu", "gameplay_options_menu.rml"),
	m_CvarModel(this),
	m_TooltipComponent(this, "tooltip_component", "tooltip_container", "tooltipText")
{
	m_CvarModel.AddEntry<bool>(NAME_CROSSHAIR_ENABLED, "crosshair");
	m_CvarModel.AddEntry<bool>(NAME_AUTOAIM_ENABLED, "sv_aim");
	m_CvarModel.AddEntry<bool>(NAME_VIBRATION_ENABLED, "vibration_enable");
	m_CvarModel.AddEntry<float>(NAME_VIBRATION_INTENSITY, "vibration_length");

	m_CvarModel.SetChangeListener(
		NAME_VIBRATION_INTENSITY,
		[](const Rml::Variant& newValue)
		{
			gEngfuncs.pfnClientCmd(false, Rml::CreateString("vibrate %f", newValue.Get<float>()).c_str());
		}
	);
}
