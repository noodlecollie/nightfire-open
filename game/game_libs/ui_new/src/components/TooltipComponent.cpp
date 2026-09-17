#include "components/TooltipComponent.h"
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementUtilities.h>
#include "framework/BaseMenu.h"
#include <cmath>

static constexpr const char* const PARAM_TYPE = "type";
static constexpr const char* const TYPE_STATIC = "static";
static constexpr const char* const TYPE_CURSOR = "cursor";

TooltipComponent::TooltipComponent(
	BaseMenu* parentMenu,
	Rml::String componentID,
	Rml::String displayElementID,
	Rml::String dataVarName
) :
	BaseComponent(parentMenu, std::move(componentID)),
	m_TooltipText {std::move(dataVarName), ""},
	m_DocumentListener(
		parentMenu,
		this,
		&TooltipComponent::HandleDocumentEvents,
		{Rml::EventId::Hide, Rml::EventId::Mousemove}
	),
	m_TooltipListener(
		parentMenu,
		this,
		&TooltipComponent::HandleTooltipTriggerEvents,
		"bigbutton[tooltip], button[tooltip], label[tooltip], .hover-tooltip[tooltip]",
		{Rml::EventId::Mouseover, Rml::EventId::Mouseout}
	),
	m_DisplayElementID(std::move(displayElementID))
{
	AddParamSpec(PARAM_TYPE, Rml::Variant(TYPE_STATIC));
}

void TooltipComponent::SetHintInnerRml(const Rml::String& rml)
{
	if ( m_TooltipDisplayElement )
	{
		m_TooltipDisplayElement->SetInnerRML(rml);
	}
	else
	{
		ASSERT(false);
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "SetHintInnerRml: Hint display element was null");
	}
}

Rml::String TooltipComponent::DefaultTooltipText() const
{
	return m_DefaultTooltipText;
}

void TooltipComponent::SetDefaultTooltipText(Rml::String text)
{
	m_DefaultTooltipText = std::move(text);

	if ( !m_CurrentTooltipSourceElement )
	{
		// Call this if we're not currently showing a tooltip
		ResetTooltip();
	}
}

bool TooltipComponent::Enabled()
{
	return m_Enabled;
}

void TooltipComponent::SetEnabled(bool enabled)
{
	if ( m_Enabled == enabled )
	{
		return;
	}

	m_Enabled = enabled;

	if ( !m_Enabled )
	{
		ResetTooltip();
	}
}

bool TooltipComponent::ComponentLoadFromDocument(Rml::ElementDocument* document)
{
	m_TooltipDisplayElement = document->GetElementById(m_DisplayElementID);

	if ( !m_TooltipDisplayElement )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"Could not find tooltip display element with ID #%s",
			m_DisplayElementID.c_str()
		);

		return false;
	}

	m_Type = TooltipType::STATIC;
	Rml::String tooltipTypeStr = GetParam(PARAM_TYPE).Get<Rml::String>();

	if ( tooltipTypeStr == TYPE_CURSOR )
	{
		m_Type = TooltipType::FOLLOW_CURSOR;
	}
	else if ( tooltipTypeStr == TYPE_STATIC )
	{
		m_Type = TooltipType::STATIC;
	}
	else
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Unrecognised tooltip type \"%s\", defaulting to \"%s\"",
			tooltipTypeStr.c_str(),
			TYPE_STATIC
		);
	}

	if ( m_Type == TooltipType::FOLLOW_CURSOR )
	{
		m_TooltipDisplayElement->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Hidden);
	}

	return true;
}

void TooltipComponent::ComponentUnload()
{
	m_TooltipDisplayElement = nullptr;
}

bool TooltipComponent::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(m_TooltipText.name, &m_TooltipText.value) )
	{
		return false;
	}

	return true;
}

void TooltipComponent::Update(float currentTime)
{
	BaseComponent::Update(currentTime);
}

void TooltipComponent::HandleDocumentEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Hide:
		{
			// The document is being hidden, so forcibly clear the tooltip.
			ResetTooltip();
			break;
		}

		case Rml::EventId::Mousemove:
		{
			if ( m_Enabled && m_Type == TooltipType::FOLLOW_CURSOR && IsShowingDisplayElement() )
			{
				UpdateTooltipPosition(event);
				break;
			}
		}

		default:
		{
			break;
		}
	}
}

void TooltipComponent::HandleTooltipTriggerEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Mouseover:
		{
			if ( m_Enabled )
			{
				SetTooltipFromMouseOver(event);
			}

			break;
		}

		case Rml::EventId::Mouseout:
		{
			Rml::Element* element = event.GetTargetElement();

			if ( element && element == m_CurrentTooltipSourceElement )
			{
				ResetTooltip();
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void TooltipComponent::SetTooltipFromMouseOver(Rml::Event& event)
{
	if ( m_CurrentTooltipSourceElement )
	{
		// We moused over another element inside the current one.
		// Don't allow setting the tooltip until the current
		// element clears it.
		return;
	}

	Rml::Element* element = event.GetTargetElement();

	if ( !element )
	{
		return;
	}

	Rml::Variant* tooltipAttr = element->GetAttribute("tooltip");

	if ( !tooltipAttr )
	{
		return;
	}

	if ( tooltipAttr->GetInto(m_TooltipText.value) && IsModelLoaded() )
	{
		DirtyVariable(m_TooltipText.name);
		m_CurrentTooltipSourceElement = element;
	}

	if ( m_Type == TooltipType::FOLLOW_CURSOR )
	{
		if ( !m_TooltipDisplayElement )
		{
			ASSERT(false);
			return;
		}

		m_TooltipDisplayElement->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Visible);

		// Annoyingly, we have to update the entire context here, so that the data model
		// contains the new value set on the variable, and we can use the correct tooltip
		// element size for positional calculations in the next step.
		m_TooltipDisplayElement->GetContext()->Update();

		UpdateTooltipPosition(event);
	}
}

void TooltipComponent::ResetTooltip()
{
	m_CurrentTooltipSourceElement = nullptr;

	if ( m_TooltipText.value != m_DefaultTooltipText )
	{
		m_TooltipText.value = m_DefaultTooltipText;
		DirtyVariable(m_TooltipText.name);
	}

	if ( m_Type == TooltipType::FOLLOW_CURSOR )
	{
		if ( !m_TooltipDisplayElement )
		{
			ASSERT(false);
			return;
		}

		m_TooltipDisplayElement->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Hidden);
	}
}

void TooltipComponent::UpdateTooltipPosition(const Rml::Event& event)
{
	static constexpr int INVALID_POS = std::numeric_limits<int>::min();

	const int x = event.GetParameter<int>("mouse_x", INVALID_POS);
	const int y = event.GetParameter<int>("mouse_y", INVALID_POS);

	if ( x == INVALID_POS || y == INVALID_POS )
	{
		ASSERT(false);
		return;
	}

	UpdateTooltipPosition({static_cast<float>(x), static_cast<float>(y)});
}

void TooltipComponent::UpdateTooltipPosition(const Rml::Vector2f& mousePos)
{
	if ( !m_TooltipDisplayElement )
	{
		ASSERT(false);
		return;
	}

	const Rml::Box& tooltipBox = m_TooltipDisplayElement->GetBox();
	const Rml::Vector2f tooltipSize = tooltipBox.GetSize(Rml::BoxArea::Border);

	// The tooltip margins can be used to offset the tooltip from the mouse pointer
	// or the edge of the screen.
	const float leftMargin = tooltipBox.GetEdge(Rml::BoxArea::Margin, Rml::BoxEdge::Left);
	const float rightMargin = tooltipBox.GetEdge(Rml::BoxArea::Margin, Rml::BoxEdge::Right);
	const float topMargin = tooltipBox.GetEdge(Rml::BoxArea::Margin, Rml::BoxEdge::Top);
	const float bottomMargin = tooltipBox.GetEdge(Rml::BoxArea::Margin, Rml::BoxEdge::Bottom);

	// This location is the tooltip's left corner, before the margins.
	// We want the middle of the tooltip to be placed relative to the mouse cursor.
	Rml::Vector2f tooltipPos {
		mousePos.x - leftMargin - (tooltipSize.x / 2),
		mousePos.y - bottomMargin - tooltipSize.y - topMargin,
	};

	if ( tooltipPos.y < 0.0f )
	{
		tooltipPos.y = mousePos.y;
	}

	const float minX = 0;
	const float maxX =
		m_TooltipDisplayElement->GetContext()->GetDimensions().x - tooltipSize.x - rightMargin - leftMargin;
	tooltipPos.x = Rml::Math::Clamp(tooltipPos.x, minX, maxX);

	Rml::Element* parent = m_TooltipDisplayElement->GetOffsetParent();

	if ( !parent )
	{
		parent = ParentMenu()->Document();
	}

	tooltipPos = tooltipPos - parent->GetAbsoluteOffset(Rml::BoxArea::Border);

	m_TooltipDisplayElement->SetProperty(Rml::PropertyId::Left, Rml::Property(tooltipPos.x, Rml::Unit::PX));
	m_TooltipDisplayElement->SetProperty(Rml::PropertyId::Top, Rml::Property(tooltipPos.y, Rml::Unit::PX));
}

bool TooltipComponent::IsShowingDisplayElement() const
{
	return m_TooltipDisplayElement && m_CurrentTooltipSourceElement;
}
