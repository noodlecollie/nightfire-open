#pragma once

#include <memory>
#include <vector>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Variant.h>
#include "framework/MenuRequests.h"

namespace Rml
{
	class Context;
	class DataModelConstructor;
	class DataModelHandle;
	class Event;
	class ElementDocument;
	class Variant;
}  // namespace Rml

class BaseMenuObserver;

class BaseMenu
{
public:
	virtual ~BaseMenu();

	const char* Name() const;
	const char* RmlFilePath() const;
	Rml::ElementDocument* Document() const;

	const MenuRequest* CurrentRequest() const;
	void ClearCurrentRequest();

	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor);
	void DocumentLoaded(Rml::ElementDocument* document);
	void DocumentUnloaded();
	virtual void Update(float currentTime);

	bool IsDocumentVisible() const;

protected:
	BaseMenu(const char* name, const char* rmlFilePath);
	void SetCurrentRequest(MenuRequestType requestType, Rml::Dictionary options = Rml::Dictionary());

	virtual void OnBeginDocumentLoaded();
	virtual void OnEndDocumentLoaded();
	virtual void OnBeginDocumentUnloaded();
	virtual void OnEndDocumentUnloaded();
	virtual bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor);

private:
	friend class BaseMenuObserver;

	void RegisterObserver(BaseMenuObserver* component);

	const char* m_Name;
	const char* m_RmlFilePath;
	Rml::ElementDocument* m_Document = nullptr;
	std::unique_ptr<MenuRequest> m_Request;

	// Assumed to be members of the derived menu class, that live
	// as long as the derived menu does.
	std::vector<BaseMenuObserver*> m_MenuObservers;
};
