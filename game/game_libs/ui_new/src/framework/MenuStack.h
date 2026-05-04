#pragma once

#include <vector>
#include <RmlUi/Core/Types.h>

class MenuDirectory;
struct MenuDirectoryEntry;
struct MenuRequest;

class MenuStack
{
public:
	explicit MenuStack(MenuDirectory* directory);

	bool Push(const MenuDirectoryEntry* menu);
	const MenuDirectoryEntry* Pop();
	void Update(float currentTime);
	void HandleRequests();

	const MenuDirectoryEntry* Top() const;
	bool IsEmpty() const;
	size_t Size() const;

	// Commands
	void CommandPushMenu(const Rml::String& name);
	void CommandPopMenu(const Rml::String& replacement = Rml::String());
	void CommandCutStack(size_t newSize, const Rml::String& topMenu = Rml::String());

private:
	using MenuVec = std::vector<const MenuDirectoryEntry*>;

	void SetTopDocumentVisible(bool visible, bool clearCurrentRequest = false);
	void HandleTopMenuRequest(const MenuRequest& request);

	MenuDirectory* m_Directory;
	MenuVec m_Stack;
};
