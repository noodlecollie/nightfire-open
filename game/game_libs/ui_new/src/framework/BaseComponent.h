#pragma once

#include <RmlUi/Core/Types.h>

namespace Rml
{
	class Element;
	class ElementDocument;
}  // namespace Rml

class BaseMenu;

class BaseComponent
{
public:
	bool Loaded() const;

	void LoadFromDocument(Rml::ElementDocument* document);
	void Unload();

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

	BaseMenu* m_ParentMenu;
	Rml::String m_ID;
	Rml::Element* m_ComponentElement = nullptr;
	Rml::Dictionary m_ComponentParamSpec;
	Rml::Dictionary m_ComponentParams;
};
