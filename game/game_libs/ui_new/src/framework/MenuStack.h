#pragma once

#include <vector>
#include <RmlUi/Core/Types.h>

class MenuDirectory;
struct MenuDirectoryEntry;
struct MenuRequest;

class MenuStack
{
public:
	enum class FocusChangeResult
	{
		None,
		SwitchFocusToGame,
		SwitchFocusToConsole
	};

	explicit MenuStack(MenuDirectory* directory);

	bool Push(const MenuDirectoryEntry* menu);
	const MenuDirectoryEntry* Pop();
	void Update(float currentTime);
	FocusChangeResult HandleRequests();

	const MenuDirectoryEntry* Top() const;
	bool IsEmpty() const;
	size_t Size() const;
	void SetVisible(bool visible);

	// Commands
	void CommandPushMenu(const Rml::String& name);
	void CommandPopMenu(const Rml::String& replacement = Rml::String());
	void CommandCutStack(size_t newSize, const Rml::String& topMenu = Rml::String());
	FocusChangeResult CommandSwitchFocus(const Rml::String& target, const Rml::String& menuToReturnTo = Rml::String());

private:
	using MenuVec = std::vector<const MenuDirectoryEntry*>;

	void SetTopDocumentVisible(bool visible, bool clearCurrentRequest = false);
	FocusChangeResult HandleTopMenuRequest(const MenuRequest& request);

	MenuDirectory* m_Directory;
	MenuVec m_Stack;
	bool m_Visible = true;
};
