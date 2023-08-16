#pragma once

#include <functional>
#include "utlstring.h"
#include "utlhashmap.h"

class CBasePlayer;

class CClientCommandRegister
{
public:
	using CommandFunc = std::function<void(CBasePlayer*)>;

	bool AddCommand(const char* commandName, CommandFunc&& callback);
	bool ExecuteCommand(const char* commandName, CBasePlayer* player) const;
	void Clear();

private:
	CUtlHashMap<CUtlString, CommandFunc> m_CommandMap;
};
