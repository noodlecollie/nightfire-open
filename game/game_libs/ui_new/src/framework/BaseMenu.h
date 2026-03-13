#pragma once

#include <memory>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Variant.h>

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

class BaseMenu
{
public:
	virtual ~BaseMenu();

	const char* Name() const;
	const char* RmlFilePath() const;

	const MenuRequest* CurrentRequest() const;
	void SetCurrentRequest(MenuRequestType requestType, const Rml::VariantList& args = Rml::VariantList());
	void ClearCurrentRequest();

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor);
	virtual void DocumentLoaded(Rml::ElementDocument* document);
	virtual void DocumentUnloaded(Rml::ElementDocument* document);
	virtual void Update(float currentTime);

protected:
	BaseMenu(const char* name, const char* rmlFilePath);

	virtual bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor);

private:
	void HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	const char* m_Name;
	const char* m_RmlFilePath;
	std::unique_ptr<MenuRequest> m_Request;
};
