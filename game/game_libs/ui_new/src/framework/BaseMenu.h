#pragma once

#include <memory>
#include <vector>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Variant.h>
#include <RmlUi/Core/EventListener.h>

namespace Rml
{
	class Context;
	class DataModelConstructor;
	class DataModelHandle;
	class Event;
	class ElementDocument;
	class Variant;
}  // namespace Rml

class BaseComponent;

enum class MenuRequestType
{
	PushMenu,
	PopMenu
};

struct MenuRequest
{
	MenuRequestType requestType;
	Rml::VariantList args;

	explicit MenuRequest(MenuRequestType inRequestType, const Rml::VariantList& inArgs = Rml::VariantList()) :
		requestType(inRequestType),
		args(inArgs)
	{
	}
};

// TODO: Don't make the menu itself an event listener, make an object within it.
// Otherwise, if subclasses listen to the same events again, the event listener
// function will be called multiple times!
class BaseMenu : public Rml::EventListener
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

	void ProcessEvent(Rml::Event& event) override;

protected:
	BaseMenu(const char* name, const char* rmlFilePath);
	void SetCurrentRequest(MenuRequestType requestType, const Rml::VariantList& args = Rml::VariantList());

	virtual void OnBeginDocumentLoaded();
	virtual void OnEndDocumentLoaded();
	virtual void OnBeginDocumentUnloaded();
	virtual void OnEndDocumentUnloaded();
	virtual void OnDocumentShown();
	virtual void OnDocumentHidden();
	virtual bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor);

private:
	friend class BaseComponent;

	void RegisterComponent(BaseComponent* component);

	const char* m_Name;
	const char* m_RmlFilePath;
	Rml::ElementDocument* m_Document = nullptr;
	std::unique_ptr<MenuRequest> m_Request;

	// Assumed to be members of the derived menu class, that live
	// as long as the derived menu does.
	std::vector<BaseComponent*> m_Components;
};
