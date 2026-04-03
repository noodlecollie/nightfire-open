#include "components/ModalComponent.h"
#include <RmlUi/Core/ElementDocument.h>
#include "framework/ElementFinder.h"

ModalComponent::ModalComponent(BaseMenu* parentMenu, Rml::String id) :
	BaseComponent(parentMenu, std::move(id))
{
	AddParamSpec("title", Rml::Variant(""));
}

bool ModalComponent::OnLoadFromDocument(Rml::ElementDocument*)
{
	ElementFinder finder;

	finder.Add(ComponentElementPtrPtr(), ".modal-shade>modal", &m_Elems.modal);
	finder.Add(&m_Elems.modal, ".modal-header", &m_Elems.modalHeader);
	finder.Add(&m_Elems.modal, ".modal-body", &m_Elems.modalBody);
	finder.Add(&m_Elems.modal, ".modal-footer", &m_Elems.modalFooter);

	return finder.FindAll();
}

void ModalComponent::OnUnload()
{
	m_Elems = Elements {};
}
