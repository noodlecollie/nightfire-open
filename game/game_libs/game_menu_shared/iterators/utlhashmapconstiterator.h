#pragma once

#include "utlhashmap.h"

// Assumes the map will exist unchanged for the entirety of this iterator's lifetime.
template<typename K, typename T, typename L = EqualityFunctor<K>, typename H = HashFunctor<K>>
class CUtlHashMapConstIterator
{
public:
	typedef CUtlHashMap<K, T, L, H> HashMap;
	typedef typename HashMap::KeyType_t KeyType;
	typedef typename HashMap::ElemType_t ElementType;

	static inline CUtlHashMapConstIterator CBegin(const HashMap& map)
	{
		return CUtlHashMapConstIterator(map);
	}

	static inline CUtlHashMapConstIterator CEnd(const HashMap& map)
	{
		CUtlHashMapConstIterator it(map);
		it.m_Index = it.m_Map->InvalidIndex();
		return it;
	}

	inline explicit CUtlHashMapConstIterator(const HashMap& map) :
		m_Map(&map),
		m_Index(0)
	{
		IncrementToNextValidIndex(true);
	}

	inline CUtlHashMapConstIterator(const CUtlHashMapConstIterator& other) :
		m_Map(other.m_Map),
		m_Index(other.m_Index)
	{
	}

	inline bool operator==(const CUtlHashMapConstIterator& other) const
	{
		if ( other.m_Map != m_Map )
		{
			return false;
		}

		if ( !IsValid() )
		{
			return !other.IsValid();
		}
		else
		{
			return other.IsValid() && other.m_Index == m_Index;
		}
	}

	inline bool operator!=(const CUtlHashMapConstIterator& other) const
	{
		return !(*this == other);
	}

	inline CUtlHashMapConstIterator CEnd() const
	{
		return CEnd(m_Map);
	}

	inline bool IsValid() const
	{
		return m_Map->IsValidIndex(m_Index);
	}

	// Prefix - increment and return incremented version.
	inline CUtlHashMapConstIterator& operator++()
	{
		IncrementToNextValidIndex();
		return *this;
	}

	// Postfix - cache a copy, increment and return old version.
	inline CUtlHashMapConstIterator& operator++(int)
	{
		CUtlHashMapConstIterator old(*this);
		IncrementToNextValidIndex();
		return old;
	}

	// If the iterator is not valid, behaviour is undefined.
	inline const KeyType& Key() const
	{
		return m_Map->Key(m_Index);
	}

	inline const ElementType& Value() const
	{
		return m_Map->Element(m_Index);
	}

private:
	// Returns true if there was a next valid index.
	inline bool IncrementToNextValidIndex(bool fromBeginning = false)
	{
		for ( m_Index = fromBeginning ? 0 : m_Index + 1; m_Index < m_Map->MaxElement(); ++m_Index )
		{
			if ( m_Map->IsValidIndex(m_Index) )
			{
				return true;
			}
		}

		// By here, if we have not reached a valid index, there are no more indices to try.
		Invalidate();
		return false;
	}

	inline void Invalidate()
	{
		m_Index = m_Map->InvalidIndex();
	}

	const HashMap* m_Map;
	typename HashMap::IndexType_t m_Index;
};
