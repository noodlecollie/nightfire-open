#include "client/clientCommandRegister.h"
#include "standard_includes.h"

bool CClientCommandRegister::AddCommand(const char* commandName, CommandFunc&& callback)
{
	if ( !commandName || !(*commandName) )
	{
		ALERT(at_error, "Cannot create command with empty name.\n");
		return false;
	}

	if ( !callback )
	{
		ALERT(at_error, "Cannot create command \"%s\" with no callback.\n", commandName);
		return false;
	}

	CUtlString nameString(commandName);

	if ( m_CommandMap.HasElement(nameString) )
	{
		ALERT(at_error, "Cannot create command \"%s\": a command with this name already exists.\n", commandName);
		return false;
	}

	m_CommandMap.Insert(nameString, std::move(callback));
	return true;
}

bool CClientCommandRegister::ExecuteCommand(const char* commandName, CBasePlayer* player) const
{
	if ( !commandName || !(*commandName) || !player )
	{
		return false;
	}

	CUtlString nameString(commandName);

	int index = m_CommandMap.Find(nameString);

	if ( !m_CommandMap.IsValidIndex(index) )
	{
		return false;
	}

	m_CommandMap[index](player);
	return true;
}

void CClientCommandRegister::Clear()
{
	m_CommandMap.Purge();
}
