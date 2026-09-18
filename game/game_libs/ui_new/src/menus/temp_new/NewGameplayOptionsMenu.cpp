#include "menus/temp_new/NewGameplayOptionsMenu.h"
#include <RmlUi/Core/StringUtilities.h>
#include "udll_int.h"

static constexpr const char* const NAME_CROSSHAIR_ENABLED = "crosshairEnabled";
static constexpr const char* const NAME_AUTOAIM_ENABLED = "autoaimEnabled";
static constexpr const char* const NAME_VIBRATION_ENABLED = "vibrationEnabled";
static constexpr const char* const NAME_VIBRATION_LENGTH = "vibrationLength";
static constexpr const char* const EVENT_TEST_VIBRATION = "testVibration";

NewGameplayOptionsMenu::NewGameplayOptionsMenu() :
	NewBaseOptionsMenu("new_gameplay_options_menu", "gameplay_options_menu.rml"),
	m_CvarModel(this),
	m_TooltipComponent(this, "tooltip_component", "tooltip_container", "tooltipText")
{
	m_CvarModel.AddEntry<bool>(NAME_CROSSHAIR_ENABLED, "crosshair");
	m_CvarModel.AddEntry<bool>(NAME_AUTOAIM_ENABLED, "sv_aim");
	m_VibrationEnabled = m_CvarModel.AddEntry<bool>(NAME_VIBRATION_ENABLED, "vibration_enable");
	m_CvarModel.AddEntry<float>(NAME_VIBRATION_LENGTH, "vibration_length");
}

bool NewGameplayOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !NewBaseOptionsMenu::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.BindEventCallback(EVENT_TEST_VIBRATION, &NewGameplayOptionsMenu::HandleTestVibration, this) )
	{
		return false;
	}

	return true;
}

void NewGameplayOptionsMenu::HandleTestVibration(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_VibrationEnabled->CachedValue() )
	{
		// The "1" is so that this lasts 1x the value of vibration_length.
		gEngfuncs.pfnClientCmd(false, "vibrate 1");
	}
}
