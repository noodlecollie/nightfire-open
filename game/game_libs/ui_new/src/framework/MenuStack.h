#pragma once

#include <vector>
#include <RmlUi/Core/Types.h>

class MenuDirectory;
class MenuDirectoryEntry;
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

private:
	using MenuVec = std::vector<const MenuDirectoryEntry*>;

	void SetTopDocumentVisible(bool visible);
	void HandleTopMenuRequest(const MenuRequest& request);
	void HandlePushMenuRequest(const Rml::String& name);
	void HandlePopMenuRequest();

	MenuDirectory* m_Directory;
	MenuVec m_Stack;
};
