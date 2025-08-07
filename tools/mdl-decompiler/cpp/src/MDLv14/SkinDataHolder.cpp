#include <stdexcept>
#include "MDLv14/SkinDataHolder.h"

namespace MDLv14
{
	SkinDataHolder::SkinDataHolder()
	{
	}

	SkinDataHolder::SkinDataHolder(size_t skinFamilyCount, size_t skinReferenceCount) :
		m_SkinFamilyCount(skinFamilyCount),
		m_SkinReferenceCount(skinReferenceCount)
	{
		if ( m_SkinFamilyCount < 1 || m_SkinReferenceCount < 1 )
		{
			throw std::invalid_argument(
				"SkinDataHolder: Number of skin families and references must be greater than zero.");
		}

		Clear();
	}

	size_t SkinDataHolder::SkinFamilyCount() const
	{
		return m_SkinFamilyCount;
	}

	size_t SkinDataHolder::SkinReferenceCount() const
	{
		return m_SkinReferenceCount;
	}

	bool SkinDataHolder::IsValid() const
	{
		return m_SkinFamilyCount > 0 && m_SkinReferenceCount > 0;
	}

	void SkinDataHolder::Clear()
	{
		m_Data.clear();

		if ( IsValid() )
		{
			m_Data.resize(m_SkinFamilyCount * m_SkinReferenceCount);
		}
	}

	int16_t& SkinDataHolder::GetEntry(size_t skinFamilyIndex, size_t skinRefIndex)
	{
		return m_Data[LinearIndex(skinFamilyIndex, skinRefIndex)];
	}

	const int16_t& SkinDataHolder::GetEntry(size_t skinFamilyIndex, size_t skinRefIndex) const
	{
		return m_Data[LinearIndex(skinFamilyIndex, skinRefIndex)];
	}

	size_t SkinDataHolder::LinearIndex(size_t skinFamilyIndex, size_t skinRefIndex) const
	{
		if ( skinFamilyIndex >= m_SkinFamilyCount )
		{
			throw std::invalid_argument("SkinDataHolder: Skin family index was out of range.");
		}

		if ( skinRefIndex >= m_SkinReferenceCount )
		{
			throw std::invalid_argument("SkinDataHolder: Skin reference index was out of range.");
		}

		return (skinFamilyIndex * m_SkinReferenceCount) + skinRefIndex;
	}
}  // namespace MDLv14
