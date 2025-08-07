#pragma once

#include "utlstring.h"
#include "utlvector.h"

class CBotRegister
{
public:
	static CBotRegister& StaticInstance();

	bool Add(const CUtlString& profileName, const CUtlString& customName = CUtlString());
	void Clear();
	size_t Count() const;

	CUtlString ProfileName(uint32_t index) const;
	CUtlString CustomName(uint32_t index) const;

private:
	struct ListEntry
	{
		CUtlString ProfileName;
		CUtlString CustomName;
	};

	CUtlVector<ListEntry> m_BotList;
};
