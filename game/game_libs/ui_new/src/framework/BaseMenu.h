#pragma once

#include <memory>
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
}  // namespace Rml

enum class MenuRequestType
{
	PushMenu,
	PopMenu
};

enum MenuAttributeFlag
{
	MenuAttrPopOnEscape = 1 << 0,
	MenuAttrRegisterPushPop = 1 << 1,

	MenuAttrsDefault = (MenuAttrPopOnEscape | MenuAttrRegisterPushPop)
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

class BaseMenu : public Rml::EventListener
{
public:
	virtual ~BaseMenu();

	const char* Name() const;
	const char* RmlFilePath() const;

	const MenuRequest* CurrentRequest() const;
	void ClearCurrentRequest();

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor);
	void DocumentLoaded(Rml::ElementDocument* document);
	void DocumentUnloaded(Rml::ElementDocument* document);
	virtual void Update(float currentTime);

	void ProcessEvent(Rml::Event& event) override;

protected:
	BaseMenu(const char* name, const char* rmlFilePath, size_t flags = MenuAttrsDefault);
	void SetCurrentRequest(MenuRequestType requestType, const Rml::VariantList& args = Rml::VariantList());

	virtual bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor);
	virtual void DocumentLoadedInternal(Rml::ElementDocument* document);
	virtual void DocumentUnloadedInternal(Rml::ElementDocument* document);

private:
	void HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	const char* m_Name;
	const char* m_RmlFilePath;
	size_t m_AttrFlags;
	std::unique_ptr<MenuRequest> m_Request;
};
