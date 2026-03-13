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

	explicit MenuRequest(MenuRequestType inRequestType, Rml::VariantList&& inArgs = Rml::VariantList()) :
		requestType(inRequestType),
		args(inArgs)
	{
	}
};

class BaseMenu
{
public:
	virtual ~BaseMenu() = default;

	const char* Name() const;
	const char* RmlFilePath() const;
	const Rml::String& FallbackRml() const;

	const MenuRequest* CurrentRequest() const;
	void ClearCurrentRequest();

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor);
	virtual void Update(float currentTime) = 0;

protected:
	BaseMenu(const char* name, const char* rmlFilePath);

	virtual bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor);

	Rml::String m_FallbackRml;

private:
	void HandlePushMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);
	void HandlePopMenu(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	const char* m_Name;
	const char* m_RmlFilePath;
	std::unique_ptr<MenuRequest> m_Request;
};
