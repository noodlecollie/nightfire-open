#pragma once

#include <RmlUi/Core/Types.h>
#include "framework/DocumentObserver.h"

namespace Rml
{
	class Element;
	class ElementDocument;
}  // namespace Rml

class BaseMenu;

class BaseComponent : public DocumentObserver
{
public:
	bool Loaded() const;

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

	Rml::Variant GetParam(const Rml::String& name) const;

protected:
	explicit BaseComponent(BaseMenu* parentMenu, Rml::String id);

	Rml::Element* ComponentElement() const;
	Rml::Element* const* ComponentElementPtrPtr() const;

	void AddParamSpec(Rml::String name, Rml::Variant defaultValue);

	virtual bool OnLoadFromDocument(Rml::ElementDocument* document);
	virtual void OnUnload();

private:
	bool CheckLoaded(const char* operation);
	void LoadParams();

	Rml::String m_ID;
	Rml::Element* m_ComponentElement = nullptr;
	Rml::Dictionary m_ComponentParamSpec;
	Rml::Dictionary m_ComponentParams;
};
