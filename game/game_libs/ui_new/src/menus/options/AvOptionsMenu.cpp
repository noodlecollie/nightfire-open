#include "menus/options/AvOptionsMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Element.h>
#include "framework/CvarAccessor.h"

static constexpr const char* const NAME_WINDOWED = "windowed";
static constexpr const char* const NAME_SHOW_MODAL = "showModal";
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
static constexpr const char* const CVAR_FULLSCREEN = "fullscreen";
static constexpr const char* const CVAR_VID_MODE = "vid_mode";
static constexpr const char* const EVENT_APPLY_VIDEO_MODE = "applyVideoMode";

AvOptionsMenu::AvOptionsMenu() :
	BaseOptionsMenu("av_options_menu", "resource/rml/av_options_menu.rml"),
	m_Modal(this, "apply_video_mode_modal"),
	m_DocumentEventListener(this, &AvOptionsMenu::ProcessDocumentEvent),
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

bool AvOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !BaseOptionsMenu::OnSetUpDataModelBindings(constructor) || !m_VideoModes.SetUpDataBindings(constructor) ||
		 !m_CvarModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !BindInverse(constructor, m_DspOff, NAME_DSP_ENABLED) ||
		 !constructor.Bind(NAME_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.Bind(NAME_NEEDS_APPLY, &m_PageModel.needsApply) ||
		 !constructor.BindEventCallback(EVENT_APPLY_VIDEO_MODE, &AvOptionsMenu::HandleApplyVideoMode, this) )
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

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void AvOptionsMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Show, &m_DocumentEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_DocumentEventListener);
	document->AddEventListener(Rml::EventId::Resize, &m_DocumentEventListener);

	m_ResolutionDropdown =
		dynamic_cast<Rml::ElementFormControlSelect*>(document->GetElementById("resolution_dropdown"));

	ASSERT(m_ResolutionDropdown);
}

void AvOptionsMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_DocumentEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_DocumentEventListener);
	document->RemoveEventListener(Rml::EventId::Resize, &m_DocumentEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void AvOptionsMenu::ProcessDocumentEvent(Rml::Event& event)
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

			if ( m_ModelHandle )
			{
				m_ModelHandle.DirtyVariable(NAME_VIDEO_MODE_INDEX);
			}

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

		default:
		{
			break;
		}
	}
}

void AvOptionsMenu::RefreshValuesFromCvars()
{
	m_PageModel.currentWindowed = !m_FullscreenCvar.GetValue();

	if ( m_PageModel.newWindowed != m_PageModel.currentWindowed )
	{
		m_PageModel.newWindowed = m_PageModel.currentWindowed;

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(NAME_WINDOWED);
		}
	}

	m_CvarModel.Refresh(NAME_CURRENT_WIDTH);
	m_CvarModel.Refresh(NAME_CURRENT_HEIGHT);

	m_PageModel.newVideoModeIndex = -1;

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_VIDEO_MODE_INDEX);
	}

	RefreshNeedsApply();
}

void AvOptionsMenu::RefreshNeedsApply()
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

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(NAME_NEEDS_APPLY);
		}
	}
}

void AvOptionsMenu::HandleApplyVideoMode(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	HandleApplyVideoMode();
}

void AvOptionsMenu::HandleApplyVideoMode()
{
	if ( !m_PageModel.needsApply )
	{
		return;
	}

	if ( m_PageModel.currentWindowed != m_PageModel.newWindowed )
	{
		m_FullscreenCvar.SetValue(!m_PageModel.newWindowed);
	}

	if ( m_PageModel.newVideoModeIndex >= 0 )
	{
		Rml::String setModeCmd;
		Rml::FormatString(setModeCmd, "vid_setmode %d", m_PageModel.newVideoModeIndex);
		gEngfuncs.pfnClientCmd(1, setModeCmd.c_str());

		m_VideoModeCvar.SetValue(m_PageModel.newVideoModeIndex);
	}

	m_Vsync->SetValue(m_Vsync->CachedValue(), true);
	RefreshValuesFromCvars();

	Rml::Log::Message(
		Rml::Log::Type::LT_INFO,
		"Applied new video mode settings (%s %s)",
		m_PageModel.newVideoModeIndex >= 0
			? m_VideoModes.DisplayString(m_PageModel.newVideoModeIndex, VideoModesModel::LABEL).c_str()
			: "current",
		m_PageModel.newWindowed ? "windowed" : "fullscreen"
	);

	if ( !m_PageModel.newWindowed )
	{
		// TODO: Disable escape handling on menu so that modal handles it instead
		// TODO: Set expiry time for modal
		m_PageModel.showModal = true;

		if ( m_ModelHandle )
		{
			m_ModelHandle.DirtyVariable(NAME_SHOW_MODAL);
		}
	}
}

void AvOptionsMenu::HandleModalButton(bool /* keepNewVideoMode */)
{
	// TODO
}
