#pragma once

#include "utlstring.h"
#include "utlhashmap.h"
#include "iterators/utlhashmapconstiterator.h"

class CBotProfileTable
{
public:
	struct ProfileData
	{
		CUtlString profileName;
		CUtlString skin;
		CUtlString playerName;
	};

	typedef CUtlHashMapConstIterator<CUtlString, ProfileData> ConstIterator;

	CBotProfileTable();

	void Clear();
	size_t Count() const;

	bool ProfileExists(const CUtlString& name) const;

	ProfileData* GetProfile(const CUtlString& name);
	const ProfileData* GetProfile(const CUtlString& name) const;

	ProfileData& CreateProfile(const CUtlString& name);
	void RemoveProfile(const CUtlString& name);

	ConstIterator CBegin() const;
	ConstIterator CEnd() const;

private:
	typedef CUtlHashMap<CUtlString, ProfileData> HashTable;

	HashTable m_Table;
	CUtlVector<uint32_t> m_IndexToHash;
};
