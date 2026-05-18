#pragma once

namespace Rml
{
	class ElementDocument;
}

class BaseMenu;

class BaseMenuObserver
{
public:
	BaseMenuObserver(BaseMenu* parentMenu);
	virtual ~BaseMenuObserver() = default;

protected:
	friend class BaseMenu;

	virtual void DocumentLoaded(Rml::ElementDocument* document);
	virtual void DocumentUnloaded(Rml::ElementDocument* document);
};
