#include "botregister.h"

static constexpr size_t MAX_BOT_COUNT = 31;

CBotRegister& CBotRegister::StaticInstance()
{
	static CBotRegister reg;
	return reg;
}

bool CBotRegister::Add(const CUtlString& profileName, const CUtlString& customName)
{
	if ( static_cast<size_t>(m_BotList.Count()) >= MAX_BOT_COUNT )
	{
		return false;
	}

	m_BotList.AddToTail({profileName, customName});
	return true;
}

void CBotRegister::Clear()
{
	m_BotList.Purge();
}

size_t CBotRegister::Count() const
{
	return m_BotList.Count();
}

CUtlString CBotRegister::ProfileName(uint32_t index) const
{
	if ( index >= static_cast<uint32_t>(m_BotList.Count()) )
	{
		return CUtlString();
	}

	return m_BotList[index].ProfileName;
}

CUtlString CBotRegister::CustomName(uint32_t index) const
{
	if ( index >= static_cast<uint32_t>(m_BotList.Count()) )
	{
		return CUtlString();
	}

	return m_BotList[index].CustomName;
}
