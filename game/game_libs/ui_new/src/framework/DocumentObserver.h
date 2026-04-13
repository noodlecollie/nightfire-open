#pragma once

namespace Rml
{
	class ElementDocument;
};

class BaseMenu;

class DocumentObserver
{
public:
	explicit DocumentObserver(BaseMenu* parentMenu);
	virtual ~DocumentObserver() = default;

	virtual void DocumentLoaded(Rml::ElementDocument* document) = 0;
	virtual void DocumentUnloaded(Rml::ElementDocument* document) = 0;
};
