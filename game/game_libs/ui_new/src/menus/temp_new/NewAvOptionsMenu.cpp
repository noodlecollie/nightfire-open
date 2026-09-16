#include "menus/temp_new/NewAvOptionsMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/Context.h>
#include <cmath>
#include "framework/CvarAccessor.h"
#include "rmlui/RmlUiBackend.h"
#include "rmlui/Utils.h"

static constexpr const char* const NAME_WINDOWED = "windowed";
static constexpr const char* const NAME_SHOW_MODAL = "showModal";
static constexpr const char* const NAME_MODAL_TIME_REMAINING = "modalTimeRemaining";
static constexpr const char* const NAME_NEEDS_APPLY = "needsApply";
static constexpr const char* const NAME_CURRENT_WIDTH = "currentWidth";
static constexpr const char* const NAME_CURRENT_HEIGHT = "currentHeight";
static constexpr const char* const NAME_VIDEO_MODE_INDEX = "videoModeIndex";
static constexpr const char* const NAME_VSYNC_ENABLED = "vsyncEnabled";
static constexpr const char* const NAME_GAMMA = "gamma";
static constexpr const char* const NAME_BRIGHTNESS = "brightness";
static constexpr const char* const NAME_USE_VBO = "useVbo";
static constexpr const char* const NAME_SFX_VOLUME = "sfxVolume";
static constexpr const char* const NAME_MUSIC_VOLUME = "musicVolume";
static constexpr const char* const NAME_DSP_OFF = "dspOff";
static constexpr const char* const NAME_DSP_ENABLED = "dspEnabled";
static constexpr const char* const NAME_MUTE_WHEN_FOCUS_LOST = "muteWhenFocusLost";
static constexpr const char* const NAME_MENU_TRANSPARENCY_CONTROL = "menuTransparencyControl";
static constexpr const char* const CVAR_FULLSCREEN = "fullscreen";
static constexpr const char* const CVAR_VID_MODE = "vid_mode";
static constexpr const char* const EVENT_APPLY_VIDEO_MODE = "applyVideoMode";

NewAvOptionsMenu::NewAvOptionsMenu() :
	NewBaseOptionsMenu("new_av_options_menu", "av_options_menu.rml"),
	m_Modal(this, "apply_video_mode_modal"),
	m_TooltipComponent(this, "tooltip_component", "tooltip_content", "tooltipText"),
	m_VideoModes(this),
	m_DocumentEventListener(
		this,
		&NewAvOptionsMenu::ProcessDocumentEvent,
		{Rml::EventId::Show, Rml::EventId::Hide, Rml::EventId::Resize, Rml::EventId::Keydown, Rml::EventId::Mouseup}
	),
	m_SliderEventListener(
		this,
		&NewAvOptionsMenu::ProcessSliderEventForMenuTransparency,
		"input[type='range'][use-transparent-menu]",
		{Rml::EventId::Mousedown, Rml::EventId::Mouseup}
	),
	m_CvarModel(this),
	m_FullscreenCvar(CVAR_FULLSCREEN),
	m_VideoModeCvar(CVAR_VID_MODE)
{
	m_Vsync = m_CvarModel.AddEntry<bool>(NAME_VSYNC_ENABLED, "gl_vsync");
	m_CvarModel.AddEntry<float>(NAME_GAMMA, "gamma");
	m_CvarModel.AddEntry<float>(NAME_BRIGHTNESS, "brightness");
	m_CvarModel.AddEntry<bool>(NAME_USE_VBO, "gl_vbo");
	m_CvarModel.AddEntry<float>(NAME_SFX_VOLUME, "volume");
	m_CvarModel.AddEntry<float>(NAME_MUSIC_VOLUME, "MP3Volume");
	m_CvarModel.AddEntry<bool>(NAME_MUTE_WHEN_FOCUS_LOST, "snd_mute_losefocus");
	m_DspOff = m_CvarModel.AddEntry<bool>(NAME_DSP_OFF, "dsp_off");
	m_CvarModel.AddEntry<int>(NAME_CURRENT_WIDTH, "width");
	m_CvarModel.AddEntry<int>(NAME_CURRENT_HEIGHT, "height");

	m_Modal.SetButtonClickCallback(
		[this](Rml::Event&, size_t buttonIndex, const Rml::Variant&)
		{
			HandleModalButton(buttonIndex == 1);
		}
	);
}

void NewAvOptionsMenu::Update(float currentTime)
{
	NewBaseOptionsMenu::Update(currentTime);

	if ( m_PageModel.showModal )
	{
		if ( m_PageModel.modalExpiry > gpGlobals->time )
		{
			// Round up to an integer
			int remaining = static_cast<int>(std::ceil(m_PageModel.modalExpiry - gpGlobals->time));

			if ( remaining != m_PageModel.modalTimeRemaining )
			{
				m_PageModel.modalTimeRemaining = remaining;
				DirtyVariable(NAME_MODAL_TIME_REMAINING);
			}
		}
		else
		{
			HandleModalButton(false);
		}
	}

	// Slight hack? Not sure of the "correct" way to deal with this.
	// If the user is interacting with a slider but drags the mouse out of the game window,
	// this will catch that and reset the transparency.
	if ( !m_PageModel.menuTransparencyControl.empty() &&
		 !RmlUiBackend::StaticInstance().GetRmlContext()->IsMouseInteracting() )
	{
		m_PageModel.menuTransparencyControl.clear();
		DirtyVariable(NAME_MENU_TRANSPARENCY_CONTROL);
	}
}

bool NewAvOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !NewBaseOptionsMenu::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !BindInverse(constructor, m_DspOff, NAME_DSP_ENABLED) ||
		 !constructor.Bind(NAME_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.Bind(NAME_MODAL_TIME_REMAINING, &m_PageModel.modalTimeRemaining) ||
		 !constructor.Bind(NAME_NEEDS_APPLY, &m_PageModel.needsApply) ||
		 !constructor.Bind(NAME_MENU_TRANSPARENCY_CONTROL, &m_PageModel.menuTransparencyControl) ||
		 !constructor.BindEventCallback(EVENT_APPLY_VIDEO_MODE, &NewAvOptionsMenu::HandleApplyVideoMode, this) )
	{
		return false;
	}

	const bool vidModeIndexBound = constructor.BindFunc(
		NAME_VIDEO_MODE_INDEX,
		[this](Rml::Variant& outVal)
		{
			outVal = Rml::Variant(m_PageModel.newVideoModeIndex);
		},
		[this](const Rml::Variant& inVal)
		{
			m_PageModel.newVideoModeIndex = inVal.Get<int>();
			RefreshNeedsApply();
		}
	);

	const bool windowedBound = constructor.BindFunc(
		NAME_WINDOWED,
		[this](Rml::Variant& outVal)
		{
			outVal = Rml::Variant(m_PageModel.newWindowed);
		},
		[this](const Rml::Variant& inVal)
		{
			m_PageModel.newWindowed = inVal.Get<bool>();
			RefreshNeedsApply();
		}
	);

	if ( !vidModeIndexBound || !windowedBound )
	{
		return false;
	}

	return true;
}

void NewAvOptionsMenu::OnDocumentLoaded()
{
	MenuPage::OnDocumentLoaded();

	m_ResolutionDropdown =
		dynamic_cast<Rml::ElementFormControlSelect*>(Document()->GetElementById("resolution_dropdown"));

	ASSERT(m_ResolutionDropdown);
}

void NewAvOptionsMenu::OnDocumentUnloaded()
{
	m_ResolutionDropdown = nullptr;
	MenuPage::OnDocumentUnloaded();
}

void NewAvOptionsMenu::ProcessDocumentEvent(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			m_VideoModes.Populate();
			RefreshValuesFromCvars();
			break;
		}

		case Rml::EventId::Hide:
		{
			if ( m_PageModel.showModal )
			{
				HandleModalButton(false);
			}

			break;
		}

		case Rml::EventId::Resize:
		{
			m_CvarModel.Refresh(NAME_CURRENT_WIDTH);
			m_CvarModel.Refresh(NAME_CURRENT_HEIGHT);
			DirtyVariable(NAME_VIDEO_MODE_INDEX);

			// Update the resolution dropdown if it says "Current (width x height)".
			// The width and height text displayed in the relevant option element is
			// updated automatically, but the top-level element's text is not.
			// This forces the displayed selection to be refreshed.
			if ( m_ResolutionDropdown && m_PageModel.newVideoModeIndex < 0 )
			{
				m_ResolutionDropdown->SetSelection(m_ResolutionDropdown->GetSelection());
			}

			break;
		}

		case Rml::EventId::Keydown:
		{
			const int keyId = GetEventKeyId(event);

			if ( keyId == Rml::Input::KI_ESCAPE && m_PageModel.showModal )
			{
				event.StopPropagation();
				HandleModalButton(false);
			}

			break;
		}

		case Rml::EventId::Mouseup:
		{
			// If the user is dragging a slider and moves the mouse cursor out
			// of the slider's physical range before letting go of the button,
			// the slider will not receive this event. We catch it here in order
			// to properly reset the transparency setting.
			if ( !m_PageModel.menuTransparencyControl.empty() )
			{
				m_PageModel.menuTransparencyControl.clear();
				DirtyVariable(NAME_MENU_TRANSPARENCY_CONTROL);
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void NewAvOptionsMenu::ProcessSliderEventForMenuTransparency(Rml::Event& event)
{
	Rml::String controlValue;

	if ( event.GetId() == Rml::EventId::Mousedown )
	{
		Rml::Element* element = event.GetTargetElement();

		if ( element->GetTagName() != "sliderbar" )
		{
			// We only want to listen for events on the slider bar itself.
			return;
		}

		Rml::Element* parent = element ? element->GetParentNode() : nullptr;

		if ( parent )
		{
			ASSERT(element->GetTagName() == "sliderbar");
			ASSERT(parent->GetTagName() == "input");

			Rml::Variant* variant = parent->GetAttribute("data-value");

			if ( variant )
			{
				variant->GetInto(controlValue);
			}
		}
	}

	if ( controlValue != m_PageModel.menuTransparencyControl )
	{
		m_PageModel.menuTransparencyControl = controlValue;
		DirtyVariable(NAME_MENU_TRANSPARENCY_CONTROL);
	}
}

void NewAvOptionsMenu::RefreshValuesFromCvars()
{
	m_PageModel.currentWindowed = !m_FullscreenCvar.GetValue();

	if ( m_PageModel.newWindowed != m_PageModel.currentWindowed )
	{
		m_PageModel.newWindowed = m_PageModel.currentWindowed;
		DirtyVariable(NAME_WINDOWED);
	}

	m_CvarModel.Refresh(NAME_CURRENT_WIDTH);
	m_CvarModel.Refresh(NAME_CURRENT_HEIGHT);

	m_PageModel.newVideoModeIndex = -1;
	DirtyVariable(NAME_VIDEO_MODE_INDEX);

	RefreshNeedsApply();
}

void NewAvOptionsMenu::RefreshNeedsApply()
{
	bool desiredValue = false;

	if ( m_PageModel.newVideoModeIndex >= 0 )
	{
		desiredValue = true;
	}

	if ( m_PageModel.currentWindowed != m_PageModel.newWindowed )
	{
		desiredValue = true;
	}

	if ( m_PageModel.needsApply != desiredValue )
	{
		m_PageModel.needsApply = desiredValue;
		DirtyVariable(NAME_NEEDS_APPLY);
	}
}

void NewAvOptionsMenu::HandleApplyVideoMode(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	HandleApplyVideoMode();
}

void NewAvOptionsMenu::HandleApplyVideoMode()
{
	if ( !m_PageModel.needsApply )
	{
		return;
	}

	CreateRevertInfo();
	ApplyVideoSettings(m_PageModel.newVideoModeIndex, m_PageModel.newWindowed);

	// Only bother with modal/revert if we're going fullscreen.
	if ( !m_PageModel.currentWindowed )
	{
		m_PageModel.showModal = true;
		m_PageModel.modalExpiry = gpGlobals->time + 10.0f;
		SetRequestPopOnEscapeKey(false);
		DirtyVariable(NAME_SHOW_MODAL);
	}
	else
	{
		m_RevertInfo.reset();
	}
}

void NewAvOptionsMenu::HandleModalButton(bool keepNewVideoMode)
{
	if ( !keepNewVideoMode )
	{
		ApplyRevertInfo();
	}

	m_RevertInfo.reset();
	m_PageModel.showModal = false;
	m_PageModel.modalTimeRemaining = 0;
	SetRequestPopOnEscapeKey(true);

	DirtyVariable(NAME_SHOW_MODAL);
	DirtyVariable(NAME_MODAL_TIME_REMAINING);
}

void NewAvOptionsMenu::CreateRevertInfo()
{
	const Rml::Vector2i ctxDims = Document()->GetContext()->GetDimensions();

	m_RevertInfo.reset(new RevertInfo {});
	m_RevertInfo->width = ctxDims.x;
	m_RevertInfo->height = ctxDims.y;
	m_RevertInfo->wasWindowed = m_PageModel.currentWindowed;

	Rml::Log::Message(
		Rml::Log::Type::LT_DEBUG,
		"NewAvOptionsMenu::CreateRevertInfo: %dx%d %s",
		m_RevertInfo->width,
		m_RevertInfo->height,
		m_RevertInfo->wasWindowed ? "windowed" : "fullscreen"
	);
}

void NewAvOptionsMenu::ApplyRevertInfo()
{
	if ( !m_RevertInfo )
	{
		return;
	}

	size_t modeRow = 0;
	int vidMode = -1;
	if ( m_VideoModes.RowForDimensions(m_RevertInfo->width, m_RevertInfo->height, modeRow) )
	{
		vidMode = m_VideoModes.VideoMode(modeRow);
	}

	ApplyVideoSettings(vidMode, m_RevertInfo->wasWindowed);
}

void NewAvOptionsMenu::ApplyVideoSettings(int vidMode, bool windowed)
{
	Rml::Log::Message(
		Rml::Log::Type::LT_INFO,
		"Changing video mode to %dx%d %s",
		vidMode >= 0 ? m_VideoModes.Width(static_cast<size_t>(vidMode)) : m_PageModel.currentWidth,
		vidMode >= 0 ? m_VideoModes.Height(static_cast<size_t>(vidMode)) : m_PageModel.currentHeight,
		windowed ? "windowed" : "fullscreen"
	);

	if ( m_PageModel.currentWindowed != windowed )
	{
		m_FullscreenCvar.SetValue(!windowed);
	}

	if ( vidMode >= 0 )
	{
		gEngfuncs.pfnClientCmd(true, Rml::CreateString("vid_setmode %d", vidMode).c_str());
		m_VideoModeCvar.SetValue(vidMode);
	}

	m_Vsync->SetValue(m_Vsync->CachedValue(), true);
	RefreshValuesFromCvars();
}
