#include "menus/FlexTest.h"
#include <RmlUi/Core/Element.h>

FlexTest::FlexTest() :
	MenuPage("flex_test", "resource/rml/flex_test.rml"),
	m_TooltipListener(this, this, &FlexTest::HandleMouseEvents, "td", {Rml::EventId::Mouseover, Rml::EventId::Mouseout})
{
}

bool FlexTest::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind("tooltip", &m_Tooltip) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

void FlexTest::HandleMouseEvents(Rml::Event& event)
{
	if ( event.GetId() == Rml::EventId::Mouseover )
	{
		m_Tooltip = event.GetTargetElement()->GetInnerRML();
	}
	else
	{
		m_Tooltip.clear();
	}

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable("tooltip");
	}
}
