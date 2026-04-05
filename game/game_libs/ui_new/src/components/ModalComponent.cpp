#include "components/ModalComponent.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/StringUtilities.h>
#include "framework/ElementFinder.h"

static constexpr const char* const PARAM_TITLE = "title";

ModalComponent::ModalComponent(BaseMenu* parentMenu, Rml::String id) :
	BaseComponent(parentMenu, std::move(id))
{
	AddParamSpec(PARAM_TITLE, Rml::Variant(""));
}

bool ModalComponent::OnLoadFromDocument(Rml::ElementDocument*)
{
	ElementFinder finder;

	finder.Add(ComponentElementPtrPtr(), ".modal-shade>modal", &m_Elems.modal);
	finder.Add(&m_Elems.modal, ".modal-header", &m_Elems.modalHeader);
	finder.Add(&m_Elems.modal, ".modal-body", &m_Elems.modalBody);
	finder.Add(&m_Elems.modal, ".modal-footer", &m_Elems.modalFooter);

	if ( !finder.FindAll() )
	{
		return false;
	}

	LoadParams();
	return true;
}

void ModalComponent::OnUnload()
{
	m_Elems = Elements {};
}

void ModalComponent::LoadParams()
{
	const Rml::String title = GetParam(PARAM_TITLE).Get<Rml::String>();

	if ( !title.empty() )
	{
		m_Elems.modalHeader->SetInnerRML("<h2>" + Rml::StringUtilities::EncodeRml(title) + "</h2>");
	}
}
