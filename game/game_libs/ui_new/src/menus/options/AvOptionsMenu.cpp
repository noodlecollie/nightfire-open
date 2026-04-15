#include "menus/options/AvOptionsMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "framework/CvarAccessor.h"

static constexpr const char* const NAME_WINDOWED = "windowed";
static constexpr const char* const CVAR_FULLSCREEN = "fullscreen";

template<typename T>
static void RefreshValueFromCvar(
	Rml::DataModelHandle& modelHandle,
	T& value,
	const char* cvarName,
	const char* bindingName,
	const std::function<T(T&&)>& transformer = std::function<T(T&&)>
)
{
	T newValue = CvarAccessor<T>::GetValue(cvarName);

	if ( transformer )
	{
		newValue = transformer(std::move(newValue));
	}

	if ( value != newValue )
	{
		newValue = value;

		if ( modelHandle )
		{
			modelHandle.DirtyVariable(bindingName);
		}
	}
}

AvOptionsMenu::AvOptionsMenu() :
	BaseOptionsMenu("av_options_menu", "resource/rml/av_options_menu.rml"),
	m_ShowHideEventListener(this, &AvOptionsMenu::ProcessShowHideEvents),
	m_CvarModel(this)
{
	m_CvarModel.AddEntry<bool>("vsync", "gl_vsync");
	m_CvarModel.AddEntry<float>("gamma", "gamma");
	m_CvarModel.AddEntry<float>("brightness", "brightness");
	m_CvarModel.AddEntry<float>("useVbo", "gl_vbo");
}

bool AvOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !BaseOptionsMenu::OnSetUpDataModelBindings(constructor) || !m_VideoModes.SetUpDataBindings(constructor) ||
		 !m_CvarModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_WINDOWED, &m_PageModel.windowed) )
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

	document->AddEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
}

void AvOptionsMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void AvOptionsMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		{
			m_VideoModes.Populate();
			RefreshValuesFromCvars();
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
	RefreshValueFromCvar<bool>(
		m_ModelHandle,
		m_PageModel.windowed,
		CVAR_FULLSCREEN,
		NAME_WINDOWED,
		[](bool&& value) -> bool
		{
			return !value;
		}
	);
}
