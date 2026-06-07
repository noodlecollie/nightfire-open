#pragma once

#include <RmlUi/Core/Types.h>
#include "framework/BaseMenuObserver.h"

namespace Rml
{
	class Element;
	class ElementDocument;
}  // namespace Rml

class BaseMenu;

class BaseComponent : private BaseMenuObserver
{
public:
	bool Loaded() const;

	Rml::Variant GetParam(const Rml::String& name) const;

protected:
	explicit BaseComponent(BaseMenu* parentMenu, Rml::String id);

	Rml::Element* ComponentElement() const;
	Rml::Element* const* ComponentElementPtrPtr() const;

	void AddParamSpec(Rml::String name, Rml::Variant defaultValue);

	virtual bool ComponentLoadFromDocument(Rml::ElementDocument* document);
	virtual void ComponentUnload();

private:
	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

	bool CheckLoaded(const char* operation);
	void LoadParams();

	Rml::String m_ID;
	Rml::Element* m_ComponentElement = nullptr;
	Rml::Dictionary m_ComponentParamSpec;
	Rml::Dictionary m_ComponentParams;
};
