#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Variant.h>

enum class MenuRequestType
{
	// Push a menu onto the stack.
	PushMenu,

	// Pop a menu from the stack, optionally specifying
	// another to take its place.
	PopMenu,

	// Cut the stack down to a specific size, and then
	// optionally push a new menu on top.
	CutStack,

	// Switch from the menu to the game or console,
	// optionally specifying a menu to show when the
	// user changes back to use the menu again.
	SwitchFocus,
};

namespace PushMenuRequest
{
	static constexpr MenuRequestType REQUEST_TYPE = MenuRequestType::PushMenu;
	static constexpr const char* const OPTION_MENU = "menu";
}  // namespace PushMenuRequest

namespace PopMenuRequest
{
	static constexpr MenuRequestType REQUEST_TYPE = MenuRequestType::PopMenu;
	static constexpr const char* const OPTION_NEW_MENU = "newMenu";
}  // namespace PopMenuRequest

namespace CutStackRequest
{
	static constexpr MenuRequestType REQUEST_TYPE = MenuRequestType::CutStack;
	static constexpr const char* const OPTION_NEW_SIZE = "newSize";
	static constexpr const char* const OPTION_NEW_MENU = "newMenu";
}  // namespace CutStackRequest

namespace SwitchFocusRequest
{
	static constexpr MenuRequestType REQUEST_TYPE = MenuRequestType::SwitchFocus;
	static constexpr const char* const OPTION_TARGET = "target";
	static constexpr const char* const OPTION_NEW_MENU = "newMenu";
}  // namespace SwitchFocusRequest

struct MenuRequest
{
	MenuRequestType requestType;
	Rml::Dictionary options;

	explicit MenuRequest(MenuRequestType inRequestType, Rml::Dictionary inOptions = Rml::Dictionary()) :
		requestType(inRequestType),
		options(std::move(inOptions))
	{
	}

	template<typename T>
	bool GetOptionInto(const Rml::String& key, T& outValue) const
	{
		const Rml::Dictionary::const_iterator it = options.find(key);

		if ( it == options.end() )
		{
			return false;
		}

		return it->second.GetInto(outValue);
	}
};
