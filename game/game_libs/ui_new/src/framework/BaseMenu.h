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
	class Variant;
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
	void ClearCurrentRequest();

	virtual bool SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor);
	virtual void DocumentLoaded(Rml::ElementDocument* document);
	virtual void DocumentUnloaded(Rml::ElementDocument* document);
	virtual void Update(float currentTime);

protected:
	BaseMenu(const char* name, const char* rmlFilePath);
	void SetCurrentRequest(MenuRequestType requestType, const Rml::VariantList& args = Rml::VariantList());

private:
	const char* m_Name;
	const char* m_RmlFilePath;
	std::unique_ptr<MenuRequest> m_Request;
};
