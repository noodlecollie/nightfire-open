#include "botprofiletable.h"
#include "utlvector.h"
#include "standard_includes.h"

static const CBotProfileTable::ProfileData DEFAULT_PROFILE = {
	"default_profile",
	"mp_mi6_stealth",
	"Default Bot",
};

CBotProfileTable::CBotProfileTable()
{
}

bool CBotProfileTable::ProfileExists(const CUtlString& name) const
{
	return m_Table.HasElement(name);
}

CBotProfileTable::ProfileData& CBotProfileTable::CreateProfile(const CUtlString& name)
{
	m_Table.Remove(name);
	HashTable::IndexType_t index = m_Table.Insert(name);

	ProfileData& data = m_Table.Element(index);
	data.profileName = name;

	return data;
}

CBotProfileTable::ProfileData* CBotProfileTable::GetProfile(const CUtlString& name)
{
	HashTable::IndexType_t index = m_Table.Find(name);
	return index != m_Table.InvalidIndex() ? &m_Table.Element(index) : NULL;
}

const CBotProfileTable::ProfileData* CBotProfileTable::GetProfile(const CUtlString& name) const
{
	HashTable::IndexType_t index = m_Table.Find(name);
	return index != m_Table.InvalidIndex() ? &m_Table.Element(index) : NULL;
}

CBotProfileTable::ProfileData& CBotProfileTable::GetRandomProfile()
{
	return const_cast<CBotProfileTable::ProfileData&>(const_cast<const CBotProfileTable*>(this)->GetRandomProfile());
}

const CBotProfileTable::ProfileData& CBotProfileTable::GetRandomProfile() const
{
	if ( m_Table.Count() < 1 )
	{
		return DEFAULT_PROFILE;
	}

	int index = RANDOM_LONG(0, m_Table.Count() - 1);

	FOR_EACH_HASHMAP(m_Table, tableIndex)
	{
		if ( index-- == 0 )
		{
			return m_Table.Element(tableIndex);
		}
	}

	ASSERT(false);
	return DEFAULT_PROFILE;
}

void CBotProfileTable::RemoveProfile(const CUtlString& name)
{
	m_Table.Remove(name);
}

void CBotProfileTable::Clear()
{
	m_Table.Purge();
}

size_t CBotProfileTable::Count() const
{
	return m_Table.Count();
}

CBotProfileTable::ConstIterator CBotProfileTable::CBegin() const
{
	return ConstIterator::CBegin(m_Table);
}

CBotProfileTable::ConstIterator CBotProfileTable::CEnd() const
{
	return ConstIterator::CEnd(m_Table);
}
