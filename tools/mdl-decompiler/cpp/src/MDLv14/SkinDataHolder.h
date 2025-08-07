#pragma once

#include <cstdint>
#include <vector>

namespace MDLv14
{
	class SkinDataHolder
	{
	public:
		SkinDataHolder();
		SkinDataHolder(size_t skinFamilyCount, size_t skinReferenceCount);

		size_t SkinFamilyCount() const;
		size_t SkinReferenceCount() const;
		bool IsValid() const;
		void Clear();

		int16_t& GetEntry(size_t skinFamilyIndex, size_t skinRefIndex);
		const int16_t& GetEntry(size_t skinFamilyIndex, size_t skinRefIndex) const;

	private:
		size_t LinearIndex(size_t skinFamilyIndex, size_t skinRefIndex) const;

		size_t m_SkinFamilyCount = 0;
		size_t m_SkinReferenceCount = 0;
		std::vector<int16_t> m_Data;
	};
}
